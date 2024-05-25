# AutoPID-for-ESP-IDF

AutoPID-for-ESP-IDF is a PID controller library adapted from the [AutoPID](https://github.com/r-downing/AutoPID) library designed for Arduino, now compatible with the ESP-IDF (Espressif IoT Development Framework) environment. This library provides an easy way to implement PID control in your ESP32 projects, offering flexibility and precision for various control systems.

## Features

- **PID Control**: Implements Proportional-Integral-Derivative (PID) control for fine-tuned process control.
- **Bang-Bang Control**: Provides bang-bang control mode for simpler on/off control with upper and lower thresholds.
- **Integral Windup Prevention**: Manages integral windup, ensuring stability and performance.
- **Flexible Output Range**: Allows setting custom output ranges for different applications.
- **Adjustable Gains**: Enables dynamic adjustment of Kp, Ki, and Kd gains.
- **Time Step Configuration**: Customizable PID update interval for better control over processing cycles.
- **Relay Control**: Includes a relay control extension for on/off switching applications.

### Time-scaling and Automatic Value Updating
The PID controller’s run() function can be called as often as possible in the main loop, and will automatically only perform the updated calculations at a specified time-interval. The calculations take the length of this time-interval into account, so the interval can be adjusted without needing to recaculate the PID gains.

Since the PID object stores pointers to the input, setpoint, and output, it can automatically update those variables without extra assignment statements.

### Bang-Bang Control
This library includes optional built-in bang-bang control. When the input is outside of a specified range from the setpoint, the PID control is deactivated, and the output is instead driven to max (bang-on) or min (bang-off).

This can help approach the setpoint faster, and reduce overshooting due to integrator wind-up.

### PWM (Relay) Control
Since the output of a PID control is an analog value, this can be adapted to control an on-off digital output (such as a relay) using pulse-width modulation.

## Getting Started

### Prerequisites

- ESP-IDF: Ensure you have the ESP-IDF installed and set up. Follow the official [ESP-IDF Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html) for instructions.
- Hardware: ESP32 development board, sensors (e.g., temperature sensor), and actuators (e.g., heaters, motors).

### Installation

#### Using ESP Component Registry (Recommended)

Head to the ESP component registry for [AutoPID-for-ESP-IDF](https://components.espressif.com/components/hayschan/AutoPID-for-ESP-IDF/) and follow the instructions to add the library to your project.

#### Using Git (Manual Installation)

1. Clone the Repository:
   ```sh
   git clone https://github.com/hayschan/AutoPID-for-ESP-IDF.git
   cd AutoPID-for-ESP-IDF
   ```

2. Add to Your ESP-IDF Project:
   - Add the `AutoPID-for-ESP-IDF` directory to the `components` directory of your ESP-IDF project.
   - Modify your project's `CMakeLists.txt` to include the AutoPID component

### Usage

An example project demonstrating the use of AutoPID for temperature control can be found in the `examples` directory. The example uses a temperature sensor, potentiometer, and DAC output to maintain a setpoint temperature.

Two examples are provided:
- Basic Example: Demonstrates the basic usage of the AutoPID library for temperature control.
- Temperature Control with Relay: Demonstrates the use of the AutoPID library with relay control for on/off temperature control.

### License

This project is licensed under the MIT License. See the `LICENSE` file for details.

### Acknowledgements

- Original AutoPID library by r-downing: [AutoPID](https://github.com/r-downing/AutoPID)
