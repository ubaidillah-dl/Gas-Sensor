#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Menentukan wifi *********************************/
#define WLAN_SSID       "Warnet Putri"        // masukkan nama wifi
#define WLAN_PASS       "poiuytrewq"          // masukkan password

/************************* Mengatur adafruit.io *********************************/
#define MQTT_SERVER      "192.168.100.117"    // ip address raspberrypi
#define MQTT_SERVERPORT  1883                 // port raspberrypi
#define MQTT_USERNAME    ""                   // dikosongkan
#define MQTT_KEY         ""                   // dikosongkan

/************************* Definisi pin sensor *********************************/
#define gas A0                             // pin ldr
short data,threshold=400;

/************************* Deklarasi wifi server *********************************/
WiFiClient client;
//WiFiClientSecure client;                    // untuk koneksi lebih aman

/************************* Mengatur mqtt client *********************************/
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_KEY);

/************************* Publish data *********************************/
Adafruit_MQTT_Publish gas_stream = Adafruit_MQTT_Publish(&mqtt, MQTT_USERNAME "sensor_gas","hello");

/*************************** Program ************************************/
void setup() {
  Serial.begin(115200);
  delay(10);
  
  /*************************** Menentukan mode pin ************************************/
  pinMode(gas,INPUT);
  
  /*************************** Menghubungkan ke wifi ************************************/
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" WiFi connected");
  Serial.print("IP Address : "); 
  Serial.println(WiFi.localIP());
}

void loop() {
  MQTT_connect();

  short data=analogRead(gas);

  Serial.print("sensor_gas ");
  Serial.println(data);

  /*************************** Mengirim data ke mqtt ************************************/
  gas_stream.publish(data);
  delay(1000);
}

void MQTT_connect(){
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries=3;
  while ((ret=mqtt.connect())!=0){
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries==0) {
         while (1);
       }
    }
  Serial.println("MQTT Connected!");
}
