setup:
	~/bin/arduino-cli core list
	~/bin/arduino-cli core install arduino:avr

build:
	~/bin/arduino-cli compile --warnings all --fqbn arduino:avr:mega .

install:
	~/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:mega .

debug:
	screen /dev/ttyUSB0 115200
