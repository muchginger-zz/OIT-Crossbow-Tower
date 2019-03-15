#include <Servo.h>
const unsigned short int trigPin = 9;   // ultrasonic trigger pin
const unsigned short int echoPin = 10;  // ultrasonic echo pin

int triggerRadar(void);
int targetFound(int pos, int distinCM);
void aimCrossbow(int pos);

long duration;      // duration of ultrasonic echo
int distinCM;       // distance to object in CM
//int pos;            // servo position
int targetPos = 0;  // target position
bool found = false; // shoot only once per sweep
Servo radar;
Servo shaft;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  
  radar.attach(5);    // init radar servo pin 5
  shaft.attach(4);    // init crossbow servo pin 4
  //trigger.attach(3);  // init crossbow trigger servo pin 3
  //primer.attach(2);   // init crossbow primer servo pin 2

  // setting servos back to origin
  radar.write(0);     
  shaft.write(0);
  //trigger.write(0);
  //primer.write(0);
}

void loop() {
  int pos = 0;
  found = false;                            // no target found yet
  for (pos = 0; pos <= 180; pos++) {        // start radar servo rotation 0-180
    radar.write(pos);

    distinCM = triggerRadar();              // triggers echo; returns distinCM
    Serial.write("distance CM: ");
    Serial.println(distinCM);
    targetPos = targetFound(pos, distinCM); // if !0, target is found
    Serial.println("Target Found!");
    
    if (targetPos != 0 && found == false) { // if target is identified
      aimCrossbow(targetPos);               // aim crossbow
      Serial.println("FIRING!!!");
      delay(5000);
      //primeCrossbow();                      // prime crossbow
      //fireCrossbow();                       // fire crossbow
    }
  }

  found = false;

  for (pos = 180; pos >= 0; pos--) {        // start servo rotation 165°-15°
    radar.write(pos);
    delay(30);

    distinCM = triggerRadar();              // triggers echo; returns distinCM
    
    targetPos = targetFound(pos, distinCM); // if !0, target is found

    if (targetPos != 0 && found == false) { // if target is identified
      aimCrossbow(targetPos);                 // aim crossbow
      Serial.println("FIRING!!!");
      delay(5000);
      //primeCrossbow();                        // prime crossbow
      //fireCrossbow();                         // fire crossbow
    }  
  }
}

int triggerRadar(void) {                    // returns distance in CM
    digitalWrite(trigPin, LOW);             // OFF
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);            // ON
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);             // OFF
    
    duration = pulseIn(echoPin, HIGH);      // calculate duration
    return duration*0.034/2;                // calculate distance in CM 
}

int targetFound(int pos, int distinCM) {
  if (distinCM < 5) { // test this
    return pos;
  }
  else
    return 0;
}

void aimCrossbow(int pos) {
  int i = 0;
  for (i = 0; i < pos; i++) {
    shaft.write(i);
    delay(10);
  }
  Serial.write("firing at index ");
  Serial.println(pos);
  return;
}
