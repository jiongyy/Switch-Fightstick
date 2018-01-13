#!/usr/bin/env bash
echo "Compiling..."
file=$1
target=$2

if [ -z "$file" ]
then
  file="FILE=Joystick.c"
else
  file="FILE=$file"
fi

if [ -z "$target" ]
then
  target="TARGET=Joystick"
else
  target="TARGET=$target"
fi

#echo "make ${file} ${target} #with-alert"
make ${file} ${target} #with-alert
echo ""
echo ""
dfu-programmer atmega16u2 erase
echo ""
echo ""
dfu-programmer atmega16u2 flash $1.hex
echo ""
echo ""
dfu-programmer atmega16u2 reset
echo ""
echo ""
echo "delete build files"
rm -f *.d *.o *.bin *.elf *.hex *.lss *.map *.sym *.eep
rm -rf obj