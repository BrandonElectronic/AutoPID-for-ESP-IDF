In the `AutoPID` algorithm, the decision to use bang-bang control or PID control is based on the current error relative to the set bang-bang thresholds.

### Bang-Bang Control

Bang-bang control is used when the error between the setpoint and the input is outside the defined bang-bang thresholds. Specifically:

- **Bang-On Threshold**: If the error is greater than the `_bangOn` threshold, the output is set to `_outputMax`.
- **Bang-Off Threshold**: If the error is greater than the `_bangOff` threshold, but in the opposite direction, the output is set to `_outputMin`.

### PID Control

PID control is used when the error is within the bang-bang thresholds. This means that the system is close enough to the setpoint that finer control is required. In this case, the PID calculations are performed to adjust the output.

### Detailed Breakdown

1. **Bang-Bang On Control**:
   - If the difference between the setpoint and the input (`*_setpoint - *_input`) is greater than `_bangOn`, the output is set to `_outputMax`.

   ```cpp
   if (_bangOn && ((*_setpoint - *_input) > _bangOn)) {
       *_output = _outputMax;
       _lastStep = esp_timer_get_time() / 1000;
   }
   ```

2. **Bang-Bang Off Control**:
   - If the difference between the input and the setpoint (`*_input - *_setpoint`) is greater than `_bangOff`, the output is set to `_outputMin`.

   ```cpp
   else if (_bangOff && ((*_input - *_setpoint) > _bangOff)) {
       *_output = _outputMin;
       _lastStep = esp_timer_get_time() / 1000;
   }
   ```

3. **PID Control**:
   - If the error is within the bang-bang thresholds, PID control is used. The time step `_dT` is checked to see if enough time has passed since the last update. If so, PID calculations are performed:

   ```cpp
   else { //otherwise use PID control
       unsigned long _dT = (esp_timer_get_time() / 1000) - _lastStep; //calculate time since last update
       if (_dT >= _timeStep) { //if long enough, do PID calculations
           _lastStep = esp_timer_get_time() / 1000;
           double _error = *_setpoint - *_input;
           _integral += (_error + _previousError) / 2 * _dT / 1000.0; //Riemann sum integral
           double _dError = (_error - _previousError) / _dT / 1000.0; //derivative
           _previousError = _error;
           double PID = (_Kp * _error) + (_Ki * _integral) + (_Kd * _dError);
           *_output = std::clamp(PID, _outputMin, _outputMax);
       }
   }
   ```

### Summary

- **Bang-Bang Control**:
  - Used when the error is outside the bang-bang thresholds (`_bangOn` or `_bangOff`).
  - Provides a simple on/off control by setting the output to `_outputMax` or `_outputMin`.

- **PID Control**:
  - Used when the error is within the bang-bang thresholds.
  - Provides finer control by calculating the output using the PID algorithm.

This hybrid approach allows the `AutoPID` algorithm to quickly react when the error is large (using bang-bang control) and finely adjust the output when the error is small (using PID control).
