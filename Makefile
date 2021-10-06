all: additionneur

additionneur: additionneur.c
	gcc additionneur.c -Wall -o additionneur

run:
# the '-'so that the Makefile ignore errors 
	-./additionneur 0x1 0x1
	-./additionneur 0xffff 0x1
	-./additionneur 1
	-./additionneur azerty 1
	-./additionneur ffffffff 1

clean:
	rm additionneur

