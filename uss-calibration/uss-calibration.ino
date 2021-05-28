/*
HC-SR04 Ping distance sensor]
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 13 Trig to Arduino pin 12
Red POS to Arduino pin 11
Green POS to Arduino pin 10
560 ohm resistor to both LED NEG and GRD power rail
More info at: http://goo.gl/kJ8Gl
Original code improvements to the Ping sketch sourced from Trollmaker.com
Some code and wiring inspired by http://en.wikiversity.org/wiki/User:Dstaub/robotcar
*/

#define trigPin_bot 3
#define echoPin_bot 2
#define trigPin_top 5
#define echoPin_top 4
#define led 11
int duration_top = 0;
int distance_top = 0;

void setup() {
  Serial.begin (9600);
  //pinMode(trigPin_bot, OUTPUT);
  //pinMode(echoPin_bot, INPUT);
  pinMode(trigPin_top, OUTPUT);
  pinMode(echoPin_top, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  /*long duration_bot, distance_bot, duration_top, distance_top;
  digitalWrite(trigPin_bot, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin_bot, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin_bot, LOW);
  duration_bot = pulseIn(echoPin_bot, HIGH);
  distance_bot = (duration_bot/2) / 29.1;*/

  digitalWrite(trigPin_top, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin_top, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin_top, LOW);
  duration_top = pulseIn(echoPin_top, HIGH);
  distance_top = (duration_top/2) / 29.1;

  /*
  if (distance_bot < 120 || distance_bot > 150) {  // This is where the LED On/Off happens
    digitalWrite(led,LOW); // When the Red condition is met, the Green LED should turn off
    Serial.print(distance_bot);
    Serial.println(" cm - BOT: PAUSED");
    delay(2000);
  }

  if (distance_bot >= 200 || distance_bot <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance_bot);
    Serial.println(" cm - BOT");
  }

  

  */
  /*if (distance_top < 75  || distance_top > 100) {  // This is where the LED On/Off happens
    digitalWrite(led,HIGH); // When the Red condition is met, the Green LED should turn off
    Serial.print(distance_top);
    Serial.println(" cm - TOP: PAUSED");
    delay(2000);
  }*/

  if (distance_top >= 200 || distance_top <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distance_top);
    Serial.println(" cm - TOP");
  }

  /*

  if (distance_top < 60){
    wait_func();
  }

  */
  delay(100); //CHANGE TO 100 FOR MEASURING
}

void wait_func(){
    Serial.print("DOING ACTION FOR 5 SEC");
    delay(5000);
    Serial.println("DONE ACTION: CONTINUING");
}

/*
void setup()
{
     pinMode(2, OUTPUT);  //Set pin 2 as output
     pinMode(3, OUTPUT);
     pinMode(4, OUTPUT);
     pinMode(5, OUTPUT);
     pinMode(6, OUTPUT);
     pinMode(7, OUTPUT);
     pinMode(8, OUTPUT);
     pinMode(9, OUTPUT);
     on_val = 8;
}
void loop()
{
    if (on_val%9 == 8){
      for (int i = 2; i < 10; i++){
        digitalWrite(i, LOW);
      }
      delay(250);
    }
    else {
      digitalWrite((on_val % 9)+2, HIGH);
    }
    on_val++;
    delay(150);
}
*/
