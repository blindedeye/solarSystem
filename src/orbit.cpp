#include "orbit.h"
#include <cmath>

Orbit::Orbit(float distance, float speed)
    : distanceFromSun(distance), orbitSpeed(speed), currentAngle(0.0f) {}

void Orbit::updateOrbit(float deltaTime) {
    currentAngle += orbitSpeed * deltaTime;
    if (currentAngle >= 360.0f) {
        currentAngle -= 360.0f;  // Keep angle within bounds
    }
}

float Orbit::getX() {
    return distanceFromSun * cos(currentAngle * (M_PI / 180.0f));
}

float Orbit::getZ() {
    return distanceFromSun * sin(currentAngle * (M_PI / 180.0f));
}