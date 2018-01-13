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

include_directories(/usr/local/Cellar/avr-gcc/7.2.0/avr/include)
include_directories(./LUFA)

add_executable(switchFightstick
        LUFA/Build/lufa_core.mk
        LUFA/Build/lufa_sources.mk
        LUFA/Build/lufa_build.mk
        LUFA/Build/lufa_cppcheck.mk
        LUFA/Build/lufa_doxygen.mk
        LUFA/Build/lufa_dfu.mk
        LUFA/Build/lufa_hid.mk
        LUFA/Build/lufa_avrdude.mk
        LUFA/Build/lufa_atprogram.mk
        Config/LUFAConfig.h
        Descriptors.c
        Descriptors.h
        Joystick.h

        Joystick.c
        eatMeat.c
        mission.c
        openCard.c
        aaa.c
        printImage.c
        image.c
        testAlert.c

        bin2c.py
        png2c.py
        makefile
        install.sh

        HORI_Descriptors)
