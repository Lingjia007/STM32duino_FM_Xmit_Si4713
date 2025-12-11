// main.cpp
#include <Arduino.h>
#include "oled.h"
#include "menu.h"
#include "si4713.h"
#include "encoder.h"
#include "key.h"

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    delay(10);
  }
  Serial.println("Starting...");
  KEY_Init();
  Encoder_Init();
  Oled_Init();
}

void MENU_RunMainMenu(void)
{
  static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                 {"FM Xmit", FM_Xmit_Proc},
                                                 {"Key Test", KEY_Test},
                                                 {"Tools", MENU_RunToolsMenu},
                                                 {"Games", MENU_RunGamesMenu},
                                                 {"Setting", NULL},
                                                 {".."}};

  static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList, .IsSettingList = 0};

  MENU_RunMenu(&MENU);
}

void MENU_RunToolsMenu(void)
{
  static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                 {"Seria", NULL},
                                                 {"Osc Scope", NULL},
                                                 {"PWM Output", NULL},
                                                 {"PWM Input", NULL},
                                                 {"ADC Input", NULL},
                                                 {".."}};

  static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList, .IsSettingList = 1};

  MENU_RunMenu(&MENU);
}

void MENU_RunGamesMenu(void)
{
  static MENU_OptionTypeDef MENU_OptionList[] = {{"<<<"},
                                                 {"Snake", NULL},        // 贪吃蛇
                                                 {"Snake II", NULL},     // 贪吃蛇2
                                                 {"Snake III", NULL},    // 贪吃蛇3
                                                 {"Game of Life", NULL}, // 康威生命游戏
                                                 {".."}};

  static MENU_HandleTypeDef MENU = {.OptionList = MENU_OptionList};

  MENU_RunMenu(&MENU);
}

void loop()
{
  MENU_RunMainMenu();
}