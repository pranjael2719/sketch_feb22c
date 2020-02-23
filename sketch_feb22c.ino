#include <ESP8266WiFi.h>`
#include "Mac.h"

extern "C" {
  #include "user_interface.h"
}
//===== SETTINGS =====//
#define channel 1 //the channel it should scan on (1-14)
#define channelHopping true //scan on all channels
#define maxChannel 13 //US = 11, EU = 13, Japan = 14
#define ledPin 2 //led pin ( 2 = built-in LED)
#define inverted false // invert HIGH/LOW for the LED
#define packetRate 3 //min. packets before it gets recognized as an attack

#define scanTime 500 //scan time per channel in ms


//Mac from;
//Mac to;
unsigned long c = 0;
unsigned long deauth = 0;
unsigned long dissoc = 0;
unsigned long prevTime = 0;
unsigned long curTime = 0;
int curChannel = channel;

void sniffer(uint8_t *buf, uint16_t len) {
        if(buf[12] == 0xA0){
            dissoc = 1000;
          }
        if(buf[12] == 0xC0){
      /*Serial.print("From ");
      from._println();
      Serial.print("To ");
      to._println();
      Serial.println();*/
      
      deauth = 1000;
    }
    else{
      if (deauth >= 1){
      deauth--;}
      if (dissoc >= 1){
      dissoc--;}
    }
    
  //}
}

void setup() {
  Serial.begin(115200);

  wifi_set_opmode(STATION_MODE);
  wifi_promiscuous_enable(0);
  WiFi.disconnect();
  wifi_set_promiscuous_rx_cb(sniffer);
  wifi_set_channel(curChannel);
  wifi_promiscuous_enable(1);

  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
    
  
  Serial.println("starting!");

}

void loop() {
  curTime = millis();
  
  if(curTime - prevTime >= scanTime){
    prevTime = curTime;
    Serial.println((String)c);
    
   /* if(c >= 1){
      if(inverted) digitalWrite(D5, LOW);
      else digitalWrite(D5, HIGH);
      c--;}
      else{
      if(inverted) digitalWrite(D5, HIGH);
      else digitalWrite(D5, LOW); 
    } */
    if(deauth >= 1){
      if(inverted) digitalWrite(D5, LOW);
      else digitalWrite(D5, HIGH);}
      else{
      if(inverted) digitalWrite(D5, HIGH);
      else digitalWrite(D5, LOW);
    }
    if(dissoc >= 1){
      if(inverted) digitalWrite(D7, LOW);
      else digitalWrite(D7, HIGH);
      }
      else{
      if(inverted) digitalWrite(D7, HIGH);
      else digitalWrite(D7, LOW);
    }
    // c = 0;
    if(channelHopping){
      curChannel++;
      if(curChannel > maxChannel) curChannel = 1;
      wifi_set_channel(curChannel);
    }
  }

}
