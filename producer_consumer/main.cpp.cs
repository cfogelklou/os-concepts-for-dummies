#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#ifdef WIN32
#include <Windows.h>
#else
#define CRITICAL_SECTION int
#define EnterCriticalSection(x) do{;}while(0)
#define LeaveCriticalSection(x) do{;}while(0)
#define InitializeCriticalSection(x) do{;}while(0)
#endif


using namespace std::chrono_literals;

static volatile auto running = true;

// Critical section to turn off interrupts (and hence task switching)
// during certain areas of the code.
CRITICAL_SECTION critical_section;


void producer(std::deque<uint8_t>* pfifo8) {
  // produce stuff...
  int32_t loops = 0;
  while (running) {
    EnterCriticalSection(&critical_section);
    pfifo8->push_back((loops >> 0) & 0x000000ff);
    pfifo8->push_back((loops >> 8) & 0x000000ff);
    pfifo8->push_back((loops >> 16) & 0x000000ff);
    pfifo8->push_back((loops >> 24) & 0x000000ff);
    LeaveCriticalSection(&critical_section);
    loops++;
    std::this_thread::sleep_for(10ms);
  }
}

void consumer(std::deque<uint8_t>* pfifo8) {
  // do stuff...
  std::cout << "Hello from consumer" << std::endl;
  while (running) {
    int32_t x = 0;
    EnterCriticalSection(&critical_section);
    if (!pfifo8->empty()) {
      const int32_t x0 = pfifo8->front();
      pfifo8->pop_front();
      const int32_t x1 = pfifo8->front();
      pfifo8->pop_front();
      const int32_t x2 = pfifo8->front();
      pfifo8->pop_front();
      const int32_t x3 = pfifo8->front();
      pfifo8->pop_front();
      x = x0 | (x1 << 8) | (x2 << 16) | (x3 << 24);
    }
    LeaveCriticalSection(&critical_section);
    if (x != 0) {
      std::cout << "Got " << x << std::endl;
    }
  }
}


int main() {
  InitializeCriticalSection(&critical_section);

  std::deque<uint8_t> fifo;
  // std::cout << "Hello world" << std::endl;
  std::thread first(producer, &fifo); // spawn new thread that calls producer()
  std::thread second(consumer, &fifo); // spawn new thread that calls consumer()  

  std::this_thread::sleep_for(20000ms);
  running = false;

  // synchronize threads:
  first.join(); // pauses until first finishes
  second.join();
  
  return 0;
}