cmake_minimum_required(VERSION 3.9)
project(switchFightstick)

set(CMAKE_CXX_STANDARD 11)

set(MCU "atmega328p")
set(ARCH "AVR8")
set(F_CPU "16000000")
set(F_USB "${F_CPU}")

##################################################################################
# tools to be used for programming the AVR
##################################################################################
set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER jtag2)
set(AVR_UPLOADTOOL_PORT usb)
# AVR and fuses needs to be set
set(AVR_MCU atmega8)
set(AVR_H_FUSE 0xd9)
set(AVR_L_FUSE 0xc3)

set(CMAKE_C_COMPILER /usr/local/bin/avr-gcc)
set(CMAKE_CXX_COMPILER /usr/local/bin/avr-g++)

set(CMAKE_C_FLAGS "-DUSE_LUFA_CONFIG_HEADER -IConfig/")
set(CMAKE_C_LINK_FLAGS "-mmcu=${MCU}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

add_definitions(-DALERT_WHEN_DONE)
add_definitions(-DWRITING_CODE_IN_IDE)
add_definitions(-D__AVR_ATmega328P__)
add_definitions(-DUSB_CAN_BE_DEVICE)
add_definitions(-DUSE_STATIC_OPTIONS)
#add_definitions(-DFIXED_CONTROL_ENDPOINT_SIZE)
#link_libraries(Config/LUFAConfig.h)
#add_library(LUFA)

include_directories(./avr/include)
include_directories(./)

add_executable(switchFightstick
        LUFA
        Config/LUFAConfig.h
        Descriptors.h
        Joystick.h
        Descriptors.c

        Joystick.c
        eatMeat.c
        mission.c
        missionAll.c
        openCard.c
        aaa.c
        printImage.c
        image.c
        testAlert.c

        bin2c.py
        png2c.py
        makefile
        install.sh

        HORI_Descriptors
        lcd1602.c
        lcd1602.h
        action.h
        action.c)
