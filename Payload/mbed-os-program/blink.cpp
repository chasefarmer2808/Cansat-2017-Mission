#include "mbed.h"

DigitalOut led(LED1);

int main() {
  while (true) {
    wait(1);
    led = 1;
    wait(0.3);
    led = 0;
  }
}
