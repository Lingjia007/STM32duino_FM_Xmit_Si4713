#include "encoder.h"
#include "key.h"

// 定时器
HardwareTimer *timer3;

// 全局变量
volatile int encoder_count = 0;
volatile bool encoder_button_pressed = false;
volatile bool encoder_button_long_pressed = false;
volatile unsigned long button_press_start_time = 0;

void Timer3_IRQHandler(void)
{
    // 编码器旋转检测 - 只检测上升沿
    static int lastCLK = HIGH;
    int currentCLK = digitalRead(ENCODER_CLK);

    if (lastCLK == LOW && currentCLK == HIGH)
    {
        if (digitalRead(ENCODER_DT) != currentCLK)
        {
            encoder_count++;
        }
        else
        {
            encoder_count--;
        }
    }
    lastCLK = currentCLK;

    // 按键检测（防抖动）
    static int button_state = HIGH;
    static int last_button_state = HIGH;
    static unsigned long last_debounce_time = 0;

    int reading = digitalRead(ENCODER_SW);

    if (reading != last_button_state)
    {
        last_debounce_time = millis();
    }

    if ((millis() - last_debounce_time) > 50)
    {
        if (reading != button_state)
        {
            button_state = reading;
            if (button_state == LOW) // 按下
            {
                button_press_start_time = millis();
            }
            else // 释放
            {
                unsigned long press_duration = millis() - button_press_start_time;
                if (press_duration < 1000) // 短按
                {
                    encoder_button_pressed = true;
                }
                else // 长按
                {
                    encoder_button_long_pressed = true;
                }
            }
        }
    }

    last_button_state = reading;

    KEY_Process();
}

// 检测短按
bool Encoder_Sw_Down(void)
{
    if (encoder_button_pressed)
    {
        encoder_button_pressed = false;
        return true;
    }
    return false;
}

// 检测长按
bool Encoder_Sw_LongPress(void)
{
    if (encoder_button_long_pressed)
    {
        encoder_button_long_pressed = false;
        return true;
    }
    return false;
}

void Encoder_Init()
{
    // GPIO初始化 - 上拉输入模式
    pinMode(ENCODER_SW, INPUT_PULLUP);
    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);

    // 定时器初始化 - 10ms中断
    timer3 = new HardwareTimer(TIM3);
    timer3->setOverflow(2000, MICROSEC_FORMAT); // 2ms = 2000us
    timer3->attachInterrupt(Timer3_IRQHandler);
    timer3->resume();
}

// 读取旋转方向
int Encoder_Read_Dir(void)
{
    int dir = 0;
    if (encoder_count > 0)
    {
        dir = 1;
        encoder_count--;
    }
    else if (encoder_count < 0)
    {
        dir = -1;
        encoder_count++;
    }
    return dir;
}