#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "AutoPID-for-ESP-IDF.h"

// Logging tag
#define TAG "AutoPID_Example"

// Pins
#define RELAY_PIN GPIO_NUM_5 // GPIO pin for the relay
#define LED_PIN GPIO_NUM_6   // GPIO pin for the LED

#define TEMP_READ_DELAY 800 // can only update temperature every ~800ms

// Setpoint temperature (user can set this value)
#define SETPOINT_TEMPERATURE 30.0

// Simulate an initial temperature reading
double temperature = 25.0; // Starting temperature
double setPoint = SETPOINT_TEMPERATURE;
uint64_t lastTempUpdate; // Tracks clock time of last temperature update

// Function to initialize GPIO for relay and LED
void init_gpio() {
    esp_rom_gpio_pad_select_gpio(RELAY_PIN);
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

// Function to update the temperature reading with a simulated random value near the setpoint
bool updateTemperature() {
    uint64_t now = esp_timer_get_time() / 1000; // Convert to milliseconds
    if ((now - lastTempUpdate) > TEMP_READ_DELAY) {
        // Simulate temperature reading: random value within +-5 degrees of the setpoint
        temperature = setPoint + (rand() % 1000) / 100.0 - 5.0;
        lastTempUpdate = now;
        return true;
    }
    return false;
}

extern "C" void app_main() {
    // Seed the random number generator
    srand((unsigned int)esp_timer_get_time());

    init_gpio();

    bool relayState;
    float KP = 0.12; // Proportional gain
    float KI = 0.0003; // Integral gain
    float KD = 0; // Derivative gain
    AutoPIDRelay myPID(&temperature, &setPoint, &relayState, 4000, KP, KI, KD); // Create an AutoPIDRelay object
    myPID.setBangBang(4); // Set bang-bang control thresholds
    myPID.setTimeStep(4000); // Set PID update interval to 4000ms

    while (true) {
        updateTemperature(); // Update the temperature with a simulated value
        myPID.run(); // Call every loop, updates automatically at the set time interval
        gpio_set_level(RELAY_PIN, relayState); // Control the relay based on PID output
        gpio_set_level(LED_PIN, myPID.atSetPoint(1)); // Light up LED when at setpoint +-1 degree
        ESP_LOGI(TAG, "Temperature: %.2f, SetPoint: %.2f, Relay State: %d", temperature, setPoint, relayState); // Log the values
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for a short period
    }
}
