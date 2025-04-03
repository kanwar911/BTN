#ifndef THERMOSTAT_H
#define THERMOSTAT_H
#include <string>

class Thermostat {
    int temperature;
public:
    Thermostat() : temperature(22) {}
    std::string setTemperature(int temp) {
        temperature = temp;
        return "Thermostat set to " + std::to_string(temp) + "°C";
    }
    std::string status() {
        return "Current temperature: " + std::to_string(temperature) + "°C";
    }
};

#endif