#pragma once
class Camera
{
public:
    Camera();
    Camera(glm::vec3 position);
    ~Camera();

    void setPosition(glm::vec3 position);
    void setRotation(float pitch, float yaw);

    void move(float dx, float dy, float dz);
    void rotate(float pitch, float yaw);
    const glm::mat4& getViewMatrix();
private:
    glm::vec3 mPosition;
    glm::vec3 mUp;
    float mPitch;
    float mYaw;
    float mPitchLimit;

    glm::mat4 mViewMatrix; //the view matrix computed from the other fields
};

