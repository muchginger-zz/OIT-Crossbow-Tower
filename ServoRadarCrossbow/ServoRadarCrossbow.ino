#include <Servo.h>

const unsigned short int trigPin = 9;   // ultrasonic trigger pin
const unsigned short int echoPin = 10;  // ultrasonic echo pin

int triggerRadar(void);                 // function definitions
int targetFound(int pos, int distinCM);
void fireCrossbow(void);
void primeCrossbow(void);

long duration;      // duration of ultrasonic echo
int distinCM;       // distance to object in CM
int pos;            // servo position
int targetPos = 0;  // target position
bool found = false; // shoot only once per sweep

Servo radar;        // ultrasonic radar servo object  
Servo shaft;        // crossbow aiming servo object
Servo primer;       // crossbow cocking servo object
Servo trigger;      // crossbow trigger pull servo object

void setup() {  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.begin(9600);
  
  radar.attach(5);    // init radar servo pin 5
  shaft.attach(4);    // init crossbow servo pin 4
  trigger.attach(3);  // init crossbow trigger servo pin 3
  primer.attach(2);   // init crossbow primer servo pin 2

  // setting servos back to origin
  radar.write(0);     
  shaft.write(0);
  trigger.write(0);
  primer.write(0);
}

void loop() {
  found = false;                            // no target found yet    
  trigger.write(0);                         // reset servo wheels
  primer.write(0);
  for (pos = 0; pos <= 180; pos++) {        // start radar servo rotation 0°-180°
    radar.write(pos);
    delay(30);
     
    distinCM = triggerRadar();              // triggers echo; returns distinCM
    
    Serial.print(pos); // Sends the current degree into the Serial Port
    Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distinCM); // Sends the distance value into the Serial Port
    Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    
    targetPos = targetFound(pos, distinCM); // if !0, target is found

    if (targetPos != 0 && found == false) { // if target is identified
      found = true;
      aimCrossbow(targetPos);               // aim crossbow
      delay(500);
      primeCrossbow();                      // prime crossbow
      delay(1000);
      fireCrossbow();                       // fire crossbow
      delay(500);
      
      primer.write(0);                      // reset servo wheels
      trigger.write(0);
    }
  }

  found = false;

  for (pos = 180; pos >= 0; pos--) {        // start servo rotation 180°-0°
    radar.write(pos);
    delay(30);
     
    distinCM = triggerRadar();              // triggers echo; returns distinCM
    
    Serial.print(pos); // Sends the current degree into the Serial Port
    Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distinCM); // Sends the distance value into the Serial Port
    Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    
    targetPos = targetFound(pos, distinCM); // if !0, target is found

    if (targetPos != 0 && found == false) { // if target is identified
      found = true;
      aimCrossbow(targetPos);               // aim crossbow
      delay(500);
      primeCrossbow();                      // prime crossbow
      delay(1000);
      fireCrossbow();                       // fire crossbow
      delay(500);
      
      primer.write(0);                      // reset servo wheels
      trigger.write(0);
    }
  }
}

// ***************************FUNCTIONS BELOW******************************

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
  if (distinCM < 50) { // ******* test this ********************************************************************
    return pos;
  }
  else
    return 0;
}

void aimCrossbow(int pos) {
  for (int i = 0; i <= pos; i++) {
    shaft.write(i);
    delay(10);
  }
  return;
}

void primeCrossbow(void) {
  for (int i = 0; i <= 180; i++) {
    primer.write(i);
    delay(10);
    
  /* // waay faster
   * primer.write(0);
   * delay(500);
   * primer.write(180);
   * delay(500);
   * primer.write(0);
   * delay(500);
   */
  }
   delay(250);
   primer.write(0);
   return;
}

void fireCrossbow(void) {
  // servo control for firing crossbow
  for (int i = 0; i <= 45; i++) {
    trigger.write(i);
  }
  /* // waay faster
  trigger.write(0);
  delay(250);
  trigger.write(180);
  delay(500);
  trigger.write(0);
  return;
  */
  delay(500);
  trigger.write(0);
  return;
}
