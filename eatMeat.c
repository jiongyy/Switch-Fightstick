/*
Nintendo Switch Fightstick - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

/** \file
 *
 *  Main source file for the posts printer demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "Joystick.h"

// Main entry point.
int main(void) {
    // We'll start by performing hardware and peripheral setup.
    SetupHardware();
    // We'll then enable global interrupts for our use.
    GlobalInterruptEnable();
    // Once that's done, we'll enter an infinite loop.
    for (;;) {
        // We need to run our task to process and deliver data for our IN and OUT endpoints.
        HID_Task();
        // We also need to run the main USB management task.
        USB_USBTask();
    }
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
    // We need to disable watchdog if enabled by bootloader/fuses.
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    // We need to disable clock division before initializing the USB hardware.
    clock_prescale_set(clock_div_1);
    // We can then initialize our hardware and peripherals, including the USB stack.

#ifdef ALERT_WHEN_DONE
    // Both PORTD and PORTB will be used for the optional LED flashing and buzzer.
#warning LED and Buzzer functionality enabled. All pins on both PORTB and \
PORTD will toggle when printing is done.
    DDRD  = 0xFF; //Teensy uses PORTD
    PORTD =  0x0;
                  //We'll just flash all pins on both ports since the UNO R3
    DDRB  = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
    PORTB =  0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
#endif
    // The USB stack should be initialized last.
    USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
    // We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
    // We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
    bool ConfigSuccess = true;

    // We setup the HID report endpoints.
    ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
    ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

    // We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
    // We can handle two control requests: a GetReport and a SetReport.

    // Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
    // If the device isn't connected and properly configured, we can't do anything here.
    if (USB_DeviceState != DEVICE_STATE_Configured)
        return;

    // We'll start with the OUT endpoint.
    Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
    // We'll check to see if we received something on the OUT endpoint.
    if (Endpoint_IsOUTReceived()) {
        // If we did, and the packet has data, we'll react to it.
        if (Endpoint_IsReadWriteAllowed()) {
            // We'll create a place to store our data received from the host.
            USB_JoystickReport_Output_t JoystickOutputData;
            // We'll then take in that data, setting it up in our storage.
            while (Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) !=
                   ENDPOINT_RWSTREAM_NoError);
            // At this point, we can react to this data.

            // However, since we're not doing anything with this data, we abandon it.
        }
        // Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
        Endpoint_ClearOUT();
    }

    // We'll then move on to the IN endpoint.
    Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
    // We first check to see if the host is ready to accept data.
    if (Endpoint_IsINReady()) {
        // We'll create an empty report.
        USB_JoystickReport_Input_t JoystickInputData;
        // We'll then populate this report with what we want to send to the host.
        GetNextReport(&JoystickInputData);
        // Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
        while (Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) !=
               ENDPOINT_RWSTREAM_NoError);
        // We then send an IN packet on this endpoint.
        Endpoint_ClearIN();
    }
}

typedef enum {
    SYNC_CONTROLLER,
    PREPARE,
    BUYING,
    CHANGE_BLADE,
    CHANGE_POS,
    CHANGE_PRE,
    CHANGING,
    EAT_PRE,
    EATING,
    CONFIRM_BLADE,
    DONE
} State_t;
State_t state = SYNC_CONTROLLER;

#define ECHOES 5
int echoes = 0;
USB_JoystickReport_Input_t last_report;

int report_count = 0;

int bladeNum = 62;
int bladePos = 0;
int line = 0;
int row = 0;
int eatCount = 27;

int mapPos = 0;
#define MAP_LEN_PRE 4
#define MAP_LEN_BUY 8
#define MAP_LEN_CHG 8
#define MAP_LEN_EAT_PRE 4
#define MAP_LEN_CON 15

uint16_t prepareMap[MAP_LEN_PRE][3] = {
        {SWITCH_B, HAT_CENTER, 10}, // 不配置
        {SWITCH_B, HAT_CENTER, 10}, // 不配置
        {SWITCH_B, HAT_CENTER, 10}, // 不配置
        {SWITCH_B, HAT_CENTER, 50}, // 不配置
};

uint16_t buyMap[MAP_LEN_BUY][3] = {
        {SWITCH_A, HAT_CENTER, 20}, // 对话
        {SWITCH_A, HAT_CENTER, 20}, // 对话
        {SWITCH_A, HAT_CENTER, 20}, // 购买
        // 第一项
        {SWITCH_A, HAT_CENTER, 20},
        {0,        HAT_LEFT,   20},
        {SWITCH_A, HAT_CENTER, 20},
        //// 第二项
        //{0,        HAT_BOTTOM, 20},
        //{SWITCH_A, HAT_CENTER, 20},
        //{0,        HAT_LEFT,   20},
        //{SWITCH_A, HAT_CENTER, 20},
        //// 第三项
        //{0,        HAT_BOTTOM, 20},
        //{SWITCH_A, HAT_CENTER, 20},
        //{0,        HAT_LEFT,   20},
        //{SWITCH_A, HAT_CENTER, 20},
        // 退出
        {SWITCH_B, HAT_CENTER, 20}, // 不配置
        {SWITCH_B, HAT_CENTER, 50}, // 不配置
};

uint16_t changeMap[MAP_LEN_CHG][3] = {
        {SWITCH_PLUS,  HAT_CENTER, 50},
        {SWITCH_A,     HAT_CENTER, 20},
        {SWITCH_A,     HAT_CENTER, 20},
        {0,            HAT_RIGHT,  20},
        {SWITCH_A,     HAT_CENTER, 20},
        {SWITCH_MINUS, HAT_CENTER, 50},
        {0,            HAT_BOTTOM, 20},
        {SWITCH_A,     HAT_CENTER, 20}
};

uint16_t eatPreMap[MAP_LEN_EAT_PRE][3] = {
        {SWITCH_B, HAT_CENTER, 50},
        {0,        HAT_RIGHT,  20},
        {0,        HAT_RIGHT,  20},
        {0,        HAT_RIGHT,  20},
};
uint16_t confirmMap[MAP_LEN_CON][3] = {
        {SWITCH_B,    HAT_CENTER, 50},
        {SWITCH_B,    HAT_CENTER, 50},
        {SWITCH_PLUS, HAT_CENTER, 100},
        {SWITCH_PLUS, HAT_CENTER, 100},
        {SWITCH_A,    HAT_CENTER, 100},
        {0,           HAT_RIGHT,  100},
        {SWITCH_A,    HAT_CENTER, 100},
        {0,           HAT_RIGHT,  100},
        {SWITCH_A,    HAT_CENTER, 200},
        {SWITCH_A,    HAT_CENTER, 50},
        {SWITCH_ZR,   HAT_CENTER, 200},
        {SWITCH_A,    HAT_CENTER, 50},
        {SWITCH_ZR,   HAT_CENTER, 200},
        {SWITCH_A,    HAT_CENTER, 50},
        {SWITCH_PLUS, HAT_CENTER, 200},
};

uint16_t button = 0;
uint8_t hat = HAT_CENTER;
int waitJump = 0;

// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t *const ReportData) {

    // Prepare an empty report
    memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
    ReportData->LX = STICK_CENTER;
    ReportData->LY = STICK_CENTER;
    ReportData->RX = STICK_CENTER;
    ReportData->RY = STICK_CENTER;
    ReportData->HAT = HAT_CENTER;

    // Repeat ECHOES times the last report
    if (echoes > 0) {
        memcpy(ReportData, &last_report, sizeof(USB_JoystickReport_Input_t));
        echoes--;
        return;
    }

    // States and moves management
    if (waitJump > 0) {
        waitJump--;
    } else {
        switch (state) {
            case SYNC_CONTROLLER:
                if (report_count > 100) {
                    report_count = 0;
                    waitJump = 200;
                    state = PREPARE;
                } else if (report_count == 25 || report_count == 50) {
                    ReportData->Button |= SWITCH_L | SWITCH_R;
                } else if (report_count == 75 || report_count == 100) {
                    ReportData->Button |= SWITCH_A;
                }
                report_count++;
                break;
            case PREPARE:
                button = prepareMap[mapPos][0];
                hat = prepareMap[mapPos][1];
                waitJump = prepareMap[mapPos][2];

                ReportData->Button = button;
                ReportData->HAT = hat;

                mapPos++;
                if (mapPos >= MAP_LEN_PRE) {
                    state = BUYING;
                    mapPos = 0;
                }
                break;
            case BUYING:
                button = buyMap[mapPos][0];
                hat = buyMap[mapPos][1];
                waitJump = buyMap[mapPos][2];

                ReportData->Button = button;
                ReportData->HAT = hat;

                mapPos++;
                if (mapPos >= MAP_LEN_BUY) {
                    state = CHANGE_BLADE;
                    mapPos = 0;
                }
                break;
            case CHANGE_BLADE:
                button = changeMap[mapPos][0];
                hat = changeMap[mapPos][1];
                waitJump = changeMap[mapPos][2];

                ReportData->Button = button;
                ReportData->HAT = hat;

                mapPos++;
                if (mapPos >= MAP_LEN_CHG) {
                    mapPos = 0;
                    state = CHANGE_POS;
                }
                break;
            case CHANGE_POS:
                bladePos++;
                line = (bladePos - 1) / 5;
                row  = (bladePos - 1) % 5;
                state = CHANGE_PRE;
                if (bladePos % 3 == 1) { // 第一个
                    //ReportData->HAT = HAT_BOTTOM;
                    waitJump = 20;
                } else if (bladePos % 3 == 2) { // 第二个
                    ReportData->HAT = HAT_BOTTOM;
                    waitJump = 20;
                } else if (bladePos % 3 == 0) { // 第三个
                    ReportData->HAT = HAT_BOTTOM;
                    waitJump = 20;
                }
                break;
            case CHANGE_PRE:
                ReportData->Button = SWITCH_A;
                waitJump = 20;
                state = CHANGING;
                break;
            case CHANGING:
                if (line > 0) {
                    line--;
                    ReportData->HAT = HAT_BOTTOM;
                } else if (row > 0) {
                    row--;
                    ReportData->HAT = HAT_RIGHT;
                } else {
                    ReportData->Button = SWITCH_A;
                    state = CHANGE_POS;
                    if (bladePos % 3 == 0) {
                        state = EAT_PRE;
                        mapPos = 0;
                    }
                }
                waitJump = 20;
                break;
            case EAT_PRE:
                button = eatPreMap[mapPos][0];
                hat = eatPreMap[mapPos][1];
                waitJump = eatPreMap[mapPos][2];

                ReportData->Button = button;
                ReportData->HAT = hat;

                mapPos++;
                if (mapPos >= MAP_LEN_EAT_PRE) {
                    mapPos = 0;
                    state = EATING;
                }
                break;
            case EATING:
                ReportData->Button = SWITCH_A;
                waitJump = 10;
                report_count++;
                if (report_count > 300) {
                    report_count = 0;
                    mapPos = 0;
                    state = CONFIRM_BLADE;
                }
                break;
            case CONFIRM_BLADE:
                button = confirmMap[mapPos][0];
                hat = confirmMap[mapPos][1];
                waitJump = confirmMap[mapPos][2];

                ReportData->Button = button;
                ReportData->HAT = hat;

                mapPos++;
                if (mapPos >= MAP_LEN_CON) {
                    mapPos = 0;
                    state = PREPARE;
                    if (bladePos >= bladeNum) {
                        state = DONE;
                        break;
                    }
                }
                break;
            case DONE:
                break;
        }
    }

    // Prepare to echo this report
    memcpy(&last_report, ReportData, sizeof(USB_JoystickReport_Input_t));
    echoes = ECHOES;

}
