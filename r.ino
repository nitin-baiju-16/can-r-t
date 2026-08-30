#include <ACAN2515.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

uint8_t MCP2515_CS  = 5;
uint8_t MCP2515_INT = 255;

static const uint32_t QUARTZ_FREQUENCY = 8000000;

ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT);

void setup() {

  delay(500);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  SPI.begin(18, 19, 23, 5);
  ACAN2515Settings settings(QUARTZ_FREQUENCY, 250000);

  can.begin(settings, NULL);
}

void loop() {

  can.poll();

  CANMessage msg;

  if (can.receive(msg)) {

    
    if (msg.id == 0x100 && msg.len >= 2) {

      int brakePressure = msg.data[0];
      int steeringAngle = -(msg.data[1]-45);

      display.clearDisplay();
      display.setTextColor(SSD1306_WHITE);
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("Brake Pressure: ");
      display.setCursor(93, 0);
      display.print(brakePressure);
      display.print("%");
      display.setCursor(0, 20);
      display.println("Steering Angle: ");
      display.setCursor(100, 20);
      display.print(steeringAngle);
      display.display();

    }
  }
}