SOURCES=$(wildcard *.s)
OBJECTS=$(SOURCES:.s=.o)

ASM      = ca65
ASMFLAGS = --cpu 65C02
LD      = ld65
LDFLAGS = --config memory.conf


all: $(OBJECTS)
	$(LD) $(LDFLAGS) $^
	python3 bin2c.py --file kernel.bin > kernel-bin.c
	cp kernel-bin.c ../420-6502-avr/

%.o: %.s
	$(ASM) $(ASMFLAGS) -o $@ $(@:.o=.s)

clean:
	$(RM) -- $(OBJECTS)
