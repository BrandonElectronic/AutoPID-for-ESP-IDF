# AutoPID-for-ESP-IDF

AutoPID-for-ESP-IDF is a PID controller library adapted from the AutoPID library designed for Arduino, now compatible with the ESP-IDF (Espressif IoT Development Framework) environment. This library provides an easy way to implement PID control in your ESP32 projects, offering flexibility and precision for various control systems.

## Features

- **PID Control**: Implements Proportional-Integral-Derivative (PID) control for fine-tuned process control.
- **Bang-Bang Control**: Provides bang-bang control mode for simpler on/off control with upper and lower thresholds.
- **Integral Windup Prevention**: Manages integral windup, ensuring stability and performance.
- **Flexible Output Range**: Allows setting custom output ranges for different applications.
- **Adjustable Gains**: Enables dynamic adjustment of Kp, Ki, and Kd gains.
- **Time Step Configuration**: Customizable PID update interval for better control over processing cycles.
- **Relay Control**: Includes a relay control extension for on/off switching applications.

## Capabilities

- **Temperature Control**: Perfect for maintaining a setpoint temperature using sensors and heaters/coolers.
- **Motor Speed Control**: Ideal for controlling the speed of DC motors in various applications.
- **Light Intensity Control**: Can be used to maintain desired light levels with sensors and dimmers.
- **General Process Control**: Suitable for any application requiring precise control over a process variable.

## Getting Started

### Prerequisites

- **ESP-IDF**: Ensure you have the ESP-IDF installed and set up. Follow the official [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) for instructions.
- **Hardware**: ESP32 development board, sensors (e.g., temperature sensor), and actuators (e.g., heaters, motors).

### Installation

1. **Clone the Repository**:
   ```sh
   git clone https://github.com/your-username/AutoPID-for-ESP-IDF.git
   cd AutoPID-for-ESP-IDF
   ```

2. **Directory Structure**:
   Ensure the directory structure looks like this:
   ```
   AutoPID-for-ESP-IDF
   ├── CMakeLists.txt
   ├── Kconfig
   ├── include
   │   └── AutoPID.h
   ├── src
   │   └── AutoPID.cpp
   └── idf_component.yml
   ```

3. **Add to Your ESP-IDF Project**:
   - Add the `AutoPID-for-ESP-IDF` directory to the `components` directory of your ESP-IDF project.
   - Modify your project's `CMakeLists.txt` to include the AutoPID component:
     ```cmake
     list(APPEND EXTRA_COMPONENT_DIRS "path/to/AutoPID-for-ESP-IDF")
     ```

### Usage

1. **Include the Library**:
   ```cpp
   #include "AutoPID.h"
   ```

2. **Initialize and Configure AutoPID**:
   ```cpp
   double temperature, setPoint, outputVal;
   AutoPID myPID(&temperature, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);
   myPID.setBangBang(4);
   myPID.setTimeStep(4000);
   ```

3. **Main Application Loop**:
   ```cpp
   void app_main() {
       init_adc();
       init_dac();
       init_gpio();

       temperatureSensors.begin();
       temperatureSensors.requestTemperatures();
       while (!updateTemperature()) {
           vTaskDelay(pdMS_TO_TICKS(100)); // Wait until temp sensor updated
       }

       while (true) {
           updateTemperature();
           setPoint = read_potentiometer();
           myPID.run(); // Call every loop, updates automatically at certain time interval
           write_dac(outputVal);
           gpio_set_level(LED_PIN, myPID.atSetPoint(1)); // Light up LED when we're at setpoint +-1 degree
           vTaskDelay(pdMS_TO_TICKS(100)); // Delay for a short period
       }
   }
   ```

### Example Project

An example project demonstrating the use of AutoPID for temperature control can be found in the `examples` directory. The example uses a temperature sensor, potentiometer, and DAC output to maintain a setpoint temperature.

### Contributing

Contributions are welcome! Please fork the repository, make your changes, and submit a pull request. Ensure your code follows the project's coding standards and includes relevant tests.

### License

This project is licensed under the MIT License. See the `LICENSE` file for details.

### Acknowledgements

- Original AutoPID library by r-downing: [AutoPID](https://github.com/r-downing/AutoPID)
