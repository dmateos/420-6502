avr-setup:
	~/bin/arduino-cli core list
	~/bin/arduino-cli core install arduino:avr

avr-build:
	~/bin/arduino-cli compile --warnings all --fqbn arduino:avr:mega .

avr-install:
	~/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:mega .

avr-debug:
	screen /dev/ttyUSB0 115200
