#include "uart1.hpp"
#include <string>
#include <iostream>
#include <Arduino.h>


#include "uart1.hpp"

char send_buffer[64] = "0.65";

void serial1_init() {
  Serial.println("Serial1 initialized with RX1 and TX1 pins.");
    Serial1.begin(115200, 134217756UL, 1, 0);
}

void serial1_task(void *args)
{
    while (1)
    {
        Serial1.print(send_buffer);
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
        Serial.println(std::stof(send_buffer));
    }
}
