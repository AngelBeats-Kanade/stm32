#!/bin/bash
sh -c "'openocd' -f './openocd.cfg' \
	-c 'init' \
	-c 'reset halt' \
	-c 'flash write_image erase ./Output/H743_LCD.hex' \
	-c 'reset' \
	-c 'shutdown'"
echo 'Flsah erase done!';
