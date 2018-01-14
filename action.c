#include <stdlib.h>
#include "action.h"
#include "Joystick.h"

void setButton(USB_JoystickReport_Input_t *const ReportData, ACTION_t action) {
    switch (action) {
        case BUTTON_RESET:
            ReportData->Button = 0;
            ReportData->LX = STICK_CENTER;
            ReportData->LY = STICK_CENTER;
            ReportData->RX = STICK_CENTER;
            ReportData->RY = STICK_CENTER;
            ReportData->HAT = HAT_CENTER;
            ReportData->VendorSpec = 0;
            break;
        case PAD_TOP:
            ReportData->HAT = HAT_TOP;
            break;
        case PAD_TOP_LEFT:
            ReportData->HAT = HAT_TOP_LEFT;
            break;
        case PAD_TOP_RIGHT:
            ReportData->HAT = HAT_TOP_RIGHT;
            break;
        case PAD_BOTTOM:
            ReportData->HAT = HAT_BOTTOM;
            break;
        case PAD_BOTTOM_LEFT:
            ReportData->HAT = HAT_BOTTOM_LEFT;
            break;
        case PAD_BOTTOM_RIGHT:
            ReportData->HAT = HAT_BOTTOM_RIGHT;
            break;
        case PAD_LEFT:
            ReportData->HAT = HAT_LEFT;
            break;
        case PAD_RIGHT:
            ReportData->HAT = HAT_TOP_RIGHT;
            break;
        case BUTTON_A:
            ReportData->Button |= SWITCH_A;
            break;
        case BUTTON_B:
            ReportData->Button |= SWITCH_B;
            break;
        case BUTTON_X:
            ReportData->Button |= SWITCH_X;
            break;
        case BUTTON_Y:
            ReportData->Button |= SWITCH_Y;
            break;
        case BUTTON_L:
            ReportData->Button |= SWITCH_L;
            break;
        case BUTTON_R:
            ReportData->Button |= SWITCH_R;
            break;
        case BUTTON_ZL:
            ReportData->Button |= SWITCH_ZL;
            break;
        case BUTTON_ZR:
            ReportData->Button |= SWITCH_ZR;
            break;
        case BUTTON_PLUS:
            ReportData->Button |= SWITCH_PLUS;
            break;
        case BUTTON_MINUS:
            ReportData->Button |= SWITCH_MINUS;
            break;
        case BUTTON_HOME:
            ReportData->Button |= SWITCH_HOME;
            break;
        case BUTTON_CAPTURE:
            ReportData->Button |= SWITCH_CAPTURE;
            break;
        case BUTTON_L_STICK:
            ReportData->Button |= SWITCH_LCLICK;
            break;
        case BUTTON_R_STICK:
            ReportData->Button |= SWITCH_RCLICK;
            break;
        case L_STICK_TOP:
            ReportData->LX = STICK_CENTER;
            ReportData->LY = STICK_MIN;
            break;
        case L_STICK_TOP_LEFT:
            ReportData->LX = STICK_MIN;
            ReportData->LY = STICK_MIN;
            break;
        case L_STICK_TOP_RIGHT:
            ReportData->LX = STICK_MAX;
            ReportData->LY = STICK_MIN;
            break;
        case L_STICK_BOTTOM:
            ReportData->LX = STICK_CENTER;
            ReportData->LY = STICK_MAX;
            break;
        case L_STICK_BOTTOM_LEFT:
            ReportData->LX = STICK_MIN;
            ReportData->LY = STICK_MAX;
            break;
        case L_STICK_BOTTOM_RIGHT:
            ReportData->LX = STICK_MAX;
            ReportData->LY = STICK_MAX;
            break;
        case L_STICK_LEFT:
            ReportData->LX = STICK_MIN;
            ReportData->LY = STICK_CENTER;
            break;
        case L_STICK_RIGHT:
            ReportData->LX = STICK_MAX;
            ReportData->LY = STICK_CENTER;
            break;
        case R_STICK_TOP:
            ReportData->RX = STICK_CENTER;
            ReportData->RY = STICK_MIN;
            break;
        case R_STICK_TOP_LEFT:
            ReportData->RX = STICK_MIN;
            ReportData->RY = STICK_MIN;
            break;
        case R_STICK_TOP_RIGHT:
            ReportData->RX = STICK_MAX;
            ReportData->RY = STICK_MIN;
            break;
        case R_STICK_BOTTOM:
            ReportData->RX = STICK_CENTER;
            ReportData->RY = STICK_MAX;
            break;
        case R_STICK_BOTTOM_LEFT:
            ReportData->RX = STICK_MIN;
            ReportData->RY = STICK_MAX;
            break;
        case R_STICK_BOTTOM_RIGHT:
            ReportData->RX = STICK_MAX;
            ReportData->RY = STICK_MAX;
            break;
        case R_STICK_LEFT:
            ReportData->RX = STICK_MIN;
            ReportData->RY = STICK_CENTER;
            break;
        case R_STICK_RIGHT:
            ReportData->RX = STICK_MAX;
            ReportData->RY = STICK_CENTER;
            break;
    }
}

void delay(double ms) {
    ms = abs(ms);
    while (ms > 0) {
        _delay_ms(1);
        ms--;
    }
}