#!/bin/bash

test ! -e /dev/ttySOFT0 -a -e /home/pi/soft_uart/soft_uart.ko && sudo insmod /home/pi/soft_uart/soft_uart.ko
