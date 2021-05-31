#include <Wire.h>

// PINOUT
// buttons
#define BttnPinFloor0Up 10
#define BttnPinFloor1Down 11
#define BttnPinFloor1Up 2
#define BttnPinFloor2Down 3
#define BttnPinFloor2Up 4
#define BttnPinFloor3Down 5
// LEDs
#define ledPinFloor0 6
#define ledPinFloor1 7
#define ledPinFloor2 8
#define ledPinFloor3 9

// želje po nadstropjih - gor(0) in dol(1)
// na zahtevo pošljemo STM32 preko I2C
byte liftPos = 0;
byte floorDemands[] = {0, 0};
int ledState = LOW;

void setup() {
  Serial.begin(9600);
  
  // PinModes
  //buttons
  pinMode(BttnPinFloor0Up, INPUT_PULLUP);
  pinMode(BttnPinFloor1Down, INPUT_PULLUP);
  pinMode(BttnPinFloor1Up, INPUT_PULLUP);
  pinMode(BttnPinFloor2Down, INPUT_PULLUP);
  pinMode(BttnPinFloor2Up, INPUT_PULLUP);
  pinMode(BttnPinFloor3Down, INPUT_PULLUP);
  // LEDs
  pinMode(ledPinFloor0, OUTPUT);
  pinMode(ledPinFloor1, OUTPUT);
  pinMode(ledPinFloor2, OUTPUT);
  pinMode(ledPinFloor3, OUTPUT);
  pinMode(13, OUTPUT);

  // I2C konfiguracija
  Wire.begin(0x14);
  Wire.onRequest(DataRequest);
  Wire.onReceive(DataReceive);
}

void loop() {
  checkButtons();
  delay(10);

  Serial.println(digitalRead(BttnPinFloor3Down));
}

// preveri pritisk gumbov
void checkButtons() {
  if (digitalRead(BttnPinFloor0Up) == LOW){
    floorDemands[0] |= 1;
  }
  if (digitalRead(BttnPinFloor1Up) == LOW){
    floorDemands[0] |= 0b10;
  }
  if (digitalRead(BttnPinFloor2Up) == LOW){
    floorDemands[0] |= 0b100;
  }

  if (digitalRead(BttnPinFloor1Down) == LOW){
    floorDemands[1] |= 0b10;
  }
  if (digitalRead(BttnPinFloor2Down) == LOW){
    floorDemands[1] |= 0b100;
  }
  if (digitalRead(BttnPinFloor3Down) == LOW){
    floorDemands[1] |= 0b1000;
  }
}

// I2C request
void DataRequest(){
  digitalWrite(13, HIGH);
  Wire.write(floorDemands, 2);
  //Serial.println(floorDemands[0]);
  //Serial.println(floorDemands[1]);
  //Serial.println();
  floorDemands[0] = 0;
  floorDemands[1] = 0;
  ledSwitch();
}

// I2C receive
void DataReceive(int numBytes){
  while(Wire.available()) 
  {
    liftPos = Wire.read();
  }
  setFloor(liftPos);
}

// na nadstropju liftPos prižgi LED
void setFloor(byte liftPos) {
  if (liftPos == 0) {
    digitalWrite(ledPinFloor0, HIGH);
    digitalWrite(ledPinFloor1, LOW);
    digitalWrite(ledPinFloor2, LOW);
    digitalWrite(ledPinFloor3, LOW);
  } else if (liftPos == 1) {
    digitalWrite(ledPinFloor0, LOW);
    digitalWrite(ledPinFloor1, HIGH);
    digitalWrite(ledPinFloor2, LOW);
    digitalWrite(ledPinFloor3, LOW);
  } else if (liftPos == 2) {
    digitalWrite(ledPinFloor0, LOW);
    digitalWrite(ledPinFloor1, LOW);
    digitalWrite(ledPinFloor2, HIGH);
    digitalWrite(ledPinFloor3, LOW);
  } else if (liftPos == 3) {
    digitalWrite(ledPinFloor0, LOW);
    digitalWrite(ledPinFloor1, LOW);
    digitalWrite(ledPinFloor2, LOW);
    digitalWrite(ledPinFloor3, HIGH);
  }
}

void ledSwitch(){
    if (ledState == LOW){
      digitalWrite(13, HIGH);
      ledState = HIGH;
    } else {
      digitalWrite(13, LOW);
      ledState = LOW;
    }
}
