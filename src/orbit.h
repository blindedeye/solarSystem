#ifndef ORBIT_H
#define ORBIT_H

class Orbit {
private:
    float distanceFromSun;
    float orbitSpeed;
    float currentAngle;

public:
    Orbit(float distance, float speed);
    void updateOrbit(float deltaTime);
    float getX();
    float getZ();
};

#endif
