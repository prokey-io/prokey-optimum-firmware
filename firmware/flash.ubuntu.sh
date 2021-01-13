#!/bin/bash
openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32f2x.cfg -c "program prokey.bin 0x08020000 reset exit"