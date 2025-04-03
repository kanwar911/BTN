#ifndef CAMERA_H
#define CAMERA_H
#include <string>

class Camera {
    bool isActive;
public:
    Camera() : isActive(true) {}
    std::string status() {
        return isActive ? "Camera is ACTIVE" : "Camera is OFFLINE";
    }
};

#endif