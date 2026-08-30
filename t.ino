#include <ACAN2515.h>
#define STEERING_POT_PIN 32

uint8_t MCP2515_CS  = 5;
uint8_t MCP2515_INT = 255;
uint32_t QUARTZ_FREQUENCY = 8000000;

ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT);

void setup() {

  delay(500);
  pinMode(STEERING_POT_PIN, INPUT);
  SPI.begin(18, 19, 23, 5); // SCK, MISO, MOSI, CS

  ACAN2515Settings settings(QUARTZ_FREQUENCY, 250000);
  
  can.begin(settings, NULL);

}

void loop() {

  int steeringRaw = analogRead(STEERING_POT_PIN);
  int steeringAngle = map(steeringRaw, 0, 4095, -45, 45);
  int breakPressure = map(4095-steeringRaw, 0, 4095, 0, 100);
  steeringAngle = constrain(steeringAngle, -45, 45);
  breakPressure = constrain(breakPressure, 0, 100);

  can.poll();
  CANMessage msg;
  msg.id = 0x100;
  msg.len = 2;
  msg.data[0] = breakPressure;
  msg.data[1]=steeringAngle+45;

  can.tryToSend(msg);

  uint16_t sendInterval = 1000;
  uint32_t startWait = millis();
  while (millis() - startWait < sendInterval) {
    can.poll();
    delay(5);
  }
}