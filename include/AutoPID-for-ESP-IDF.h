#ifndef AUTOPID_FOR_ESP_IDF_H
#define AUTOPID_FOR_ESP_IDF_H

#include <cmath>
#include <algorithm>
#include "esp_timer.h"

class AutoPID {

  public:
    /**
     * @brief Creates a new AutoPID object.
     *
     * This constructor initializes a new AutoPID object with the provided parameters. The PID controller will
     * use the pointers to the input, setpoint, and output variables to update the PID calculations dynamically.
     *
     * @param[in] input Pointer to the variable holding the input value.
     * @param[in] setpoint Pointer to the variable holding the setpoint value.
     * @param[in] output Pointer to the variable holding the output value.
     * @param[in] outputMin The minimum range value for the output. This defines the lower limit of the output.
     * @param[in] outputMax The maximum range value for the output. This defines the upper limit of the output.
     * @param[in] Kp Proportional gain. This gain determines how much the output will respond proportionally to the current error.
     * @param[in] Ki Integral gain. This gain determines how much the output will respond based on the accumulation of past errors.
     * @param[in] Kd Derivative gain. This gain determines how much the output will respond based on the rate of change of the error.
     */
    AutoPID(double *input, double *setpoint, double *output, double outputMin, double outputMax,
            double Kp, double Ki, double Kd);

    /**
     * @brief Manual adjustment of PID gains.
     *
     * This function allows for manual tuning of the PID controller gains. By adjusting the proportional,
     * integral, and derivative gains, the behavior of the PID controller can be modified to better
     * suit the specific control requirements.
     *
     * @param[in] Kp Proportional gain.
     * @param[in] Ki Integral gain.
     * @param[in] Kd Derivative gain.
     */
    void setGains(double Kp, double Ki, double Kd);

    /**
     * @brief Set the bang-bang control thresholds.
     *
     * This function sets the thresholds for bang-bang control, which is a simple on/off control mechanism.
     * When the input is below `(setpoint - bangOn)`, the PID will set `output` to `outputMax`. When the input
     * is above `(setpoint + bangOff)`, the PID will set `output` to `outputMin`.
     *
     * @param[in] bangOn The upper offset from the setpoint. This defines the point below which the output is set to `outputMax`.
     * @param[in] bangOff The lower offset from the setpoint. This defines the point above which the output is set to `outputMin`.
     */
    void setBangBang(double bangOn, double bangOff);

    /**
     * @brief Set the bang-bang control threshold.
     *
     * This function sets the threshold for bang-bang control with a single range value. When the input
     * is below `(setpoint - bangRange)`, the PID will set `output` to `outputMax`. When the input is above
     * `(setpoint + bangRange)`, the PID will set `output` to `outputMin`.
     *
     * @param[in] bangRange The absolute offset from the setpoint. This defines the range around the setpoint for bang-bang control.
     */
    void setBangBang(double bangRange);

    /**
     * @brief Manual (re)adjustment of output range.
     *
     * This function allows for manual adjustment of the output range. The `outputMin` and `outputMax` values
     * define the allowable range for the output variable.
     *
     * @param[in] outputMin The minimum range value for the output.
     * @param[in] outputMax The maximum range value for the output.
     */
    void setOutputRange(double outputMin, double outputMax);

    /**
     * @brief Manual adjustment of PID time interval for calculations.
     *
     * This function sets the time interval (in milliseconds) at which the PID calculations are allowed to run.
     * By default, this interval is set to 1000 milliseconds.
     *
     * @param[in] timeStep The time interval in milliseconds for PID calculations.
     */
    void setTimeStep(unsigned long timeStep);

    /**
     * @brief Indicates if input has reached the desired setpoint.
     *
     * This function checks whether the input value is within a specified threshold of the setpoint.
     * It returns `true` if the input is within ±(threshold) of the setpoint.
     *
     * @param[in] threshold The absolute offset from the setpoint that the input should be at.
     * @return true if the input is within ±(threshold) of the setpoint, otherwise false.
     */
    bool atSetPoint(double threshold);

