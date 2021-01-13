#!/bin/bash
openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32f2x.cfg -c "program bootloader.bin 0x08010000 reset exit"