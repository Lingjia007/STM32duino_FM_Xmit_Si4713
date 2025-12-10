#ifndef KEY_H
#define KEY_H

#include <Arduino.h>

#define KEY_Freq_Up PB14
#define KEY_Freq_Down PB13

#define KEY_TXpower_Up PC14
#define KEY_TXpower_Down PB15

// 按键枚举类型
typedef enum
{
    KEY_NONE = 0,
    KEY_FREQ_UP,
    KEY_FREQ_DOWN,
    KEY_TXPOWER_UP,
    KEY_TXPOWER_DOWN
} Key_t;

void KEY_Init(void);
Key_t KEY_Scan(void);
void KEY_Test(void);
bool KEY_Read(void);
void KEY_Process(void);

#endif