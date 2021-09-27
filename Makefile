build:
	~/bin/arduino-cli compile --warnings all --fqbn arduino:avr:mega .

install:
	~/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:mega .
