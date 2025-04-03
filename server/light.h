#ifndef LIGHT_H
#define LIGHT_H
#include <string>

class Light {
    bool isOn;
public:
    Light() : isOn(false) {}
    std::string turnOn() { isOn = true; return "Light turned ON"; }
    std::string turnOff() { isOn = false; return "Light turned OFF"; }
    std::string status() { return isOn ? "Light is ON" : "Light is OFF"; }
};

#endif