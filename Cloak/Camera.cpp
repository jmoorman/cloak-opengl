#include "stdafx.h"
#include "Camera.h"


Camera::Camera() : mPosition(0), mUp(0, 1, 0), mPitch(0), mYaw(0), mPitchLimit(75.f)
{
}

Camera::Camera(glm::vec3 position) : mPosition(position), mUp(0, 1, 0), mPitch(0), mYaw(0), mPitchLimit(75.f)
{
}

Camera::~Camera(void)
{
}

void Camera::setPosition(glm::vec3 position) {
    mPosition = position;
}

void Camera::setRotation(float pitch, float yaw) {
    mPitch = pitch;
    mYaw = yaw;

    clampf(mPitch, -mPitchLimit, mPitchLimit);
}

void Camera::move(float dx, float dy, float dz)
{
    mPosition.x += dx;
    mPosition.y += dy;
    mPosition.z += dz;
}

void Camera::rotate(float pitch, float yaw)
{
    mPitch += pitch;
    mYaw += yaw;

    clampf(mPitch, -mPitchLimit, mPitchLimit);
}

const glm::mat4& Camera::getViewMatrix()
{
    float yawInRadians = mYaw * M_PI / 180.f;
    float pitchInRadians = mPitch * M_PI / 180.f;
    //glm::vec3 direction(
    //    cosf(yawInRadians) * cosf(pitchInRadians),
    //    sinf(yawInRadians) * cosf(pitchInRadians),
    //    sinf(pitchInRadians));
    glm::vec3 direction(cosf(yawInRadians), sinf(pitchInRadians), cosf(pitchInRadians) + sinf(yawInRadians));
    glm::vec3 center = glm::normalize(direction);

    mViewMatrix = glm::lookAt(mPosition, center + mPosition, mUp);
    return mViewMatrix;
}