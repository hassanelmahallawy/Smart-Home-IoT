#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <Keypad.h>
#include <ESP32Servo.h>
#include <LiquidCrystal.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <ctime>

using namespace std;

// WiFi credentials
#define WIFI_SSID "Qutraz"
#define WIFI_PASSWORD "BeastlyX"

// Firebase credentials
#define API_KEY ""
#define USER_EMAIL ""
#define USER_PASSWORD "124578"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL ""

// Initialize Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
FirebaseJson json;

String ir_path = "Smart_Home/Intruder";                
String door_path = "Smart_Home/Actuators/door";                      
String window_path = "Smart_Home/Actuators/Window";         
String flame_path = "Smart_Home/Sensors/Flame";                                 
String ldr_path = "Smart_Home/Sensors/LDR";                                       
String doorTime_path = "Smart_Home/Actuators/Latest_Entrance"

const int buzzerPin = 32;
const int LDR_PIN = 35;
const int flameSensorPin = 39;
const int led_PIN1 = 25;
const int led_PIN2 = 26;
const int touchSensorPin = 36;

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {21, 19, 18, 5};
byte colPins[COLS] = {12, 13, 14, 15};
Keypad customKeypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Servo configuration
Servo door;
Servo window;

// LCD configuration
LiquidCrystal lcd(22,23, 17, 16, 2, 4);
//_____________________________________________________________________________________Setup________________________________________________________________________________
void setup()
{
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  pinMode(buzzerPin, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.begin(&config, &auth);
  if(Firebase.ready()){
    Serial.println("Ready");
  }else{
    Serial.print("failed");
  }

  int irReading = analogRead(34);
  door.attach(27);
  window.attach(33);
  door.write(25);
  window.write(7);
  lcd.begin(16, 2);

  while (irReading > 3000)
  {
    lcd.setCursor(0, 0);
    lcd.print("Smart Home");
    irReading = analogRead(34);
  }

  // Authenticate using keypad
  int crack = 0;
  bool flag = false;
  while (!flag)
  {
    Firebase.RTDB.setString(&fbdo, ir_path, "Intruder detected");
    if (crack == 3)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Limit Exceeded");
      Firebase.RTDB.setString(&fbdo, ir_path,"Crack Attempted");
      tone(buzzerPin, 50, 3000);
      lcd.clear();
      lcd.print("SYSTEM LOCKED.");
      while(1){}
    }
    else
    {

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter password:");

      char enteredPassword[7];
      int i = 0;
      while (i < 6)
      {
        char key = customKeypad.waitForKey();
        enteredPassword[i] = key;
        lcd.setCursor(i, 1);
        lcd.print("X");
        Serial.print("X");
        delay(200);
        i++;
      }
      enteredPassword[6] = '\0';
      Serial.println();
      if (auth.user.password == enteredPassword)
      {
        flag = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Granted");
        for (int pos=25; pos <= 110; pos += 1) { // goes from 0 degrees to 110 degrees
        // in steps of 1 degree
          door.write(pos); // tell servo to go to position in variable 'pos'
          delay(7); // waits 15ms for the servo to reach the position
        };
        std::time_t currentTime = std::time(nullptr);
        Firebase.RTDB.setString(&fbdo, door_path, "Opened");
        Firebase.RTDB.setInt(&fbdo, doorTime_path, currentTime);
        Firebase.RTDB.setString(&fbdo, ir_path, "Owner/Guest Welcomed");
        delay(4000);
        for (int pos=110; pos >= 25; pos -= 1) { // goes from 110 degrees to 0 degrees
          door.write(pos); // tell servo to go to position in variable 'pos'
          delay(7); // waits 15ms for the servo to reach the position
        }
        Firebase.RTDB.setString(&fbdo, door_path, "Closed");
      }
      else
      {
        crack++;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Access Denied");
        lcd.setCursor(0, 1);
        lcd.print("Re-Enter Pass");
        delay(1000);
      }
    }
    pinMode(flameSensorPin, INPUT);
    pinMode(touchSensorPin, INPUT);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome Home");
  delay(2000);
}

//_______________________________________________________________________________Loop________________________________________________________________________________
void loop()
{
  pinMode(led_PIN1, OUTPUT);
  pinMode(led_PIN2, OUTPUT);
  int flameSensorReading = digitalRead(flameSensorPin);
  int touchSensorReading = digitalRead(touchSensorPin);
  int ldrReading = analogRead(LDR_PIN);

  if (flameSensorReading == HIGH)
  {
    // If Flame detected
    window.write(160);
    door.write(90);
    //lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WARNING!!!");
    tone(buzzerPin, 1000);
    Firebase.RTDB.setString(&fbdo, flame_path, "FIRE ALERT!!!");
    Firebase.RTDB.setString(&fbdo, window_path, "Opened");
    Firebase.RTDB.setString(&fbdo, door_path, "Opened");
    digitalWrite(led_PIN2, HIGH); // Turn on the light
    delay(50);
    digitalWrite(led_PIN2, LOW);
    delay(50);
    lcd.clear();
  }
  else
  {
    noTone(buzzerPin);
    Firebase.RTDB.setString(&fbdo, flame_path, "Safe");
  }
  if (touchSensorReading == HIGH)
  {
    // If Touch sensor is touched
    if (window.read() > 7)
    {
      window.write(7); // Close the window fully
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Window Closed");
      Firebase.RTDB.setString(&fbdo, window_path, "Closed");
      delay(1000);
      lcd.clear();
      lcd.print("Welcome Home");
    }
    else
    {
      window.write(160); // Open the window
      //lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Window Opened");
      Firebase.RTDB.setString(&fbdo, window_path, "Opened");
      delay(1000);
      lcd.clear();
      lcd.print("Welcome Home");
    }
    delay(2500); // Delay to prevent rapid servo movements
  }
  int ldrScaled = map(ldrReading, 0, 4095, 0, 600);
  if (ldrScaled < 150)
  {
    Firebase.RTDB.setString(&fbdo, ldr_path, "Night/Cloudy");
    digitalWrite(led_PIN1, HIGH); // Turn on the LED
  }
  else
  {
    Firebase.RTDB.setString(&fbdo, ldr_path, "Sunny");
    digitalWrite(led_PIN1, LOW); // Turn off the LED
  }

  if (Firebase.RTDB.getString(&fbdo, door_path) && fbdo.stringData() == "Opened"){
    door.write(90);
  }
  if (Firebase.RTDB.getString(&fbdo, window_path) && fbdo.stringData() == "Opened"){
    window.write(160);
  }
  if (Firebase.RTDB.getString(&fbdo, door_path) && fbdo.stringData() == "Closed"){
    door.write(25);
  }
  if (Firebase.RTDB.getString(&fbdo, window_path) && fbdo.stringData() == "Closed"){
    window.write(7);
  }
}
