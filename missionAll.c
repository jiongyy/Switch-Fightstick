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
#include "action.h"
#ifndef ALERT_WHEN_DONE
#define ALERT_WHEN_DONE
#endif

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
  DDRD = 0xFF; //Teensy uses PORTD
  PORTD = 0x0;
  //We'll just flash all pins on both ports since the UNO R3
  DDRB = 0xFF; //uses PORTB. Micro can use either or, but both give us 2 LEDs
  PORTB = 0x0; //The ATmega328P on the UNO will be resetting, so unplug it?
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
  CHOOSE_PEOPLE,
  START_MISSION,
  WAITING
} State_t;
State_t state = SYNC_CONTROLLER;

// region maps

BUTTON_MAP_t prepareMap[] = {
    {BUTTON_B,    1000},
    {BUTTON_B,    1000},
    {BUTTON_B,    1000},
    {BUTTON_B,    1500},

    {BUTTON_PLUS, 1500}, // 打开界面
    {PAD_RIGHT,   50},
    {PAD_RIGHT,   50},
    {PAD_RIGHT,   50},
    {PAD_RIGHT,   50},
    {BUTTON_A,    1000}, // 进入佣兵团


    {BUTTON_A,    1000}, // 确认任务1
    {BUTTON_B,    2000},
    {BUTTON_A,    1000}, // 1
    {BUTTON_B,    2000},
    {BUTTON_A,    1000}, // 2
    {BUTTON_B,    2000},
    {BUTTON_A,    1000}, // 3
    {BUTTON_B,    2000},
    {BUTTON_A,    1000}, // 4
    {BUTTON_B,    2000},
    {BUTTON_A,    1000}, // 5
    {BUTTON_B,    2000},
    {BUTTON_A,    1000}, // 6
    {BUTTON_B,    2000},
    {BUTTON_A,    1000}, // 7
    {BUTTON_B,    2000},
    {BUTTON_A,    1000},
    {BUTTON_B,    1000},
};
BUTTON_MAP_t chooseMap[] = {
    {BUTTON_A,   500}, // 进入商会
    {PAD_RIGHT,  50},
    {PAD_RIGHT,  50},
    {PAD_RIGHT,  50},
    {PAD_BOTTOM, 50},
    {BUTTON_A,   1000}, // 任务1

    {PAD_BOTTOM, 50},
    {PAD_BOTTOM, 50},
    {PAD_RIGHT,  50},
    {PAD_RIGHT,  50},
    {PAD_RIGHT,  50},
    {BUTTON_A,   50}, // 七冰
    {PAD_BOTTOM, 50},
    {PAD_BOTTOM, 50},
    {PAD_RIGHT,  50},
    {BUTTON_A,   50}, // 月
    {PAD_BOTTOM, 50},
    {PAD_LEFT,   50},
    {BUTTON_A,   50}, // 路人暗
    {PAD_BOTTOM, 50},
    {PAD_LEFT,   50},
    {PAD_LEFT,   50},
    {PAD_LEFT,   50},
    {BUTTON_A,   50},  // 路人暗
    {PAD_BOTTOM, 50},
    {PAD_BOTTOM, 50},
    {PAD_BOTTOM, 50},
    {PAD_BOTTOM, 50},
    {PAD_RIGHT,  50},
    {PAD_RIGHT,  50},
    {PAD_RIGHT,  50},
    {BUTTON_A,   50},  // 路人暗
    {PAD_BOTTOM, 50},
    {BUTTON_A,   50},  // 路人暗

    {BUTTON_X,   1000},
    {BUTTON_A,   500},
    {PAD_TOP,    100},
    {BUTTON_A,   300}, // 语音
    {BUTTON_A,   4000}, // 语音
    {BUTTON_A,   500}, // 任务1 开始
};

BUTTON_MAP_t startMissionMap[] = {
    {BUTTON_PLUS, 100},
};
// endregion

int report_count = 0;
int mapPos = 0;

bool holding = true;
bool waiting = false;

#define DEFAULT_HOLD_TIME 50
int hold_time = DEFAULT_HOLD_TIME;
int wait_time = 50;
int mission_time = 30;

uint8_t ports_val = 0;

// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t *const ReportData) {

  setButton(ReportData, BUTTON_RESET);

  if (holding && hold_time) {
    #ifdef ALERT_WHEN_DONE
    PORTD = ports_val;
    PORTB = ports_val;
    #endif
    delay(hold_time);
    holding = false;
    waiting = true;
  } else if (waiting && wait_time) {
    #ifdef ALERT_WHEN_DONE
    PORTD = ~ports_val;
    PORTB = ~ports_val;
    #endif
    delay(wait_time);
    waiting = false;
  } else {
    // region do something
    switch (state) {
    case SYNC_CONTROLLER:
      if (report_count > 100) {
        report_count = 0;
        wait_time = 500;
        mapPos = 0;
        state = PREPARE;
      } else if (report_count == 25 || report_count == 50) {
        setButton(ReportData, BUTTON_L);
        setButton(ReportData, BUTTON_R);
      } else if (report_count == 75 || report_count == 100) {
        setButton(ReportData, BUTTON_A);
      }
      report_count++;
      wait_time = 0;
      break;
    case PREPARE:
      setButton(ReportData, prepareMap[mapPos].action);
      wait_time = prepareMap[mapPos].wait_time;

      mapPos++;
      if (mapPos >= (sizeof(prepareMap) / sizeof(BUTTON_MAP_t))) {
        state = CHOOSE_PEOPLE;
        mapPos = 0;
      }
      break;
    case CHOOSE_PEOPLE:
      setButton(ReportData, chooseMap[mapPos].action);
      wait_time = chooseMap[mapPos].wait_time;

      mapPos++;
      if (mapPos >= (sizeof(chooseMap) / sizeof(BUTTON_MAP_t))) {
        state = START_MISSION;
        mapPos = 0;
      }
      break;
    case START_MISSION:
      setButton(ReportData, startMissionMap[mapPos].action);
      wait_time = startMissionMap[mapPos].wait_time;

      mapPos++;
      if (mapPos >= (sizeof(startMissionMap) / sizeof(BUTTON_MAP_t))) {
        state = WAITING;
        mission_time = 7;
        mapPos = 0;
      }
      break;
    case WAITING:
      while (mission_time) {
        _delay_ms(60000);
        mission_time--;
      }
      mission_time = 0;
      mapPos = 0;
      state = PREPARE;
      //if (mission_time > 0) {
      //  _delay_ms(60000);
      //  mission_time--;
      //  setButton(ReportData, BUTTON_B);
      //  hold_time = 50;
      //  wait_time = 0;
      //} else {
      //  mission_time = 0;
      //  state = PREPARE;
      //}
      break;
    }
    // endregion
    if (!hold_time) hold_time = 50;
    holding = true;
  }
}
