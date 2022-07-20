#include <Arduino.h>
#include <ArduinoBLE.h>

BLEService AccService("180A");
BLEByteCharacteristic edgeCharacteristic("2A57", BLERead | BLEWrite);
BLEStringCharacteristic percentCharacteristc("2A56",BLERead,5);

void function01();
void function02();
void function03();

void setup() {
  Serial.begin(9600);
  while (!Serial);
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
          function01();
          break;
        case 02:
          function02();
          break;
        case 03:
          function03();
          break;
        default:
          break;
        }

      }
    }
  }
}


void function01(){
  Serial.println("Red LED on");
  percentCharacteristc.setValue("RLO");
  digitalWrite(LEDR, LOW);            
  digitalWrite(LEDG, HIGH);         
  digitalWrite(LEDB, HIGH);
}

void function02(){
  Serial.println("Green LED on");
  percentCharacteristc.setValue("GLO");
  digitalWrite(LEDR, HIGH);            
  digitalWrite(LEDG, LOW);         
  digitalWrite(LEDB, HIGH);
}

void function03(){
  Serial.println("Blue LED on");
  percentCharacteristc.setValue("BLO");
  digitalWrite(LEDR, HIGH);            
  digitalWrite(LEDG, HIGH);         
  digitalWrite(LEDB, LOW);
  
}