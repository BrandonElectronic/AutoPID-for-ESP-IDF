#include "AutoPID-for-ESP-IDF.h"

AutoPID::AutoPID(double *input, double *setpoint, double *output, double outputMin, double outputMax,
                 double Kp, double Ki, double Kd) {
  _input = input;
  _setpoint = setpoint;
  _output = output;
  _outputMin = outputMin;
  _outputMax = outputMax;
  setGains(Kp, Ki, Kd);
  _timeStep = 1000;
  _lastStep = esp_timer_get_time() / 1000;  // ESP-IDF equivalent for millis()
}//AutoPID::AutoPID

void AutoPID::setGains(double Kp, double Ki, double Kd) {
  _Kp = Kp;
  _Ki = Ki;
  _Kd = Kd;
}//AutoPID::setControllerParams

void AutoPID::setBangBang(double bangOn, double bangOff) {
  _bangOn = bangOn;
  _bangOff = bangOff;
}//void AutoPID::setBangBang

void AutoPID::setBangBang(double bangRange) {
  setBangBang(bangRange, bangRange);
}//void AutoPID::setBangBang

void AutoPID::setOutputRange(double outputMin, double outputMax) {
  _outputMin = outputMin;
  _outputMax = outputMax;
}//void AutoPID::setOutputRange

void AutoPID::setTimeStep(unsigned long timeStep){
  _timeStep = timeStep;
}

bool AutoPID::atSetPoint(double threshold) {
  return abs(*_setpoint - *_input) <= threshold;
}//bool AutoPID::atSetPoint

void AutoPID::run() {
  if (_stopped) {
    _stopped = false;
    reset();
  }
  //if bang thresholds are defined and we're outside of them, use bang-bang control
  if (_bangOn && ((*_setpoint - *_input) > _bangOn)) {
    *_output = _outputMax;
    _lastStep = esp_timer_get_time() / 1000;
  } else if (_bangOff && ((*_input - *_setpoint) > _bangOff)) {
    *_output = _outputMin;
    _lastStep = esp_timer_get_time() / 1000;
  } else {                                    //otherwise use PID control
    unsigned long _dT = (esp_timer_get_time() / 1000) - _lastStep;   //calculate time since last update
    if (_dT >= _timeStep) {                     //if long enough, do PID calculations
      _lastStep = esp_timer_get_time() / 1000;
      double _error = *_setpoint - *_input;
      _integral += (_error + _previousError) / 2 * _dT / 1000.0;   //Riemann sum integral
      double _dError = (_error - _previousError) / _dT / 1000.0;   //derivative
      _previousError = _error;
      double PID = (_Kp * _error) + (_Ki * _integral) + (_Kd * _dError);
      *_output = std::clamp(PID, _outputMin, _outputMax);
    }
  }
}//void AutoPID::run

void AutoPID::stop() {
  _stopped = true;
  reset();
}
void AutoPID::reset() {
  _lastStep = esp_timer_get_time() / 1000;
  _integral = 0;
  _previousError = 0;
}

bool AutoPID::isStopped(){
  return _stopped;
}

double AutoPID::getIntegral(){
  return _integral;
}

void AutoPID::setIntegral(double integral){
  _integral = integral;
}

void AutoPIDRelay::run() {
  AutoPID::run();
  while ((esp_timer_get_time() / 1000 - _lastPulseTime) > _pulseWidth) _lastPulseTime += _pulseWidth;
  *_relayState = ((esp_timer_get_time() / 1000 - _lastPulseTime) < (_pulseValue * _pulseWidth));
}

double AutoPIDRelay::getPulseValue(){
  return (isStopped()?0:_pulseValue);
}

