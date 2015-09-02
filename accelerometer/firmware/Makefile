# Name: Makefile
# Author: Timothy McPherson
# Copyright: 
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = atmega328p
CLOCK      = 16000000
PROGRAMMER = #-c stk500v2 -P avrdoper

FUSES      = -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m # arduino defaults

ARDCOREDIR = /Applications/Arduino.app/Contents/Java/hardware/arduino/avr/cores/arduino/

ARDCORE_SOURCE_CPP = $(filter-out $(ARDCOREDIR)/main.cpp, $(wildcard $(ARDCOREDIR)/*.cpp))
ARDCORE_SOURCE_C = $(wildcard $(ARDCOREDIR)/*.c)
ARDCORE_SOURCE_ASM = $(wildcard $(ARDCOREDIR)/*.S)

MY_SOURCE_CPP = main.cpp
MY_SOURCE_C = # nothing


OBJECTS    = $(MY_SOURCE_CPP:.cpp=.o) $(MY_SOURCE_C:.c=.o) $(ARDCORE_SOURCE_CPP:.cpp=.o) $(ARDCORE_SOURCE_C:.c=.o) $(ARDCORE_SOURCE_ASM:.S=.o)

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -I$(ARDCOREDIR) -I$(ARDCOREDIR)/../../variants/standard -Wl,--gc-sections -ffunction-sections -fdata-sections

# symbolic targets:
all:	main.hex

%.o: %.cpp
	$(COMPILE) -c $< -o $@

%.o: %.c
	$(COMPILE) -c $< -o $@

%.o: %.S
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

%.s: %.c
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(DEVICE) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
