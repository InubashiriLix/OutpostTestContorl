#include <Arduino.h>
#include <services.hpp>
#include <uart1.hpp>
#include <board_led.hpp>    

void setup() {
    Serial.begin(115200);

    serial1_init();

    led_init();

    if (!ap_init()) {
        Serial.println("Failed to initialize Access Point");
        return;
    }

    service_init();
    
    

    xTaskCreate(ap_service_task, "AP Service Task", 10240, NULL, 3, NULL);
    xTaskCreate(serial1_task, "Serial1 Task", 2048, NULL, 4, NULL);
    xTaskCreate(led_task, "LED Task", 2048, NULL, 5, NULL);

}

void loop() {

}
