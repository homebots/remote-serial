FLASH_SPEED   ?= 230400
ESP_PORT      ?= $$(ls /dev/tty.wchusbserial*)

.PHONY: build flash inspect

build:
	docker run --rm -i -v$$(pwd)/:/home/espbuilder/project darlanalves/homebots-sdk:latest

inspect:
	docker run --rm -i -v$$(pwd)/:/home/espbuilder/project darlanalves/homebots-sdk:latest /bin/bash

asm:
	docker run --rm -i -v$$(pwd)/:/home/espbuilder/project darlanalves/homebots-sdk:latest make disassemble

flash:
	esptool.py --baud $(FLASH_SPEED) --port $(ESP_PORT) write_flash -fm qio -fs 512KB 0x00000 firmware/0x00000.bin 0x10000 firmware/0x10000.bin 0x7c000 firmware/0x7c000.bin

