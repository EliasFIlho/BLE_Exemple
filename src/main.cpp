#include <Arduino.h>
#include <ArduinoBLE.h>
#include "Arduino_LSM9DS1.h"

BLEService AccService("180A");
BLEByteCharacteristic edgeCharacteristic("2A57", BLERead | BLEWrite);
BLEStringCharacteristic percentCharacteristc("2A56",BLERead,5);

void function(char LedC);

float x, y, z;
unsigned long previous_time = 0;
unsigned long base_time = 10;  


void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, LOW);         
  digitalWrite(LEDR, HIGH);               
  digitalWrite(LEDG, HIGH);               
  digitalWrite(LEDB, HIGH); 


  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy failed!");

    while (1);
  }
  BLE.setLocalName("Nano 33 BLE Sense");
  BLE.setAdvertisedService(AccService);
  AccService.addCharacteristic(edgeCharacteristic);
  AccService.addCharacteristic(percentCharacteristc);
  BLE.addService(AccService);
  edgeCharacteristic.writeValue(0);
  BLE.advertise();

}

void loop() {
  BLEDevice central = BLE.central();
  if(central){
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()){
      if(edgeCharacteristic.written()){
        switch (edgeCharacteristic.value()){
        case 01:
          function('R');
          break;
        case 02:
          function('G');
          break;
        case 03:
          function('B');
          break;
        default:
          break;
        }

      }
    }
  }
}


void function(char LedC){
  
  switch (LedC)
  {
  case 'R':
    Serial.println("Red LED on");
    percentCharacteristc.setValue("RLO");
    digitalWrite(LEDR, LOW);            
    digitalWrite(LEDG, HIGH);         
    digitalWrite(LEDB, HIGH);
    break;
  case 'G':
    Serial.println("Green LED on");
    percentCharacteristc.setValue("GLO");
    digitalWrite(LEDR, HIGH);            
    digitalWrite(LEDG, LOW);         
    digitalWrite(LEDB, HIGH);
    break;
  case 'B':
    Serial.println("Blue LED on");
    percentCharacteristc.setValue("RLO");
    digitalWrite(LEDR, HIGH);            
    digitalWrite(LEDG, HIGH);         
    digitalWrite(LEDB, LOW);
    break;
  default:
    Serial.println("Invalid value");
    percentCharacteristc.setValue("NA");
    break;
  }
  previous_time = millis();
  while(millis()-previous_time <= 4000){
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
    }
    Serial.println(String(x)+","+String(y)+","+String(z));
  }
  
}