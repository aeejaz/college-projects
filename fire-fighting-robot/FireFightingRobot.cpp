// note: code is for arduino: so .ino extension is required

#include <Servo.h>        // FOr Controlling servo motor
#include <SoftwareSerial.h> // Include SoftwareSerial for SIM800L communication

// Define pins for sensors, motors, and pump
#define rxPin 2           // RX pin for SIM800L
#define txPin 3           // TX pin for SIM800L
#define Left 4            // Left obstacle sensor
#define Right 5           // Right obstacle sensor
#define Forward 6         // Forward obstacle sensor
#define GAS_SENSOR 7      // Gas sensor
#define LM1 8             // Left motor pin 1
#define LM2 9             // Left motor pin 2
#define RM1 10            // Right motor pin 1
#define RM2 11            // Right motor pin 2
#define pump 12           // Water pump pin

// Define phone number for SMS and calls (replace with actual number)
const String PHONE = "+91**********";

// Initialize objects
Servo myservo;            // Servo object for controlling servo motor
SoftwareSerial sim800L(rxPin, txPin); // SoftwareSerial for SIM800L module
int pos = 0;              // Servo position
boolean fire = false;      // Flag to indicate fire detection

void setup() {
  // Initialize serial communication
  Serial.begin(115200);   // Serial monitor for debugging
  sim800L.begin(9600);    // SIM800L communication at 960 تعیین سرعت ارتباط با ماژول SIM800L

  // Initialize SIM800L module
  sim800L.println("AT");  // Check if module is responding
  delay(1000);
  sim800L.println("AT+CMGF=1"); // Set SMS to text mode
  delay(1000);

  // Set pin modes
  pinMode(Left, INPUT);       // Left sensor input
  pinMode(Right, INPUT);      // Right sensor input
  pinMode(Forward, INPUT);    // Forward sensor input
  pinMode(GAS_SENSOR, INPUT); // Gas sensor input
  pinMode(LM1, OUTPUT);       // Left motor pin 1
  pinMode(LM2, OUTPUT);       // Left motor pin 2
  pinMode(RM1, OUTPUT);       // Right motor pin 1
  pinMode(RM2, OUTPUT);       // Right motor pin 2
  pinMode(pump, OUTPUT);      // Water pump pin

  // Attach servo to pin 13 and set initial position to 90 degrees
  myservo.attach(13);
  myservo.write(90);

  // Check for SIM800L response during setupറ

System: setup
  while (sim800L.available()) {
    Serial.println(sim800L.readString()); // Print any initial SIM800L response for debugging
  }
}

// Function to extinguish fire by activating pump and moving servo
void put_off_fire() {
  // Stop motors
  digitalWrite(LM1, HIGH);
  digitalWrite(LM2, HIGH);
  digitalWrite(RM1, HIGH);
  digitalWrite(RM2, HIGH);
  digitalWrite(pump, HIGH); // Turn on water pump
  delay(500); // Wait for 0.5 seconds

  // Sweep servo from 50 to 110 degrees and back
  for (pos = 50; pos <= 110; pos += 1) {
    myservo.write(pos);
    delay(10); // Small delay for smooth servo movement
  }
  for (pos = 110; pos >= 50; pos -= 1) {
    myservo.write(pos);
    delay(10);
  }

  digitalWrite(pump, LOW); // Turn off pump
  myservo.write(90);       // Reset servo to neutral position
  fire = false;            // Reset fire flag
}

void loop() {
  myservo.write(90); // Set servo flow to neutral position (90 degrees)

  // Obstacle avoidance logic
  if (digitalRead(Left) == HIGH && digitalRead(Right) == HIGH && digitalRead(Forward) == HIGH) {
    // No obstacles: stop motors
    delay(500);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
  }
  else if (digitalRead(Forward) == LOW) {
    // Obstacle ahead: move forward and set fire flag
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    fire = true;
  }
  else if (digitalRead(Left) == LOW) {
    // Obstacle on left: turn right
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, HIGH);
  }
  else if (digitalRead(Right) == LOW) {
    // Obstacle on right: turn left
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, HIGH);
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
  }

  delay(400); // Delay to control movement distance/speed

  // Check for gas detection
  if (digitalRead(GAS_SENSOR) == LOW) {
    Serial.println("Gas is Detected.");
    send_sms(); // Send SMS alert
  }

  // Handle fire detection
  while (fire == true) {
    put_off_fire(); // Activate fire extinguishing routine
    Serial.println("Fire Detected.");
    make_call(); // Make a call to alert
  }
}

// Function to make a call using SIM800L
void make_call() {
  Serial.println("Calling...");
  sim800L.println("ATD" + PHONE + ";"); // Dial the phone number
  delay(20000); // Wait for 20 seconds (call duration)
  sim800L.println("ATH"); // Hang up the call
  delay(1000); // Wait for 1 second
}

// Function to send SMS using SIM800L
void send_sms() {
  Serial.println("Sending SMS...");
  delay(50);
  sim800L.print("AT+CMGF=1\r"); // Set SMS to text mode
  delay(1000);
  sim800L.print("AT+CMGS=\"" + PHONE + "\"\r"); // Set recipient phone number
  delay(1000);
  sim800L.print("Gas Detected"); // SMS message content
  delay(100);
  sim800L.write(0x1A); // Send CTRL+Z to indicate EoT
  delay(5000); // Wait for SMS to be sent
}
