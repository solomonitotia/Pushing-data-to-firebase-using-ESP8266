
#include <ESP8266WiFi.h>                                                    // esp8266 library
#include <FirebaseArduino.h>                                                // firebase library
#include <DHT.h>                                                            // dht11 temperature and humidity sensor library
#define FIREBASE_HOST "smart-green-eceb7.firebaseio.com"                          // the project name address from firebase id
#define FIREBASE_AUTH "hhTrHHEpk8kWwyYuIVnas1bAWW0SVc8Pk2xoDbTH"            // the secret key generated from firebase
#define WIFI_SSID "IoT Research Lab"                                             // input your home or public wifi name 
#define WIFI_PASSWORD "IoTlab#2019"                                    //password of wifi ssid
#define DHTPIN D4                                                           // what digital pin we're connected to
#define DHTTYPE DHT11                                                       // select dht type as DHT 11 or DHT22
DHT dht(DHTPIN, DHTTYPE); 
int value = 0;
int sensorPin = A0; // Soil Sensor input at Analog PIN A0
int output_value ;                                                    
void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  delay(1000);                
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                            //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                              // connect to firebase
  dht.begin();                                                               //Start reading dht sensor
}
void loop() { 
  float h = dht.readHumidity();                                              // Reading temperature or humidity takes about 250 milliseconds!
  float t = dht.readTemperature();                                           // Read temperature as Celsius (the default)
  float m = analogRead(sensorPin);
  Serial.print(m);
  // read the input of digital pin gpio 5
  if (isnan(h) || isnan(t) ||isnan(m)) {                                                // Check if any reads failed and exit early (to try again).
    Serial.println(F("Check sensors whether they are working!"));
    return;
  }
  Serial.print("Humidity: ");  Serial.print(h); Serial.println();
  String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("Temperature: ");  Serial.print(t);Serial.println();
  String fireTemp = String(t) + String("°C");                                                     //convert integer temperature to string temperature
  Serial.print("Soil Moisture:"); Serial.print(m);Serial.println();
  String fireMoisture = String(m*1/10) + String("%");
  delay(10000);
  
  Firebase.pushString("/Smart_Green_Data/Humidity", fireHumid);                                  //setup path and send readings
  Firebase.pushString("/Smart_Green_Data/Temperature", fireTemp);  
  Firebase.pushString("/Smart_Green_Data/Soil_Moisture", fireMoisture);//setup path and send readings
   
}
