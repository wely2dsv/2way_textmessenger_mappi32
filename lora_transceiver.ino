//Lora Transceiver with Mappi32
//Code by YC2DSV

#include <math.h>
#include <SPI.h>
#include <LoRa.h>

float rssi;
float pwr;
int percent;

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27
#define TxRxLed 5


//+++++++++++++++++++++
//START OF Initializing
void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  //initialize TX LED
  pinMode(TxRxLed, OUTPUT);
  
  while (!Serial);
  Serial.println("LoRa Transceiver");

  //setup LoRa transceiver module
  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);
  
  while (!LoRa.begin(921E6)) {
    Serial.println(".");
    delay(500);
  }

  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Transceiver Initializing : OK");
  Serial.println("-----");
}
//END OF Initializing
//+++++++++++++++++++++


//+++++++++++++++++++++
//START OF Main Program 
void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      LoRaData.trim();
      Serial.print(LoRaData);
      Serial.print("<<");
      getRssiData();   
      blinkTxRxLed(100);  
    }       
  }

  if (Serial.available()) {
    LoRa.beginPacket();   
    LoRa.print(Serial.readString()); 
    LoRa.endPacket();
    blinkTxRxLed(100);
    //Serial.println("Data sent...");
  }  
}
//END OF Main Program 
//+++++++++++++++++++++


//+++++++++++++++++++++
//START OF Subroutine & Function 
  void blinkTxRxLed(int t){
    digitalWrite(TxRxLed, HIGH);
    delay(t);
    digitalWrite(TxRxLed, LOW);  
  }

  void getRssiData() {
    rssi = LoRa.packetRssi();
    
    // print RSSI of packet
    Serial.print("RSSI:");
    Serial.println(rssi);
  }

//END OF Subroutine & Function 
//+++++++++++++++++++++
