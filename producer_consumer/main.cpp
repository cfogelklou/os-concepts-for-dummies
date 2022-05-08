#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
// Shared with the interrupt service routine
char _rxchar = 0;
bool _rxflag = false;

static int32_t uart_rx = 0;

// Hardware address of the serial port
#define UART0_RX_PTR ((volatile char*)&uart_rx)

void foo() {
  // do stuff...
}

void bar(int x) {
  // do stuff...
  std::cout << "Hello from bar" << std::endl;
}


int main() {
  // std::cout << "Hello world" << std::endl;
  std::thread first(foo); // spawn new thread that calls foo()
  std::thread second(bar, 1); // spawn new thread that calls foo()


  // synchronize threads:
  first.join(); // pauses until first finishes
  second.join();
  return 0;
}