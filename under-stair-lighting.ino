// Define light pins.
const int light_A = 23; // Bottom stair.
const int light_B = 25;
const int light_C = 27;
const int light_D = 29;
const int light_E = 31;
const int light_F = 33;
const int light_G = 35;
const int light_H = 37;
const int light_I = 39;
const int light_J = 41;
const int light_K = 43;
const int light_L = 45;
const int light_M = 47;
const int light_N = 49;
const int light_O = 51;
const int light_P = 53; // Top stair.

// Define ultrasonic sensor pins.
const int sensor_bottom_trig = 3;
const int sensor_bottom_echo = 2;
const int sensor_top_trig = 5;
const int sensor_top_echo = 4;

// Define time all stairs will be illuminated after the on animation.
const int lightup_time = 6800;

// Define delay between concurrent stairs lighting up.
const int interlight_delay = 150;

// Define cooldown time before the next trigger is allowed.
const int cooldown_time = 1500;
 
void setup() {
    Serial.begin(9600);

    // Output pins.
    pinMode(light_A, OUTPUT);
    pinMode(light_B, OUTPUT);
    pinMode(light_C, OUTPUT);
    pinMode(light_D, OUTPUT);
    pinMode(light_E, OUTPUT);
    pinMode(light_F, OUTPUT);
    pinMode(light_G, OUTPUT);
    pinMode(light_H, OUTPUT);
    pinMode(light_I, OUTPUT);
    pinMode(light_J, OUTPUT);
    pinMode(light_K, OUTPUT);
    pinMode(light_L, OUTPUT);
    pinMode(light_M, OUTPUT);
    pinMode(light_N, OUTPUT);
    pinMode(light_O, OUTPUT);
    pinMode(light_P, OUTPUT);

    // Ultrasonic sensor pins.
    pinMode(sensor_bottom_trig, OUTPUT);
    pinMode(sensor_bottom_echo, INPUT);
    pinMode(sensor_top_trig, OUTPUT);
    pinMode(sensor_top_echo, INPUT);
}
 
void loop() {
  // Define vars for response time and calculated distance for top and bottom ultrasonic sensors.
  long duration_bot, distance_bot, duration_top, distance_top;
  
  // Get distance from bottom sensor.
  digitalWrite(sensor_bottom_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_bottom_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_bottom_trig, LOW);
  duration_bot = pulseIn(sensor_bottom_echo, HIGH);
  distance_bot = (duration_bot/2) / 29.1;

  // Get distance from top sensor.
  digitalWrite(sensor_top_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor_top_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor_top_trig, LOW);
  duration_top = pulseIn(sensor_top_echo, HIGH);
  distance_top = (duration_top/2) / 29.1;

  // If bottom distance is less than calibrated threshold.
  // Note: uss-calibration.ino bundled with this code's repo has code to determine the threshold.
  if (distance_bot < 105){
    lights_on_up();         // Upwards on animation.
    delay(lightup_time);    // All lights on.
    lights_off_up();        // Upwards off animation.
    delay(cooldown_time);   // All lights off.
  }

  // If top distance is less than calibrated threshold.
  if (distance_top < 60){
    lights_on_down();       // Downwards on animation.
    delay(lightup_time);    // All lights on.
    lights_off_down();      // Downwards off animation.
    delay(cooldown_time);   // All lights off.
  }
  delay(25);
}

void lights_on_up() {
  // Upwards on animation.
  // Turn on each light from bottom to top, delay for a time between lights.
  digitalWrite(light_A, HIGH);
  delay(interlight_delay);
  digitalWrite(light_B, HIGH);
  delay(interlight_delay);
  digitalWrite(light_C, HIGH);
  delay(interlight_delay);
  digitalWrite(light_D, HIGH);
  delay(interlight_delay);
  digitalWrite(light_E, HIGH);
  delay(interlight_delay);
  digitalWrite(light_F, HIGH);
  delay(interlight_delay);
  digitalWrite(light_G, HIGH);
  delay(interlight_delay);
  digitalWrite(light_H, HIGH);
  delay(interlight_delay);
  digitalWrite(light_I, HIGH);
  delay(interlight_delay);
  digitalWrite(light_J, HIGH);
  delay(interlight_delay);
  digitalWrite(light_K, HIGH);
  delay(interlight_delay);
  digitalWrite(light_L, HIGH);
  delay(interlight_delay);
  digitalWrite(light_M, HIGH);
  delay(interlight_delay);
  digitalWrite(light_N, HIGH);
  delay(interlight_delay);
  digitalWrite(light_O, HIGH);
  delay(interlight_delay);
  digitalWrite(light_P, HIGH);
}

