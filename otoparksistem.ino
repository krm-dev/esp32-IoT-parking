#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#include <HTTPClient.h>

const char* ssid = "Yourwifiadress";
const char* password = "wifipass";

static const int servoPin = 13;
const int trigPin = 18;
const int echoPin = 19;
int slot = 6;
LiquidCrystal_I2C lcd(0x27,16,2);
String serverNameslot = "https://api...";
String serverNamedistance = "https://api...";

#define SOUND_SPEED 0.034
Servo servo1;
long duration;
float distanceCm;

unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() {
    Serial.begin(115200);
    servo1.attach(servoPin);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    pinMode(27, OUTPUT);
    pinMode(26, OUTPUT);
    lcd.init();
    lcd.clear();         
    lcd.backlight();  
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
    lcd.print("Bos Park Alani");
    lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
    lcd.print(slot);
    WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverNameslot + "?kalanslot="+slot;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
}


void loop() {
    
    digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  if (distanceCm < 10 and slot != 0)
  {
    digitalWrite(27, HIGH); // turn the LED on
    digitalWrite(26, HIGH);
    servo1.write(180);
    delay(2000);
    lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
    slot--;
    lcd.print(slot);
     if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverNameslot + "?kalanslot="+slot;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  }
  else
  {
    digitalWrite(26, LOW);
    digitalWrite(27, LOW);
    servo1.write(0);
  }
  if (slot <= 0)
  {
    digitalWrite(27, HIGH); // turn the LED on
    lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
    lcd.print("Yok");
  }
  else
  {

  }
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
 // servo1.write(180);
  delay(1000);
}
