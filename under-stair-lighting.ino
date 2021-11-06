#include <arduinoFFT.h>

// Define light pins and num of stairs.
const int led_pins[] = {23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53};
const int num_lights = 16;
const int num_lights_music = 12; // Music mode ignores the top four stairs (they're around a corner and can't be seen from the bottom).

// Define button pin and state.
const int button = 8;
int button_state = 0;

// Define light pin.
const int mic_pin = A0;

// Store the active mode. 1=standard, 2=music.
int active_mode = 1;

// Define ultrasonic sensor pins.
const int sensor_bottom_trig = 3;
const int sensor_bottom_echo = 2;
const int sensor_top_trig = 5;
const int sensor_top_echo = 4;

// Define time all stairs will be illuminated after the on animation.
const int lightup_time = 6800;

// Define delay between concurrent stairs lighting up.
const int interlight_delay = 150;

// Define threshold distances for top and bottom sensors.
const int threshold_distance_top = 60;
const int threshold_distance_bot = 95;

// Define cooldown time before the next trigger is allowed.
const int cooldown_time = 1500;


// --- ADDITIONAL MUSIC MODE DECLARATIONS ---

// Define a var to keep track of the highest lit stair in music mode. This gets reset when switching modes. Note that this will keep track from 1-12, not 0-11. 0 means nothing lit.
int highest_lit_music = 0;

// Define delay times for music mode.
const int interlight_delay_music = 10;  // Concurrent stairs lighting upwards when sounds are detected.
const int top_delay_music = 50;         // Short delay after top light is illuminated when sounds are detected.
const int drop_delay_music = 75;        // Delay for decaying lights off from top to bottom when no sound is detected.

// Define values for 
const int noise_threshold = 300;                    // Minimum threshold for a frequency bin to go into the overall bass calculation.
const int amplitude_divisor = 500;                  // Amplitude for converting bass values to number of stairs to light up.
const double bin_multipliers[] = {5,4,3,1.5,.5};    // Multipliers for bass bins. These result in lower frequencies being given more importance in the overall bass calculations. Mid values given less importance.

// Define vars for keeping track of the last time the lights decayed and the last time a audio sample was taken.
unsigned long last_drop_time = 0;
unsigned long last_sample_time = 0;

// Define number of sample and the sampling frequency for the Forier Transforms.
const int sample_count = 64;      // Note: Must be a power of 2.
const int sampling_freq = 4096;   // Note: Value in Hz, must be less than ~9,000 on an Uno or equvilent due to ADC and other overhead.

// Var for the lenth of the audio sampling period in microseconds. Calculated during setup. 
unsigned int sampling_period_micro;

// Arrays for keeping track of the real and imaginary outputs of the Forier Transform.
double v_real[sample_count];
double v_imaginary[sample_count];

// Define vars for keeping track of the current time in milli and micro seconds.
unsigned long time_now_milli = 0;
unsigned long time_now_micro = 0;

// Create an FFT object from the arduinoFFT library.
arduinoFFT FFT = arduinoFFT();

void setup() {
    Serial.begin(115200);

    // Setup stair light pins.
    for(int i = 0; i<num_lights; i++){
      pinMode(led_pins[i], OUTPUT);
    }

    // Setup button pin.
    pinMode(button, INPUT_PULLUP);

    // Setup ultrasonic sensor pins.
    pinMode(sensor_bottom_trig, OUTPUT);
    pinMode(sensor_bottom_echo, INPUT);
    pinMode(sensor_top_trig, OUTPUT);
    pinMode(sensor_top_echo, INPUT);

    // Setup mic pin.
    pinMode(mic_pin, INPUT);

    // Update the button state.
    button_state = digitalRead(button);

    //Check if was in music mode when powered up. If so, update active_mode.
    if (button_state == HIGH) {
      active_mode = 2;
    }

    // Calculate the sampling period.
    sampling_period_micro = round(1000000 * (1.0 / sampling_freq));

    // Perform appropriate flash animation.
    lights_flash(active_mode); 
}
 
void loop() {
  // Check button and update mode accordingly.
  change_mode();

  // If in standard mode.
  if (active_mode == 1) {
    standard_mode();
  }

  // Else if in music mode.
  else if (active_mode == 2) {
    music_mode();     
  }

  smart_pause_milli(5);
}

void change_mode(){
  // Check if the button is pressed or not. Note: this requires a toggle switch.

  // Update the button state. 
  button_state = digitalRead(button);
  
  // Change the mode as needed.
  if (button_state == HIGH && active_mode != 2) {
    active_mode = 2;
    lights_flash(active_mode); // Double flash animation.
    highest_lit_music = 0;     // Reset highest lit music.
  }
  else if (button_state == LOW && active_mode != 1) {
    active_mode = 1;
    lights_flash(active_mode); // Single flash animation.
  }
}

