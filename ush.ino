#include <ESP8266WiFi.h> //esp8266 header
#include <SoftwareSerial.h>
#include<MFRC522.h>
#include<SPI.h>
//adafruit header file
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

//connected details
#define WLAN_SSID "uday"
#define WLAN_PASS "uday1998"
#define AIO_SERVERPORT 1883

//account details
#define AIO_USERNAME "Uday_M"
#define AIO_KEY "df6a8b0f3a7b4df1836c6b413f8f50fa"
#define AIO_SERVER "io.adafruit.com"
#define c1 16  //d0
#define c2 5  //d1
#define SS_PIN 4  //D2
#define RST_PIN 5 //D1

MFRC522 mfrc522(SS_PIN, RST_PIN);
int statuss = 0;
int out = 0;
int x;

WiFiClient client;
const char MQTT_SERVER[] PROGMEM = AIO_SERVER;
const char MQTT_CLIENT[] PROGMEM = AIO_KEY __DATE__ __TIME__;
const char MQTT_PAAWORD[] PROGMEM = AIO_KEY;
const char MQTT_USERNAME[] PROGMEM = AIO_USERNAME;


Adafruit_MQTT_Client mqtt(&client,AIO_SERVER,AIO_SERVERPORT,AIO_USERNAME,AIO_KEY);

Adafruit_MQTT_Publish id1=Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/id1");
Adafruit_MQTT_Publish ld1=Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/ld1");
Adafruit_MQTT_Publish id2=Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/id2");
Adafruit_MQTT_Publish l1=Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/l1");
Adafruit_MQTT_Publish l2=Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME"/feeds/l2");

SoftwareSerial ESPserial(13,15);
int y=0,z=0;
char data[4];
void setup() {
pinMode(c1, OUTPUT);
pinMode(c2, OUTPUT);
Serial.begin(115200);
Serial.begin(9600);
SPI.begin();
mfrc522.PCD_Init();   
delay(10);
Serial.println(F("Adafruit MQTT demo"));
//F for flash memory based serial printing.
//connect to WiFi access point
Serial.println();
Serial.println();
Serial.print("CONNECTING TO \t");
Serial.println(WLAN_SSID);

//connect to WiFi
WiFi.begin(WLAN_SSID,WLAN_PASS);
//Print progress while WiFi is connecting
while(WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");

//print the IP address
Serial.print("The device IP address is: ");
Serial.println(WiFi.localIP()); 
//setup MQTT subscription for onoff feed.


}
  
void loop() 
{
//ensure the connection to the mqtt server is alive
//connection and automaticallyreconnect when disconnected
//function definition further below
MQTT_connect();

// Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
   Serial.println();
  if (content.substring(1) == "B0 6A 0D A8") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println();
    statuss = 1;
    if (y==0)
    {
      Serial.println("Id1 in");
      x=5;
    id1.publish(x);
    ld1.publish(x);
    digitalWrite(c1, HIGH);
    y=1;
    }
    else if (y==1)
    {
      Serial.println("Id1 out");
     x=0;
    id1.publish(x);
    ld1.publish(x);
    digitalWrite(c1, HIGH);
    y=0;
    }
  }
  
  else if (content.substring(1) == "8E 39 32 50") //change UID of the card that you want to give access
  {
    Serial.println(" Access Granted ");
    Serial.println();
    statuss = 1;
    id2.publish(z);
    digitalWrite(c1, HIGH);
  }
else
{
  Serial.println();
  Serial.println("Unathorised access");
  digitalWrite(c2,HIGH);
}

if(!mqtt.ping()){
  mqtt.disconnect();
  }
}
//function to connect and reconnect as necesary to the mqtt server
//should be ccalledd in the loop function and it will take care if connecting

void MQTT_connect()
{
  int8_t ret;
  //stop if already connected.
  if(mqtt.connected())
  {
    return;
  }
  Serial.print("CONNECTING to MQTT...");
  while ((ret=mqtt.connect())!=0)
  {
    //connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retying MQTT connection in 5 seconds");
    mqtt.disconnect();
    delay(5000); //wait for 5 seconds
  }
  Serial.println("MQTT CONNECTED");
}
