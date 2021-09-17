build:
	~/bin/arduino-cli compile --fqbn arduino:avr:mega .

install:
	~/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:mega .
