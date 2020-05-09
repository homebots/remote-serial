#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID       "HomeBots"
#define WIFI_PASSWORD   "HomeBots"
#define RELAY_SERVER    "ws://hub.homebots.io/hub"

#include "index.h"
#include "wifi.h"
#include "websocket.h"
#include "homebots.h"
#include "uart.h"

#define RX_INTERVAL 1000

static os_timer_t webSocketCheck;
static os_timer_t rxCheckTimer;
static Wifi wifiConnection;
static ws_info webSocket;
static char uartBuffer[UART_RX_BUFFER_SIZE] = {0};

void onReceive(struct ws_info *socket, int length, char *message, int opCode) {
  if (opCode == WS_OPCODE_TEXT) {
    os_printf("%s\r\n", message);
  }
}

void onConnection(struct ws_info *socket) {
  os_timer_disarm(&rxCheckTimer);
  os_timer_arm(&rxCheckTimer, RX_INTERVAL, 1);
  system_uart_de_swap();
}

void onFailure(struct ws_info *wsInfo, int errorCode) {
  os_timer_disarm(&rxCheckTimer);
  system_uart_swap();
}

void ICACHE_FLASH_ATTR connectWebSocket() {
  if (!wifiConnection.isConnected()) {
    return;
  }

  if (webSocket.connectionState != CS_CONNECTED) {
    ws_connect(&webSocket, RELAY_SERVER);
  }
}

void ICACHE_FLASH_ATTR uartReceive() {
  struct UartBuffer* bufferRef = getRxBuffer();
  uint16 bufferSize = 0;

  if (bufferRef->UartBuffSize - bufferRef->Space > 0) {
    bufferSize = rx_buff_deq(uartBuffer, UART_RX_BUFFER_SIZE);

    if (bufferSize > 0) {
      ws_send(&webSocket, WS_OPCODE_TEXT, (const char*)uartBuffer, bufferSize);
    }
  }

  os_timer_arm(&rxCheckTimer, RX_INTERVAL, 1);
}

void ICACHE_FLASH_ATTR setup() {
  system_uart_swap();

  uart_init(BIT_RATE_115200, BIT_RATE_115200);

  os_timer_disarm(&rxCheckTimer);
  os_timer_setfn(&rxCheckTimer, (os_timer_func_t *)uartReceive , NULL);

  os_timer_setfn(&webSocketCheck, (os_timer_func_t *)connectWebSocket, NULL);
  os_timer_arm(&webSocketCheck, 1000, 1);

  wifiConnection.connectTo(WIFI_SSID, WIFI_PASSWORD);
  webSocket.onReceive = onReceive;
  webSocket.onConnection = onConnection;
  webSocket.onFailure = onFailure;
}

#ifdef __cplusplus
}
#endif
