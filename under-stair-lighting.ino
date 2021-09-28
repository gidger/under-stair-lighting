// Define light pins and num of stairs.
const int led_pins[] = {23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53};
const int num_lights = 16;
const int num_lights_music = 12; // Music mode ignores the top four stairs (they're around a corner and can't be seen from the bottom).

// Define button pin and state.
const int button = 8;
int button_state = 0;

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

// Define delay between concurrent stairs lighting up in music mode.
const int interlight_delay_music = 10;

// Define threshold distances for top and bottom sensors.
const int threshold_distance_top = 60;
const int threshold_distance_bot = 105;

// Define cooldown time before the next trigger is allowed.
const int cooldown_time = 1500;

void setup() {
    Serial.begin(9600);

    // Stair light pins.
    for(int i = 0; i<num_lights; i++){
      pinMode(led_pins[i], OUTPUT);
    }

    // Button pin.
    pinMode(button, INPUT_PULLUP);

    // Ultrasonic sensor pins.
    pinMode(sensor_bottom_trig, OUTPUT);
    pinMode(sensor_bottom_echo, INPUT);
    pinMode(sensor_top_trig, OUTPUT);
    pinMode(sensor_top_echo, INPUT);

    // Update the button state.
    button_state = digitalRead(button);

    //Check if was in music mode when powered up.
    if (button_state == HIGH) {
      active_mode = 2;
    }

    // Appropriate flash animation.
    lights_flash(active_mode); 
}
 
void loop() {

  // Check button and update mode accordingly.
  change_mode();

  // If in standard mode.
  if (active_mode == 1) {
      // Define vars for calculated distance for top and bottom ultrasonic sensors. Also default vaule for animation direction.
      long distance_bot, distance_top;
      String animation_direction = "none";

      // Get distances from top and bottom sensors.
      distance_top = get_top_distance();
      distance_bot = get_bot_distance();
      
      // If top distance is less than calibrated threshold.
      // Note: uss-calibration.ino bundled with this code's repo has code to determine the threshold.
      if (distance_top < threshold_distance_top) {
        animation_direction = "down";
      }

      // If bottom distance is less than calibrated threshold.
      if (distance_bot < threshold_distance_bot) {
        animation_direction = "up";
      }

      // If top or bottom distance is less than threshold, do animation on & off.
      if (animation_direction != "none") {
        lights_on(animation_direction);   // Upwards/downwards on animation.
        delay(lightup_time);              // All lights on.

        // Refresh distances.
        distance_top = get_top_distance();
        distance_bot = get_bot_distance();

        // Don't run the off animation right away if someone is sitting at the top/bottom of the stairs.
        // Note: a proper interupt library should be implemented here for it to allow sensors picking up motion even if all lights are on.
        while (distance_bot < threshold_distance_bot || distance_top < threshold_distance_top) {
          delay(3000);
          
          // Refresh distances.
          distance_top = get_top_distance();
          distance_bot = get_bot_distance();
        }

        lights_off(animation_direction);  // Upwards/downwards off animation.
        delay(cooldown_time);             // All lights off.
      }
  }

  // If in music mode.
  else if (active_mode == 2) {
    // TODO: Put bass visualized code here.
  }

  delay(25);
}

void change_mode(){
  // Check if the button is pressed or not. Note: this requires a toggle switch.
  button_state = digitalRead(button);
  
  // Change the mode as needed.
  if (button_state == HIGH && active_mode != 2) {
    active_mode = 2;
    // Double flash animation.
    lights_flash(active_mode); 
  }
  else if (button_state == LOW && active_mode != 1) {
    active_mode = 1;
    // Single flash animation.
    lights_flash(active_mode);
  }
}

long get_top_distance(){
  // Returns the distance read by the top sensor.
  long duration_top, distance_top;
  digitalWrite(sensor_top_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_top_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_top_trig, LOW);
  duration_top = pulseIn(sensor_top_echo, HIGH);
  distance_top = (duration_top/2) / 29.1;
  return distance_top;
}

long get_bot_distance(){
  // Returns the distance read by the bottom sensor.
  long duration_bot, distance_bot;
  digitalWrite(sensor_bottom_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_bottom_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_bottom_trig, LOW);
  duration_bot = pulseIn(sensor_bottom_echo, HIGH);
  distance_bot = (duration_bot/2) / 29.1;
  return distance_bot;
}

void lights_on(String direction) {
  if (direction == "up"){
    // Upwards on animation.
    // Turn on each light from bottom to top, delay for a time between lights.
    for(int i = 0; i<num_lights; i++){
      digitalWrite(led_pins[i], HIGH);
      delay(interlight_delay);
    }
  }
  else if (direction == "down"){
    // Downwards on animation.
    // Turn on each light from top to bottom, delay for a time between lights.
    for(int i = num_lights-1; i>=0; i--){
      digitalWrite(led_pins[i], HIGH);
      delay(interlight_delay);
    }
  }
}

void lights_off(String direction) {
  if (direction == "up"){
    // Upwards off animation.
    // Turn off each light from bottom to top, delay for a time between lights.
    for(int i = 0; i<num_lights; i++){
      digitalWrite(led_pins[i], LOW);
      delay(interlight_delay);
    }
  }
  else if (direction == "down"){
    // Downwards off animation.
    // Turn off each light from top to bottom, delay for a time between lights.
    for(int i = num_lights-1; i>=0; i--){
      digitalWrite(led_pins[i], LOW);
      delay(interlight_delay);
    }
  }
}

void lights_flash(int flash_count){
  // Turn off all lights and pause for 0.5s.
  for(int i = 0; i<num_lights; i++){
    digitalWrite(led_pins[i], LOW);
  }
  delay(500);

  // Flash lights on and off a number of times.
  for(int j = 0; j<flash_count; j++){
    
    // Turn lights on.
    for(int i = 0; i<num_lights; i++){
      digitalWrite(led_pins[i], HIGH);
    }
    delay(250); // Pause for 0.25s with lights on.
    
    // Turn lights off.
    for(int i = 0; i<num_lights; i++){
      digitalWrite(led_pins[i], LOW);
    }
    delay(250); // Pause for 0.25s with lights off.
  }
  delay(250); // Pause for 0.25s after animation compete.
}
