#!/bin/bash
cd /home/pi/rpi-rgb-led-matrix
#sudo ./runtext.py --led-chain=4 --led-slowdown-gpio=2 -b75
sudo ./clock.py --led-chain=4 --led-slowdown-gpio=2 #|| sudo ./clock_default.py --led-chain=4 --led-slowdown-gpio=2
