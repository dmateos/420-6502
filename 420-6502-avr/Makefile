avr-setup:
	~/bin/arduino-cli core list
	~/bin/arduino-cli core install arduino:avr

avr-build:
	~/bin/arduino-cli compile --warnings all --fqbn .

avr-install:
	~/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn .

avr-debug:
	screen /dev/ttyUSB0 115200
