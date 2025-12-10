// oled.h
#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

extern Adafruit_SSD1306 display;
extern TwoWire Wire1;

void Oled_Init();
void testdrawrect(void);
void testfillrect(void);
void testdrawtriangle(void);
void testfilltriangle(void);

#endif