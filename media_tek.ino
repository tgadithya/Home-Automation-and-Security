/*
 * THIS IS THE CODE FOR HOME AUTOMATION AND SECURITY PROJECT   
 * BY USING MEDIATEK LINKIT SMART 7688 DUO

 PROJECT BY- 
 ADITHYA TG
 */

#include <Bridge.h>
#include <HttpClient.h>
#include<String.h>
#include<dht.h>
//-----------------------------------------
//    pin definations
//-----------------------------------------
#define DHT11_PIN 6
#define Door_closed A0
#define Light 2
#define heater 3
#define Fan 4
#define Alarm 5
//------------------------------------------
//       variables
//------------------------------------------
dht DHT;
char c,e;                       // stores the bytes coming from the server

float temp,h;                   //stores float value of temp and humididty
String humidity,temperature;    //stores string value of temp and humididty
int co=0;                       //control the sending of fire Alert

int x=0,y=0,check=0;            //control the sending of intruder Alert
int p=0,q=0,r=0,s=0,t=0;        //these variables prevent switching ON/OFF of already turned ON/OFF devices

 //============================================================================================================
 //                                   Start of Program
 //============================================================================================================

void setup() 
{
   //setting all the required pins as OUTPUT/INPUT
  pinMode(Light,OUTPUT);                              
  pinMode(heater,OUTPUT);
  pinMode(Fan,OUTPUT);
  pinMode(Alarm,OUTPUT);
  pinMode(Door_closed,INPUT);
  //making all the required pins as HIGH
  // the four pins are set HIGH because it is given as the input to the relays
  // relays work when the inputs become low
  digitalWrite(Light,HIGH);
  digitalWrite(heater,LOW);
  digitalWrite(Fan,HIGH);
  digitalWrite(Alarm,LOW);
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board Light
  // as an indicator for when it has initialized
  delay(2000);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.println("initializing Pls wait...");
  Bridge.begin();
  digitalWrite(13,HIGH);
  delay(1000); 
  Serial.println("Complete");
  // pin 13 inbuilt Light turns ON after completion of initial setup
}

void loop() 
{
  // Initialize the client library
  HttpClient client;
  digitalWrite(13,LOW);
  delay(1000);  
 //============================================================================================================
 //                                        Mini Weather Station
 //============================================================================================================
  int chk = DHT.read11(DHT11_PIN);
  h=DHT.humidity;
  temp=DHT.temperature;
  if(temp<50.0)
  {
    co=0;
  }
  if(temp>=50.0&&co==0)
  {
    Serial.println("Sending Alert please wait.......");
    delay(1000);
    client.get("http://api.pushingbox.com/pushingbox?devid=v04F9*******D6E3");        //sending http request to pushingBox for executing scenarios using device ID
    co=1;
    Serial.println(co);
  }
  humidity=String(h);
  temperature=String(temp); 
  Serial.println(h);
  Serial.println(temp);   
  client.get("https://api.thingspeak.com/update?key=K2RKE*******RYIF&field1="+humidity);             //update humidity
  client.get("https://api.thingspeak.com/update?key=IT7CC*******0UEH&field1="+temperature);          //update temperature

//============================================================================================================
//                                          Home Security
//============================================================================================================

  x=analogRead(Door_closed);
  Serial.println(x);
  if(x>550)
  {
    y=0;
  }
  if(x<=550&&y==0)
  {
    Serial.println("Sending Alert Pls Wait.....");
    delay(1000);
    client.get("http://api.pushingbox.com/pushingbox?devid=v2548*******9057");        //sending http request to pushingBox for executing scenarios using device ID
    delay(1000);
    y=1;
  }
 //============================================================================================================
 //                                        Device Control
 //============================================================================================================
 // Make a HTTP request to get value of channel from thingspeak.com:
    client.get("https://api.thingspeak.com/channels/84312/field/field1/last.txt");    //http request for public channel
                                                                                      //for private channel use read key
    
    // if there are incoming bytes available
    // from the server, read them and print them:
   
    while (client.available())
    {
       c = client.read();
       Serial.print(c); 
    }
    e=c;     // stores the value of the field in the channel
    
    // Performing required operations based on the value of channel
    //-------------------------------------------------------------
    //                        LIGHT
    //-------------------------------------------------------------
    
    if(e=='0'&&p==0)         //Light ON            
    {
      digitalWrite(Light,LOW);
      // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
      // this channel is read by the app and the status of devices in the app gets updated accordingly
      client.get("https://api.thingspeak.com/update?key=F3B4*******FQUD3&field1=0");                   //updating with channel key
      p=1;
    }
     if(e=='5'&&p==1)        //Light off
    {
      digitalWrite(Light,HIGH);
      // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
      // this channel is read by the app and the status of devices in the app gets updated accordingly
      client.get("https://api.thingspeak.com/update?key=F3B4*******FQUD3&field1=1");
      p=0;
    }
    
    //-------------------------------------------------------------
    //                        Heater
    //-------------------------------------------------------------
   
    if(e=='1'&&q==0)
    {
      digitalWrite(heater,HIGH);
      // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
      // this channel is read by the app and the status of devices in the app gets updated accordingly
      client.get("https://api.thingspeak.com/update?key=TJIWC*******65LE&field1=0");
      q=1;
    }
    if(e=='6'&&q==1)
    {
      digitalWrite(heater,LOW);
      // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
      // this channel is read by the app and the status of devices in the app gets updated accordingly
      client.get("https://api.thingspeak.com/update?key=TJIWC*******65LE&field1=1");
      q=0;
    }
    
    //-------------------------------------------------------------
    //                        Fan
    //-------------------------------------------------------------
    
    if(e=='2'&&r==0)
    {
      digitalWrite(Fan,LOW);
      // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
      // this channel is read by the app and the status of devices in the app gets updated accordingly
      client.get("https://api.thingspeak.com/update?key=OF7V1*******PHOP&field1=0");
      r=1;
    }
     if(e=='7'&&r==1)
    {
      digitalWrite(Fan,HIGH);
      // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
      // this channel is read by the app and the status of devices in the app gets updated accordingly
      client.get("https://api.thingspeak.com/update?key=OF7V1*******PHOP&field1=1");  
      r=0;
     }
  
  //-------------------------------------------------------------
  //                        Alarm
  //-------------------------------------------------------------
    
    if(e=='3'&&s==0)
    {
      digitalWrite(Alarm,HIGH);
      // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
      // this channel is read by the app and the status of devices in the app gets updated accordingly
      client.get("https://api.thingspeak.com/update?key=MELB********16HE&field1=0");
      s=1;
     }
     if(e=='8'&&s==1)
     {
       digitalWrite(5,LOW);
       // Make a HTTP  POST request to update a  value of a field in the channel in thingspeak.com:
       // this channel is read by the app and the status of devices in the app gets updated accordingly
       client.get("https://api.thingspeak.com/update?key=MELB********16HE&field1=1"); 
       s=0;
       }

}
 //============================================================================================================
 //                                      end of program
 //============================================================================================================