void standard_mode(){
  // Lights will cascade on/off in the corret direction based on ultrasonic sensors detecting someone passing by.

  // Define vars for calculated distance for top and bottom ultrasonic sensors. Set default vaule for animation direction.
  long distance_bot, distance_top;
  String animation_direction = "none";

  // Get distances from top and bottom sensors.
  distance_top = get_top_distance();
  distance_bot = get_bot_distance();
  
  // If top distance is less than calibrated threshold.
  // Note: uss-calibration.ino bundled with this code's repo has code to determine the threshold.
  if (distance_top < threshold_distance_top ) {
    animation_direction = "down";
  }

  // If bottom distance is less than calibrated threshold.
  else if (distance_bot < threshold_distance_bot) {
    animation_direction = "up";
  }

  // If top or bottom distance is less than threshold, do animation on & off.
  if (animation_direction != "none") {
    lights_on(animation_direction);   // Upwards/downwards on animation.
    smart_pause_milli(lightup_time);  // All lights on.

    // Refresh sensor distances.
    distance_top = get_top_distance();
    distance_bot = get_bot_distance();

    // Don't run the off animation right away if someone is sitting at the top/bottom of the stairs.
    // Note: a proper interupt library should be implemented here for it to allow sensors picking up motion even if all lights are on.
    while (distance_bot < threshold_distance_bot || distance_top < threshold_distance_top) {
      smart_pause_milli(3000);
      
      // Refresh distances.
      distance_top = get_top_distance();
      distance_bot = get_bot_distance();
    }

    // Casccade lights off and pause for the cooldown time.
    lights_off(animation_direction);
    smart_pause_milli(cooldown_time);
  }
}

