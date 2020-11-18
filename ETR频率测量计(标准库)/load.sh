#!/bin/bash
sh -c "'openocd' -f './openocd.cfg' \
	-c 'init' \
	-c 'reset halt' \
	-c 'flash write_image erase ./OBJ/Template.hex' \
	-c 'reset' \
	-c 'shutdown'"
echo 'Flsah erase done!';