    /**
     * @brief Automatically runs PID calculations at a certain time interval.
     *
     * This function should be called repeatedly from the main loop. It will only perform PID calculations
     * when the specified time interval has passed. The function reads the input and setpoint values, updates
     * the output, and performs necessary calculations.
     */
    void run();

    /**
     * @brief Stops PID calculations and resets internal PID calculation values.
     *
     * This function stops the PID calculations and resets the internal values used in the calculations,
     * such as the integral and derivative terms. The PID controller can be resumed by calling the `run()` function.
     */
    void stop();

    /**
     * @brief Resets internal PID calculation values.
     *
     * This function resets the internal values used in PID calculations, such as the integral and derivative terms.
     * It only clears the current calculations and does not stop the PID controller from running.
     */
    void reset();

    /**
     * @brief Indicates if PID calculations have been stopped.
     *
     * This function checks whether the PID calculations have been stopped.
     *
     * @return true if the PID calculations have been stopped, otherwise false.
     */
    bool isStopped();

    /**
     * @brief Get the current value of the integral.
     *
     * This function returns the current value of the error integral. It is useful for storing the state
     * of the PID controller after a power cycle.
     *
     * @return The current value of the error integral.
     */
    double getIntegral();

    /**
     * @brief Override the current value of the integral.
     *
     * This function allows for overriding the current value of the error integral. It is useful for resuming
     * the state of the PID controller after a power cycle. This function should be called after `run()` is called
     * for the first time, otherwise the value will be reset.
     *
     * @param[in] integral The value of the error integral to be used.
     */
    void setIntegral(double integral);

  private:
    double _Kp, _Ki, _Kd;
    double _integral, _previousError;
    double _bangOn, _bangOff;
    double *_input, *_setpoint, *_output;
    double _outputMin, _outputMax;
    unsigned long _timeStep, _lastStep;
    bool _stopped;

};//class AutoPID

class AutoPIDRelay : public AutoPID {
  public:
    /**
     * @brief Creates a new AutoPIDRelay object.
     *
     * This constructor initializes a new AutoPIDRelay object with the provided parameters. The PID controller will
     * use the pointers to the input, setpoint, and relayState variables to update the PID calculations dynamically.
     *
     * @param[in] input Pointer to the variable holding the input value.
     * @param[in] setpoint Pointer to the variable holding the setpoint value.
     * @param[in] relayState Pointer to the variable holding the relay state.
     * @param[in] pulseWidth The PWM pulse width in milliseconds. This defines the duration of the relay state.
     * @param[in] Kp Proportional gain. This gain determines how much the output will respond proportionally to the current error.
     * @param[in] Ki Integral gain. This gain determines how much the output will respond based on the accumulation of past errors.
     * @param[in] Kd Derivative gain. This gain determines how much the output will respond based on the rate of change of the error.
     */
    AutoPIDRelay(double *input, double *setpoint, bool *relayState, double pulseWidth, double Kp, double Ki, double Kd)
      : AutoPID(input, setpoint, &_pulseValue, 0, 1.0, Kp, Ki, Kd) {
      _relayState = relayState;
      _pulseWidth = pulseWidth;
    };

    /**
     * @brief Automatically runs PID calculations at a certain time interval.
     *
     * This function should be called repeatedly from the main loop. It will only perform PID calculations
     * when the specified time interval has passed. The function reads the input and setpoint values, updates
     * the output, and performs necessary calculations.
     */
    void run();

    /**
     * @brief Get the current pulse length.
     *
     * This function returns the current pulse length used by the relay. While the relay state is managed internally,
     * this value can be used for additional control or monitoring purposes.
     *
     * @return The current pulse length in milliseconds.
     */
    double getPulseValue();

  private:
    bool * _relayState;
    unsigned long _pulseWidth, _lastPulseTime;
    double _pulseValue;
};//class AutoPIDRelay

#endif // AUTOPID_FOR_ESP_IDF_H
