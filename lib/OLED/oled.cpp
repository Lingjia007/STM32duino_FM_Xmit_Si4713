// oled.cpp
#include "oled.h"
#include <Wire.h>

TwoWire Wire1;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire1, OLED_RESET, 800000);

void Oled_Init()
{

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println("SSD1306 allocation failed");
        for (;;)
            ; // Don't proceed, loop forever
    }

    Serial.println("SSD1306 allocation successful");

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(1500); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();

    testdrawrect();     // Draw rectangles (outlines)
    testfillrect();     // Draw rectangles (filled)
    testdrawtriangle(); // Draw triangles (outlines)
    testfilltriangle(); // Draw triangles (filled)
}

void testdrawrect(void)
{
    display.clearDisplay();

    for (int16_t i = 0; i < display.height() / 2; i += 2)
    {
        display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, SSD1306_WHITE);
        display.display(); // Update screen with each newly-drawn rectangle
        delay(1);
    }

    delay(100);
}

void testfillrect(void)
{
    display.clearDisplay();

    for (int16_t i = 0; i < display.height() / 2; i += 3)
    {
        // The INVERSE color is used so rectangles alternate white/black
        display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, SSD1306_INVERSE);
        display.display(); // Update screen with each newly-drawn rectangle
        delay(1);
    }

    delay(100);
}

void testdrawtriangle(void)
{
    display.clearDisplay();

    for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 5)
    {
        display.drawTriangle(
            display.width() / 2, display.height() / 2 - i,
            display.width() / 2 - i, display.height() / 2 + i,
            display.width() / 2 + i, display.height() / 2 + i, SSD1306_WHITE);
        display.display();
        delay(1);
    }

    delay(100);
}

void testfilltriangle(void)
{
    display.clearDisplay();

    for (int16_t i = max(display.width(), display.height()) / 2; i > 0; i -= 5)
    {
        // The INVERSE color is used so triangles alternate white/black
        display.fillTriangle(
            display.width() / 2, display.height() / 2 - i,
            display.width() / 2 - i, display.height() / 2 + i,
            display.width() / 2 + i, display.height() / 2 + i, SSD1306_INVERSE);
        display.display();
        delay(1);
    }

    delay(100);
}