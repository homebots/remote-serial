# Remote Serial for esp8266

Serial connection over WiFi using WebSockets

## Requirements

- A relay server for a WebSocket connection (it acts as a proxy for messages)
- esp8266

## Build

The code is built using [Docker](https://www.docker.com/) and [HomeBots SDK](https://github.com/homebots/homebots-sdk)

If you have Docker installed, just run `make` to build the binaries.

## Flash

You will need the [esptool](https://github.com/espressif/esptool) to flash your image.

You can then run `make flash` to write your image to esp8266. If you want to specify a port, run `ESP_PORT=/dev/[tty.yourporthere] make flash`


## Misc

Check the Makefile for more details and commands.

