#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

// 引脚定义
#define ENCODER_SW PA7  // 按键引脚
#define ENCODER_CLK PA4 // 时钟引脚
#define ENCODER_DT PA6  // 数据引脚

void Encoder_Init(void);
int Encoder_Read_Dir(void);
bool Encoder_Sw_Down(void);      // 短按检测
bool Encoder_Sw_LongPress(void); // 长按检测

#endif