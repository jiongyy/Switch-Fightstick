//
// Created by 陈龙 on 2018/1/14.
//

#ifndef _ACTION_H_
#define _ACTION_H_

#include "Joystick.h"

typedef enum {
  BUTTON_RESET = 0x00,
  PAD_TOP = 0x01,
  PAD_TOP_LEFT = 0x02,
  PAD_TOP_RIGHT = 0x03,
  PAD_BOTTOM = 0x04,
  PAD_BOTTOM_LEFT = 0x05,
  PAD_BOTTOM_RIGHT = 0x06,
  PAD_LEFT = 0x07,
  PAD_RIGHT = 0x08,
  BUTTON_A = 0x09,
  BUTTON_B = 0x0A,
  BUTTON_X = 0x0B,
  BUTTON_Y = 0x0C,
  BUTTON_L = 0x0D,
  BUTTON_R = 0x0E,
  BUTTON_ZL = 0x0F,
  BUTTON_ZR = 0x10,
  BUTTON_PLUS = 0x11,
  BUTTON_MINUS = 0x12,
  BUTTON_HOME = 0x13,
  BUTTON_CAPTURE = 0x14,
  BUTTON_L_STICK = 0x15,
  BUTTON_R_STICK = 0x16,
  L_STICK_TOP = 0x17,
  L_STICK_TOP_LEFT = 0x18,
  L_STICK_TOP_RIGHT = 0x19,
  L_STICK_BOTTOM = 0x1A,
  L_STICK_BOTTOM_LEFT = 0x1B,
  L_STICK_BOTTOM_RIGHT = 0x1C,
  L_STICK_LEFT = 0x1D,
  L_STICK_RIGHT = 0x1E,
  R_STICK_TOP = 0x1F,
  R_STICK_TOP_LEFT = 0x21,
  R_STICK_TOP_RIGHT = 0x22,
  R_STICK_BOTTOM = 0x23,
  R_STICK_BOTTOM_LEFT = 0x24,
  R_STICK_BOTTOM_RIGHT = 0x25,
  R_STICK_LEFT = 0x26,
  R_STICK_RIGHT = 0x27,
} ACTION_t;

typedef struct {
  ACTION_t action;
  uint16_t wait_time;
} BUTTON_MAP_t;

void setButton(USB_JoystickReport_Input_t *ReportData, ACTION_t action);

void delay(double ms);

#endif
