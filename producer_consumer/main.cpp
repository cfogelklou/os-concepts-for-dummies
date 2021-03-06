#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std::chrono_literals;

static volatile auto running = true;


void producer(std::deque<uint8_t>* pfifo8) {
  // produce stuff...
  int32_t loops = 0;
  while (running) {
    pfifo8->push_back((loops >> 0) & 0x000000ff);
    pfifo8->push_back((loops >> 8) & 0x000000ff);
    pfifo8->push_back((loops >> 16) & 0x000000ff);
    pfifo8->push_back((loops >> 24) & 0x000000ff);
    loops++;
    std::this_thread::sleep_for(10ms);
  }
}

void consumer(std::deque<uint8_t>* pfifo8) {
  // do stuff...
  std::cout << "Hello from consumer" << std::endl;
  while (running) {
    int32_t x = 0;
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
    if (x != 0) {
      std::cout << "Got " << x << std::endl;
    }
  }
}


int main() {

  std::deque<uint8_t> fifo;
  // std::cout << "Hello world" << std::endl;
  std::thread first(producer, &fifo); // spawn new thread that calls producer()
  std::thread second(consumer, &fifo); // spawn new thread that calls consumer()  

  std::this_thread::sleep_for(20000ms);
  running = false;

  // synchronize threads:
  first.join(); // pauses until first finishes
  second.join();

  std::cout << "Ran for 20 seconds without crashing. Yay!!!" << std::endl;
  
  return 0;
}