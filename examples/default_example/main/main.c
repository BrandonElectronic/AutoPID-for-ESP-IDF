#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "AutoPID.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#define TAG "AutoPID_Example"

// Pins
#define POT_PIN ADC1_CHANNEL_0 // GPIO 36 (ADC1_CH0)
#define OUTPUT_PIN DAC_CHANNEL_1 // GPIO 26 (DAC1)
#define TEMP_PROBE_PIN GPIO_NUM_5
#define LED_PIN GPIO_NUM_6

#define TEMP_READ_DELAY 800 // can only read digital temp sensor every ~750ms

// PID settings and gains
#define OUTPUT_MIN 0
#define OUTPUT_MAX 255
#define KP 0.12
#define KI 0.0003
#define KD 0

double temperature, setPoint, outputVal;

OneWire oneWire(TEMP_PROBE_PIN);
DallasTemperature temperatureSensors(&oneWire);

// Input/output variables passed by reference, so they are updated automatically
AutoPID myPID(&temperature, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);

uint64_t lastTempUpdate; // tracks clock time of last temp update

// Function to initialize ADC for potentiometer input
void init_adc() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(POT_PIN, ADC_ATTEN_DB_11);
}

// Function to read analog value from potentiometer
int read_potentiometer() {
    return adc1_get_raw(POT_PIN);
}

// Function to initialize DAC for output
void init_dac() {
    dac_output_enable(OUTPUT_PIN);
}

// Function to write analog value to DAC output
void write_dac(int value) {
    dac_output_voltage(OUTPUT_PIN, value);
}

// Function to initialize GPIO for LED
void init_gpio() {
    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}

// Function to update temperature reading
bool updateTemperature() {
    uint64_t now = esp_timer_get_time() / 1000; // Convert to milliseconds
    if ((now - lastTempUpdate) > TEMP_READ_DELAY) {
        temperature = temperatureSensors.getTempFByIndex(0); // Get temp reading
        lastTempUpdate = now;
        temperatureSensors.requestTemperatures(); // Request reading for next time
        return true;
    }
    return false;
}

void app_main() {
    init_adc();
    init_dac();
    init_gpio();

    temperatureSensors.begin();
    temperatureSensors.requestTemperatures();
    while (!updateTemperature()) {
        vTaskDelay(pdMS_TO_TICKS(100)); // Wait until temp sensor updated
    }

    // If temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
    myPID.setBangBang(4);
    // Set PID update interval to 4000ms
    myPID.setTimeStep(4000);

    while (true) {
        updateTemperature();
        setPoint = read_potentiometer();
        myPID.run(); // Call every loop, updates automatically at certain time interval
        write_dac(outputVal);
        gpio_set_level(LED_PIN, myPID.atSetPoint(1)); // Light up LED when we're at setpoint +-1 degree
        vTaskDelay(pdMS_TO_TICKS(100)); // Delay for a short period
    }
}
