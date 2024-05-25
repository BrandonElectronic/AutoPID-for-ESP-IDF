Sure! Here's a detailed README for the `bang_bang_temperature_control_relay` example, which highlights the specifics of using the `AutoPID` library for controlling a relay.

---

# Bang-Bang Temperature Control Relay Example

This example demonstrates how to use the `AutoPID` library to implement a bang-bang control algorithm for a heating element, controlled via a relay. The example showcases the features of the `AutoPIDRelay` class, which is specifically designed for relay control applications.

## Features

- **Bang-Bang Control**: Implements a simple on/off control strategy for maintaining temperature within a specified range.
- **Simulated Temperature Input**: Uses a random temperature value near the setpoint for testing purposes.
- **Relay Control**: Uses a relay to control a heating element based on the temperature.

## Prerequisites

- **ESP-IDF**: Ensure you have the ESP-IDF environment set up. Follow the official [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) if needed.
- **Hardware**: ESP32 development board, a relay module, and an LED for indication.

## Hardware Setup

- **Relay Pin**: Connect the relay control pin to GPIO 5 of the ESP32.
- **LED Pin**: Connect an LED to GPIO 6 of the ESP32 to indicate when the system is at the setpoint.

## Code Overview

### Main Components

1. **GPIO Initialization**:
    - Initializes GPIO pins for the relay and LED.
    
2. **Temperature Simulation**:
    - Simulates temperature readings with random values near the setpoint.

3. **Bang-Bang Control with `AutoPIDRelay`**:
    - Uses the `AutoPIDRelay` class to control the relay output.
    - Configures bang-bang control thresholds and time steps.

### Key Points

- **Using `AutoPIDRelay`**: For relay control applications, you should use the `AutoPIDRelay` class instead of the `AutoPID` class. This class is specifically designed to handle relay states efficiently.
- **Setting Parameters**: The `AutoPIDRelay` constructor requires parameters for input, setpoint, relay state, pulse width, and PID gains.

## Code

### main.cpp

```cpp
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

// PID settings
#define OUTPUT_MIN 0
#define OUTPUT_MAX 1

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

    while (true) {
        updateTemperature(); // Update the temperature with a simulated value
        myPID.run(); // Call every loop, updates automatically at the set time interval
        gpio_set_level(RELAY_PIN, relayState); // Control the relay based on PID output
        gpio_set_level(LED_PIN, myPID.atSetPoint(1)); // Light up LED when at setpoint +-1 degree
        ESP_LOGI(TAG, "Temperature: %.2f, SetPoint: %.2f, Relay State: %d", temperature, setPoint, relayState); // Log the values
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for a short period
    }
}
```

1. **Initialization**:
    - The `init_gpio()` function sets up the GPIO pins for the relay and the LED.

2. **Temperature Simulation**:
    - The `updateTemperature()` function generates random temperature values near the setpoint to simulate real sensor data.

3. **Bang-Bang Control**:
    - The `AutoPIDRelay` object is initialized with pointers to the temperature, setpoint, relay state, pulse width, and PID gains.
    - The `setBangBang()` method sets the thresholds for the bang-bang control.
    - The `run()` method updates the relay state based on the temperature.

4. **Logging**:
    - Logs the temperature, setpoint, and relay state for monitoring and debugging.

### Conclusion

This example demonstrates how to use the `AutoPIDRelay` class for relay control in a bang-bang temperature control system. It highlights the simplicity and effectiveness of the `AutoPID` library for various control applications. By using `AutoPIDRelay`, you can efficiently manage relay states and implement robust control systems with minimal code.
