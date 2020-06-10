#pragma once
#include "Vector.h"
#include "Ray.h"

class Camera
{
public:
    Vector3 position;
    Vector3 centerOfInterest;
    Vector3 up;
    double fov;
    double aperture;

    Camera() :
        fov(45), position(0, 0, 0), centerOfInterest(0, 0, -1), up(0, 1, 0), aperture(0), width_(800), height_(600)
    {
    }

    Camera(Vector3 pos, Vector3 coi, Vector3 u, double fv, int width, int height) :
        position(pos), centerOfInterest(coi), up(u), fov(fv), aperture(0), width_(width), height_(height)
    {
    }

    void SetViewport(int w, int h);
    // takes an x in [0,width) and y in [0, height)
    Ray CreateRay(int x, int y);
    Ray CreateRandomRayOld(int x, int y);
    Ray CreateRandomRay(int x, int y); // uses apeture

private:
    Vector3 eyePosition_;
    Vector3 vdx_, vdy_;
    Vector3 upperLeftBound_;

    int width_;
    int height_;
};