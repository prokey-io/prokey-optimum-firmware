#!/bin/bash
if [[ "$OSTYPE" == "linux-gnu" ]]; then
 echo "linux"
 openocd -f $OCD/scripts/interface/stlink-v2.cfg -f $OCD/scripts/target/stm32f2x.cfg -c "program bootloader.bin 0x08010000 reset exit"
elif [[ "$OSTYPE" == "win32" ]]; then
 $OCD/bin/openocd -f $OCD/scripts/interface/stlink-v2.cfg -f $OCD/scripts/target/stm32f2x.cfg -c "program bootloader.bin 0x08010000 reset exit"
fi