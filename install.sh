#!/usr/bin/env bash
echo "Compiling..."
file=$1
target=$2

if [ -z "$file" ]
then
  file="Joystick.c"
else
  file="$file"
fi

if [ -z "$target" ]
then
  target="Joystick"
else
  target="$target"
fi

echo "make FILE=${file} TARGET=${target} #with-alert"
make FILE=${file} TARGET=${target} #with-alert
echo ""
echo ""
dfu-programmer atmega16u2 erase
echo ""
echo ""
dfu-programmer atmega16u2 flash ${target}.hex
echo ""
echo ""
dfu-programmer atmega16u2 reset
echo ""
echo ""
echo "delete build files"
rm -f *.d *.o *.bin *.elf *.hex *.lss *.map *.sym *.eep
rm -rf obj