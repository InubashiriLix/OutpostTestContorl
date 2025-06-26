#include <Arduino.h>

void led_init() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW); // Turn off the LED initially
    pinMode(LED_BUILTIN_AUX, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void led_task(void *args)
{
    while (1)
    {
        digitalWrite(LED_BUILTIN, HIGH);     // Turn on the LED
        digitalWrite(LED_BUILTIN_AUX, LOW); // Turn on the auxiliary LED
        vTaskDelay(pdMS_TO_TICKS(1000));     // Delay for 1 second
        digitalWrite(LED_BUILTIN, LOW);      // Turn off the LED
        digitalWrite(LED_BUILTIN_AUX, HIGH);  // Turn off the auxiliary LED
        vTaskDelay(pdMS_TO_TICKS(1000));     // Delay for 1 second
    }
}