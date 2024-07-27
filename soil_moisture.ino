#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT11.h>

#define wifi_ssid "Soi13"
#define wifi_password ""

#define mqtt_server "10.0.0.4"
#define mqtt_user "mqtt_user"
#define mqtt_password ""

//#define humidity_topic "homeassistant/sensor/humidity"
//#define temperature_topic "homeassistant/sensor/temperature"
#define soil_moisture_topic "homeassistant/sensor/soil_moisture"
#define soil_moisture_condition_topic "homeassistant/sensor/soil_moisture_condition"
#define soil_moisture_controller_topic "homeassistant/sensor/soil_moisture_state"
#define soil_moisture_topic1 "homeassistant/sensor/soil_moisture1"
#define soil_moisture_condition_topic1 "homeassistant/sensor/soil_moisture_condition1"
#define soil_moisture_controller_topic1 "homeassistant/sensor/soil_moisture_state1"
//#define soil_moisture_topic_flower_new "homeassistant/sensor/soil_moisture_flower_new"
//#define soil_moisture_condition_topic_flower_new "homeassistant/sensor/soil_moisture_condition_flower_new"
//#define soil_moisture_controller_topic_flower_new "homeassistant/sensor/soil_moisture_state_flower_new"

#define sensorPower1 14 //D5 on ESP8266 board
#define sensorPower 5 //D1 on ESP8266 board
#define sensorPin A0

//#define DHTPIN  4 //D2 on ESP8266 board
//DHT11 dht11(DHTPIN);

#define MUX_A 12 //D6 on ESP8266 board
#define MUX_B 13 //D7 on ESP8266 board
#define MUX_C 15 //D8 on ESP8266 board

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
//float temp = 0.0;
//float hum = 0.0;
float diff = 1.0;
float diff1 = 1.0;
int soil_moisture = 0;
String level_moisture = "";
int soil_moisture1 = 0;
String level_moisture1 = "";

void changeMux(int c, int b, int a) {
  digitalWrite(MUX_A, a);
  digitalWrite(MUX_B, b);
  digitalWrite(MUX_C, c);
}

void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  //Soil moisture
  pinMode(sensorPower, OUTPUT);
  pinMode(sensorPower1, OUTPUT); 
  digitalWrite(sensorPower, LOW);
  digitalWrite(sensorPower1, LOW);

  //Deifne output pins for Mux
  pinMode(MUX_A, OUTPUT);
  pinMode(MUX_B, OUTPUT);     
  pinMode(MUX_C, OUTPUT);  
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//Humidity and temperature
/*bool checkBound(float newValue, float prevValue, float maxDiff) {
  return !isnan(newValue) && (newValue < prevValue - maxDiff || newValue > prevValue + maxDiff);
}*/

//Soil moisture
bool checkBoundSoil(float newVal, float prevVal, float maxDiff) {
  return !isnan(newVal) && (newVal < prevVal - maxDiff || newVal > prevVal + maxDiff);
}

// This function returns the analog soil moisture measurement on Rose flower
int readSensor() {
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10); // Allow power to settle
  changeMux(LOW, HIGH, HIGH); //Change multiplexer
  int val = analogRead(sensorPin); //Value of the sensor connected Option 0 pin of Mux
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}

// This function returns the analog soil moisture measurement on New flower
int readSensor1() {
  digitalWrite(sensorPower1, HIGH);  // Turn the sensor ON
  delay(10); // Allow power to settle
  changeMux(LOW, LOW, HIGH); //Change multiplexer
  int val = analogRead(sensorPin); //Value of the sensor connected Option 0 pin of Mux
  digitalWrite(sensorPower1, LOW);   // Turn the sensor OFF
  return val;             // Return analog moisture value
}


void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //int chk = dht11(DHTPIN);

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;

    //////////Send state of controller
    client.publish(soil_moisture_controller_topic, "1");
    client.publish(soil_moisture_controller_topic1, "1");

    /*//////////Temperature and humidity sensor
    float newTemp = (float)dht11.readTemperature();
    float newHum = (float)dht11.readHumidity();


    if (checkBound(newTemp, temp, diff)) {
      temp = newTemp;
      Serial.print("New temperature:");
      Serial.println(String(temp).c_str());
      client.publish(temperature_topic, String(temp).c_str(), true);
    }

    if (checkBound(newHum, hum, diff)) {
      hum = newHum;
      Serial.print("New humidity:");
      Serial.println(String(hum).c_str());
      client.publish(humidity_topic, String(hum).c_str(), true);
    }
    ////////////////////////////////////////////////////////////////*/


    ///////////Soil moisture sensors
    int new_soil_moisture = readSensor();
    String tempMoisture = level_moisture;

    int new_soil_moisture1 = readSensor1();
    String tempMoisture1 = level_moisture1;

    //Checking for Rose flower
    if (checkBoundSoil(new_soil_moisture, soil_moisture, diff)) {
      soil_moisture = new_soil_moisture;
      Serial.print("New soil moisture Rose flower:");
      if (soil_moisture < 400) {
        level_moisture = "Soil is extremelly wet!";        
      }
      if (soil_moisture >= 400 && soil_moisture <= 600) {
        level_moisture = "Soil is wet!";       
      }
      if (soil_moisture >= 601 && soil_moisture <= 850) {
        level_moisture = "Soil moisture is perfect!";       
      }
      if (soil_moisture >= 851 && soil_moisture <= 970) {
        level_moisture = "Soil is dry!";
      }
      if (soil_moisture >970) {
        level_moisture = "Soil is extremelly dry!";
      }

      if (tempMoisture != level_moisture) {
        Serial.println(level_moisture);
        client.publish(soil_moisture_condition_topic, level_moisture.c_str());
      }
          
      Serial.println(String(soil_moisture).c_str());
      client.publish(soil_moisture_topic, String(soil_moisture).c_str(), true);
    }

    
    //Checking for new flower
    if (checkBoundSoil(new_soil_moisture1, soil_moisture1, diff1)) {
      soil_moisture1 = new_soil_moisture1;
      Serial.print("New soil moisture New flower:");
      if (soil_moisture1 < 400) {
        level_moisture1 = "Soil is extremelly wet!";        
      }
      if (soil_moisture1 >= 400 && soil_moisture1 <= 600) {
        level_moisture1 = "Soil is wet!";       
      }
      if (soil_moisture1 >= 601 && soil_moisture1 <= 850) {
        level_moisture1 = "Soil moisture is perfect!";       
      }
      if (soil_moisture1 >= 851 && soil_moisture1 <= 970) {
        level_moisture1 = "Soil is dry!";
      }
      if (soil_moisture1 >970) {
        level_moisture1 = "Soil is extremelly dry!";
      }

      if (tempMoisture1 != level_moisture1) {
        Serial.println(level_moisture1);
        client.publish(soil_moisture_condition_topic1, level_moisture1.c_str());
      }
          
      Serial.println(String(soil_moisture1).c_str());
      client.publish(soil_moisture_topic1, String(soil_moisture1).c_str(), true);
    }
    
  }
  
}
