#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128  // OLED display width,  in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void write_display_char(char c) {
  oled.write(c);
  oled.display();
}

void clear_display() {
  oled.setCursor(0, 0);
  oled.clearDisplay();
  oled.println("420-6502 BIOS 0.1");
}

void setup_display() {
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true)
      ;
  }

  clear_display();
  oled.setTextSize(1);
}