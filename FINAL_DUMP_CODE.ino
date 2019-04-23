//Air Pollution Monitoring using NodeMCU
#include "MQ135.h"
#include <DHT.h>  
#include <ESP8266WiFi.h>  
#include <WiFiClient.h>  
#include <ThingSpeak.h>  


const char* myWriteAPIKey = "B716BUY5ZCL89X7X";//Channel API wirte key
const char* ssid = "D-Link";  //MY WIFI NETWORK  
const char* password = "gandlasaiteja"; //MY WIFI PASSWORD

#define DHTPIN D5  
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);  

MQ135 gasSensor = MQ135(A0); 

WiFiClient client;  

unsigned long myChannelNumber = 646497;  //Channel ID
float temperature, humidity;

void setup()  
  {  
    Serial.begin(115200);  
    dht.begin();  
    delay(10);  
    // Connect to WiFi network  
    Serial.println();  
    Serial.println();  
    Serial.print("Connecting to ");  
    Serial.println(ssid);  
    WiFi.begin(ssid, password);  
    while (WiFi.status() != WL_CONNECTED)  
        {  
          delay(500);  
          Serial.print(".");  
        }  
    Serial.println("");  
    Serial.println("WiFi connected");  
    // Print the IP address  
    Serial.println(WiFi.localIP());  
    ThingSpeak.begin(client);  
  }  

void loop()   
 {  
    static boolean data_state = false;  
    static boolean a = false; 
    temperature = dht.readTemperature();  
    humidity = dht.readHumidity();  
    float ppm = gasSensor.getPPM();
    // Check if any reads failed and exit early (to try again).    
    if (isnan(humidity) || isnan(temperature)) 
    {
    return;
    }
    Serial.print("Temperature Value is :");  
    Serial.print(temperature);  
    Serial.println("C");  
    Serial.print("Humidity Value is :");  
    Serial.print(humidity);  
    Serial.println("%");  
    Serial.print("Quality of Smoke in ppm: ");
    Serial.print(ppm);
    Serial.println("ppm");
    if( a||data_state )  
        {  
          if(a)
         {
          ThingSpeak.writeField(myChannelNumber, 2, humidity, myWriteAPIKey);    
          a=false;
          }
          else
          {
         ThingSpeak.writeField(myChannelNumber, 1, temperature, myWriteAPIKey); 
         data_state = false;
         a=true; 
          }
          }  
          else  
        {
            
        ThingSpeak.writeField(myChannelNumber, 3, ppm, myWriteAPIKey);  
           
         data_state = true; 
         
         }  
    delay(30000); // ThingSpeak will only accept updates every 15 seconds.  
 }  
 //BY G.SAI TEJA
