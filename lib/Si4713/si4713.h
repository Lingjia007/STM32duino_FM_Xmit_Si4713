#include <Wire.h>
#include <Adafruit_Si4713.h>

extern Adafruit_Si4713 radio;
void Si4713_Init();
void Si4713_Print_ASQ_Status();
void FM_Xmit_Proc(void);
void FM_Freq_Up(void);
void FM_Freq_Down(void);