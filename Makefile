setup-avr:
	~/bin/arduino-cli core list
	~/bin/arduino-cli core install arduino:avr

avr-debug-console:
	screen /dev/ttyUSB0 115200

avr-build:
	~/bin/arduino-cli compile --warnings all --fqbn arduino:avr:mega 420-6502-avr/.

avr-install:
	~/bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:mega 420-6502-avr/.
