build:
	bin/arduino-cli compile --fqbn arduino:avr:mega 420-6502

install:
	bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:mega 420-6502
