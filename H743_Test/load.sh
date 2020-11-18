#!/bin/bash
sh -c "'openocd' -f './openocd.cfg' \
	-c 'init' \
	-c 'reset halt' \
	-c 'flash write_image erase ./build/H743_Test.hex' \
	-c 'reset' \
	-c 'shutdown'"
echo 'Flsah erase done!';
