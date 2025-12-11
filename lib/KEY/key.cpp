#include "key.h"
#include "menu.h"
#include "si4713.h"
void KEY_Init(void)
{
    pinMode(KEY_Freq_Up, INPUT_PULLUP);
    pinMode(KEY_Freq_Down, INPUT_PULLUP);
    pinMode(KEY_TXpower_Up, INPUT_PULLUP);
    pinMode(KEY_TXpower_Down, INPUT_PULLUP);
}

// 按键扫描（带消抖，按下返回对应按键值，未按下返回KEY_NONE）
Key_t KEY_Scan(void)
{
    static Key_t last_key = KEY_NONE;
    Key_t key = KEY_NONE;

    // 读取按键状态（低电平有效）
    if (digitalRead(KEY_Freq_Up) == LOW)
        key = KEY_FREQ_UP;
    else if (digitalRead(KEY_Freq_Down) == LOW)
        key = KEY_FREQ_DOWN;
    else if (digitalRead(KEY_TXpower_Up) == LOW)
        key = KEY_TXPOWER_UP;
    else if (digitalRead(KEY_TXpower_Down) == LOW)
        key = KEY_TXPOWER_DOWN;
    else
        key = KEY_NONE;

    // 消抖处理
    static uint16_t debounce_cnt = 0;
    static Key_t debounce_key = KEY_NONE;

    if (key != KEY_NONE)
    {
        if (debounce_key == key)
        {
            if (debounce_cnt < 5)
                debounce_cnt++;
        }
        else
        {
            debounce_key = key;
            debounce_cnt = 0;
        }
    }
    else
    {
        debounce_key = KEY_NONE;
        debounce_cnt = 0;
    }

    // 只在按下抬起后生效一次
    if (last_key == KEY_NONE && debounce_cnt >= 5 && key != KEY_NONE)
    {
        last_key = key;
        return key;
    }
    if (key == KEY_NONE)
    {
        last_key = KEY_NONE;
    }
    return KEY_NONE;
}

void KEY_Test(void)
{
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("Press keys for test");
    display.display();
    while (1)
    {
        Key_t key = KEY_Scan();

        if (key != KEY_NONE)
        {
            display.clearDisplay();
            display.setTextSize(2);
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.print("KEY ");

            Serial.print("KEY ");
            switch (key)
            {
            case KEY_FREQ_UP:
                Serial.println("FREQ_UP Pressed");
                display.println("FREQ_UP Pressed");
                break;
            case KEY_FREQ_DOWN:
                Serial.println("FREQ_DOWN Pressed");
                display.println("FREQ_DOWN Pressed");
                break;
            case KEY_TXPOWER_UP:
                Serial.println("TXPOWER_UP Pressed");
                display.println("TXPOWER_UP Pressed");
                break;
            case KEY_TXPOWER_DOWN:
                Serial.println("TXPOWER_DOWN Pressed");
                display.println("TXPOWER_DOWN Pressed");
                break;
            default:
                break;
            }
            display.display(); // 更新显示
        }
        if (menu_command_callback(GET_EVENT_BACK))
            return;
    }
}

bool KEY_Read(void)
{
    return KEY_Scan() != KEY_NONE;
}

// 添加全局状态标志
static bool fm_xmit_active = false;

void KEY_Set_FM_Active(bool active)
{
    fm_xmit_active = active;
}

void KEY_Process(void)
{
    uint8_t key = KEY_Scan();
    switch (key)
    {
    case KEY_FREQ_UP:
        if (fm_xmit_active)
        {
            FM_Freq_Up();
        }
        break;
    case KEY_FREQ_DOWN:
        if (fm_xmit_active)
        {
            FM_Freq_Down();
        }
        break;
    case KEY_TXPOWER_UP:
        break;
    case KEY_TXPOWER_DOWN:
        break;
    default:
        break;
    }
}