void music_mode(){
  // Lights will react to the bass detected from the microphone. The more bass, the more lights that will turn on.
  // Only the first 12 stairs are used as the remainder are not visibile from the ground floor.

  // Define bass sum and bass sum scaled vars. These will be used to store the ammount of bass picked up by the microphone and the number of lights to illuminate.
  double bass_sum = 0;
  int bass_sum_scaled = 0;
  
  // Get audio samples and save them to the v_real array.
  for(int i=0; i<sample_count; i++){
    last_sample_time = micros();      // Record last sample time.

    v_real[i] = analogRead(mic_pin);  // Read the analog input from the microphone.
    v_imaginary[i] = 0;               // Set v_imaginary to 0.
  
    while((unsigned long)(micros() - last_sample_time) < sampling_period_micro){/* ＣＨＩＬＬ */} // Ensure that the at least the sampling period has passed.
  }

  // Forier transform.
  FFT.Windowing(v_real, sample_count, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(v_real, v_imaginary, sample_count, FFT_FORWARD);
  FFT.ComplexToMagnitude(v_real, v_imaginary, sample_count);

  // Go though the frequency bins resulting from the Forier transform.
  // Not there's acctually 64 bins (32 usuable b/c of Nyquist's theorem), but since we're only looking for the bass values we're only looking at bins 2 through 5.
  // This will give us visiblity of frequencies between 64 and 320Hz.
  // Bins 0 and 1 contain garbage values, so not looking at those.
  for(int i=2; i<6; i++) {
    // If the bin value is greater than the noise threshold, multiply it by the appropriate multiplier and add it to bass_sum.
    if (v_real[i] > noise_threshold){
      bass_sum += v_real[i] * bin_multipliers[i-2]; // i-2 since we're not looking at the first two bins and weights only has 5 elements. 
    }
  }

  // Transform bass_sum into the number of lights to be illuminated based on the audio sample. Divide bass_sum by the amplitude_divisor.
  bass_sum_scaled = round(bass_sum / amplitude_divisor);
  
  // If the claculed number of lights is more than the total number of lights, set to the total number of lights.
  if (bass_sum_scaled > num_lights_music){
    bass_sum_scaled = num_lights_music;
  }

  // If the calcultated number of lights is less than 3, set it to zero. Lights flashign on/off at the bottom of the stairs does not look good.
  if (bass_sum_scaled < 3){
    bass_sum_scaled = 0;
  }
  
  // Bump the lights up to the calculated number.
  lights_music_bump(bass_sum_scaled);

  // Every "drop_delay_music" miliseconds reduce the number of lights that are illumicated by 1 and update highest_lit_music accordingly.
  if ((unsigned long)(millis() - last_drop_time) > drop_delay_music){
    if (highest_lit_music>=0){ // If at least one light is illuminated.
      digitalWrite(led_pins[highest_lit_music], LOW);
      if(highest_lit_music != 0){
        highest_lit_music--;
      }
    }
    last_drop_time = millis(); // Update the last drop time.
  }
}

long get_top_distance(){
  // Returns the distance read by the top sensor.

  long duration_top, distance_top;
  digitalWrite(sensor_top_trig, LOW);
  smart_pause_micro(2);
  digitalWrite(sensor_top_trig, HIGH);
  smart_pause_micro(10);
  digitalWrite(sensor_top_trig, LOW);
  duration_top = pulseIn(sensor_top_echo, HIGH);
  distance_top = (duration_top/2) / 29.1;
  return distance_top;
}

long get_bot_distance(){
  // Returns the distance read by the bottom sensor.

  long duration_bot, distance_bot;
  digitalWrite(sensor_bottom_trig, LOW);
  smart_pause_micro(2);
  digitalWrite(sensor_bottom_trig, HIGH);
  smart_pause_micro(10);
  digitalWrite(sensor_bottom_trig, LOW);
  duration_bot = pulseIn(sensor_bottom_echo, HIGH);
  distance_bot = (duration_bot/2) / 29.1;
  return distance_bot;
}

void lights_on(String direction) {
  // Cascade lights on in the specified direction.

  if (direction == "up"){
    // Upwards on animation.
    // Turn on each light from bottom to top, delay for a time between lights.
    for(int i = 0; i<num_lights; i++){
      digitalWrite(led_pins[i], HIGH);
      smart_pause_milli(interlight_delay);
    }
  }
  else if (direction == "down"){
    // Downwards on animation.
    // Turn on each light from top to bottom, delay for a time between lights.
    for(int i = num_lights-1; i>=0; i--){
      digitalWrite(led_pins[i], HIGH);
      smart_pause_milli(interlight_delay);
    }
  }
}

void lights_off(String direction) {
  // Cascade lights off in the specified direction.

  if (direction == "up"){
    // Upwards off animation.
    // Turn off each light from bottom to top, delay for a time between lights.
    for(int i = 0; i<num_lights; i++){
      digitalWrite(led_pins[i], LOW);
      smart_pause_milli(interlight_delay);
    }
  }
  else if (direction == "down"){
    // Downwards off animation.
    // Turn off each light from top to bottom, delay for a time between lights.
    for(int i = num_lights-1; i>=0; i--){
      digitalWrite(led_pins[i], LOW);
      smart_pause_milli(interlight_delay);
    }
  }
}

void lights_flash(int flash_count){
  // Flash all lights flash_count number of times.

  // Turn off all lights and pause for 0.5s.
  for(int i = 0; i<num_lights; i++){
    digitalWrite(led_pins[i], LOW);
  }
  smart_pause_milli(500);

  // Flash lights on and off a number of times.
  for(int i = 0; i<flash_count; i++){
    
    // Turn lights on.
    for(int j = 0; j<num_lights; j++){
      digitalWrite(led_pins[j], HIGH);
    }
    smart_pause_milli(250); // Pause for 0.25s with lights on.
    
    // Turn lights off.
    for(int j = 0; j<num_lights; j++){
      digitalWrite(led_pins[j], LOW);
    }
    smart_pause_milli(250); // Pause for 0.25s with lights off.
  }
  smart_pause_milli(250); // Pause for 0.25s after animation compete.
}

void lights_music_bump(int top_stair_number){
  // Quickly cascade up to the calculated stair number, top_stair_number.

  // If input is bigger than what's already illuminated.
  if (top_stair_number > highest_lit_music){
    for(int i = highest_lit_music; i<top_stair_number; i++){
      digitalWrite(led_pins[i], HIGH);
      smart_pause_milli(interlight_delay_music);
    }

    // After cascade, delay for top_delay_music miliseconds and update highest_lit_music.
    smart_pause_milli(top_delay_music);
    highest_lit_music = top_stair_number;
  }
}

void smart_pause_milli(int pause_time_in_milli){
  // Pause for pause_time_in_milli milliseconds.

  time_now_milli = millis();
  while((unsigned long)(millis() - time_now_milli) < pause_time_in_milli){/* ＣＨＩＬＬ */}
}

void smart_pause_micro(int pause_time_in_micro){
  // Pause for pause_time_in_micro microseconds.

  time_now_micro = micros();
  while((unsigned long)(micros() - last_sample_time) < pause_time_in_micro){/* ＣＨＩＬＬ */}
}
