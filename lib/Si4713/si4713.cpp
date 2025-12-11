/***************************************************
  This is an example for the Si4713 FM Radio Transmitter with RDS

  Designed specifically to work with the Si4713 breakout in the
  adafruit shop
  ----> https://www.adafruit.com/products/1958

  These transmitters use I2C to communicate, plus reset pin.
  3 pins are required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution

  Many thx to https://github.com/phrm/fmtx/blob/master/firmware/firmware.ino !

 ****************************************************/
#include "si4713.h"
#include "menu.h"

#define RESETPIN PA12

#define FMSTATION 10230 // 10230 == 102.30 MHz
TwoWire Wire_Si4713(PB11, PB10);
Adafruit_Si4713 radio = Adafruit_Si4713(RESETPIN);

void Si4713_Print_ASQ_Status()
{
    switch (radio.currASQ)
    {
    case 0x0001:
        Serial.println("ASQ: Signal level over low!");
        break;
    case 0x0002:
        Serial.println("ASQ: Signal level too high!");
        break;
    case 0x0004:
        Serial.println("ASQ: OVERMOD!");
        break;
    default:
        break;
    }
}
void Si4713_Init()
{
    radio.reset();
    delay(100);

    if (!radio.begin(SI4710_ADDR1, &Wire_Si4713))
    { // begin with address 0x63 (CS high default)
        Serial.println("Couldn't find radio?");
        while (1)
            ;
    }

    // Uncomment to scan power of entire range from 87.5 to 108.0 MHz
    /*
    for (uint16_t f  = 8750; f<10800; f+=10) {
     radio.readTuneMeasure(f);
     Serial.print("Measuring "); Serial.print(f); Serial.print("...");
     radio.readTuneStatus();
     Serial.println(radio.currNoiseLevel);
     }
     */

    Serial.print("\nSet TX power");
    radio.setTXpower(115); // dBuV, 88-115 max

    Serial.print("\nTuning into ");
    Serial.print(FMSTATION / 100);
    Serial.print('.');
    Serial.println(FMSTATION % 100);
    radio.tuneFM(FMSTATION); // 102.3 mhz

    radio.setProperty(SI4713_PROP_TX_ASQ_LEVEL_LOW, 0xFFC6);  // ASQ level low
    radio.setProperty(SI4713_PROP_TX_AQS_LEVEL_HIGH, 0xFFFE); // ASQ level high

    // This will tell you the status in case you want to read it from the chip
    radio.readTuneStatus();
    Serial.print("\tCurr freq: ");
    Serial.println(radio.currFreq);
    Serial.print("\tCurr freqdBuV:");
    Serial.println(radio.currdBuV);
    Serial.print("\tCurr ANTcap:");
    Serial.println(radio.currAntCap);

    //  begin the RDS / RDBS transmission
    radio.beginRDS();
    radio.setRDSstation("AdaRadio");
    radio.setRDSbuffer("Adafruit g0th Radio!");

    Serial.println("RDS on!");

    radio.setGPIOctrl(_BV(1) | _BV(2)); // set GP1 and GP2 to output
}

void FM_Xmit_Proc(void)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("  FM Xmit");
    display.setCursor(0, 16);
    display.print("  Init...");
    display.display();

    Si4713_Init();
    KEY_Set_FM_Active(true); // 启用FM按键控制

    display.setTextSize(1);
    display.setCursor(0, 0);

    while (1)
    {
        // radio.readASQ();
        // Si4713_Print_ASQ_Status();
        // Serial.print("\tCurr InLevel:");
        // Serial.println(radio.currInLevel);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.printf("FM %d.%02d MHz", radio.currFreq / 100, radio.currFreq % 100);
        display.display();
        delay(10);

        if (menu_command_callback(GET_EVENT_BACK))
        {
            KEY_Set_FM_Active(false); // 禁用FM按键控制
            return;
        }
    }
}

void FM_Freq_Up(void)
{
    radio.readTuneStatus();
    delay(50);
    radio.tuneFM(radio.currFreq + 10);
    delay(50);
    radio.readTuneStatus();
    Serial.print("\tCurr freq: ");
    Serial.println(radio.currFreq);
    delay(50);
}

void FM_Freq_Down(void)
{
    radio.readTuneStatus();
    delay(50);
    radio.tuneFM(radio.currFreq - 10);
    delay(50);
    radio.readTuneStatus();
    Serial.print("\tCurr freq: ");
    Serial.println(radio.currFreq);
    delay(50);
}
