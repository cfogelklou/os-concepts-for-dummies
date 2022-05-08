#include <cstdint>
#include <iostream>
#include <string>
// Shared with the interrupt service routine
char _rxchar = 0;
bool _rxflag = false;

static int32_t uart_rx = 0;

// Hardware address of the serial port
#define UART0_RX_PTR ((volatile char*)&uart_rx)

int main() {
  std::cout << "Hello world" << std::endl;
}