void lights_on_down() {
  // Upwards off animation.
  // Turn off each light from bottom to top, delay for a time between lights.
  digitalWrite(light_P, HIGH);
  delay(interlight_delay);
  digitalWrite(light_O, HIGH);
  delay(interlight_delay);
  digitalWrite(light_N, HIGH);
  delay(interlight_delay);
  digitalWrite(light_M, HIGH);
  delay(interlight_delay);
  digitalWrite(light_L, HIGH);
  delay(interlight_delay);
  digitalWrite(light_K, HIGH);
  delay(interlight_delay);
  digitalWrite(light_J, HIGH);
  delay(interlight_delay);
  digitalWrite(light_I, HIGH);
  delay(interlight_delay);
  digitalWrite(light_H, HIGH);
  delay(interlight_delay);
  digitalWrite(light_G, HIGH);
  delay(interlight_delay);
  digitalWrite(light_F, HIGH);
  delay(interlight_delay);
  digitalWrite(light_E, HIGH);
  delay(interlight_delay);
  digitalWrite(light_D, HIGH);
  delay(interlight_delay);
  digitalWrite(light_C, HIGH);
  delay(interlight_delay);
  digitalWrite(light_B, HIGH);
  delay(interlight_delay);
  digitalWrite(light_A, HIGH);
}

void lights_off_up() {
  // Downwards on animation.
  // Turn on each light from top to bottom, delay for a time between lights.
  digitalWrite(light_A, LOW);
  delay(interlight_delay);
  digitalWrite(light_B, LOW);
  delay(interlight_delay);
  digitalWrite(light_C, LOW);
  delay(interlight_delay);
  digitalWrite(light_D, LOW);
  delay(interlight_delay);
  digitalWrite(light_E, LOW);
  delay(interlight_delay);
  digitalWrite(light_F, LOW);
  delay(interlight_delay);
  digitalWrite(light_G, LOW);
  delay(interlight_delay);
  digitalWrite(light_H, LOW);
  delay(interlight_delay);
  digitalWrite(light_I, LOW);
  delay(interlight_delay);
  digitalWrite(light_J, LOW);
  delay(interlight_delay);
  digitalWrite(light_K, LOW);
  delay(interlight_delay);
  digitalWrite(light_L, LOW);
  delay(interlight_delay);
  digitalWrite(light_M, LOW);
  delay(interlight_delay);
  digitalWrite(light_N, LOW);
  delay(interlight_delay);
  digitalWrite(light_O, LOW);
  delay(interlight_delay);
  digitalWrite(light_P, LOW);
}


void lights_off_down() {
  // Downwards off animation.
  // Turn off each light from top to bottom, delay for a time between lights.
  digitalWrite(light_P, LOW);
  delay(interlight_delay);
  digitalWrite(light_O, LOW);
  delay(interlight_delay);
  digitalWrite(light_N, LOW);
  delay(interlight_delay);
  digitalWrite(light_M, LOW);
  delay(interlight_delay);
  digitalWrite(light_L, LOW);
  delay(interlight_delay);
  digitalWrite(light_K, LOW);
  delay(interlight_delay);
  digitalWrite(light_J, LOW);
  delay(interlight_delay);
  digitalWrite(light_I, LOW);
  delay(interlight_delay);
  digitalWrite(light_H, LOW);
  delay(interlight_delay);
  digitalWrite(light_G, LOW);
  delay(interlight_delay);
  digitalWrite(light_F, LOW);
  delay(interlight_delay);
  digitalWrite(light_E, LOW);
  delay(interlight_delay);
  digitalWrite(light_D, LOW);
  delay(interlight_delay);
  digitalWrite(light_C, LOW);
  delay(interlight_delay);
  digitalWrite(light_B, LOW);
  delay(interlight_delay);
  digitalWrite(light_A, LOW);
}
