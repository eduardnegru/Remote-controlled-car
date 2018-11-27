all: lab0.hex

lab0.hex: lab0.elf
	avr-objcopy  -j .text -j .data -O ihex $^ $@
	avr-size lab0.elf

lab0.elf: lab0.c usart.c
	avr-gcc -mmcu=atmega324p -DF_CPU=16000000 -std=c99 -Wall -Os -o $@ $^
	
run: lab0.hex
	./bootloadHID -r lab0.hex

clean:
	rm -rf lab0.elf lab0.hex
