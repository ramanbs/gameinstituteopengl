#include "Camera.h"
#include "glm/gtx/transform.hpp"


Camera::Camera()
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mYaw(0.0f),
	mPitch(0.0f)

{
}

glm::mat4 Camera::getViewMatrix() const 
{
	// camPos or eyePos = glm::vec3 position the virtual camera is located in the world
	// targetPos = the aim point where the camera should be looking
	// up = up direction unit vector of the camera
	// lookat generates the view matrix where in  everything translates and rotates in opp direction of the camera
	return glm::lookAt(mPosition, mTargetPos, mUp);
}


// Orbit Camera

OrbitCamera::OrbitCamera()
	:mRadius(10.0f) 
{
}

void OrbitCamera::setLookAt(const glm::vec3& target) 
{
	mTargetPos = target;
}

void OrbitCamera::setRadius(float radius)
{
	mRadius = glm::clamp(radius, 2.0f, 80.0f);
}

void OrbitCamera::rotate(float yaw, float pitch) 
{
	mYaw = glm::radians(yaw);
	mPitch = glm::radians(pitch);

	// TODO -glm::pi<float>() / 2.0f + 0.1f -  +0.1f (slop factor) so that it gets almost to 90 degree and not exaclty 90. Why ?
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}

// needs to be called every time we rotate the camera
// Converts the angle of yaw and pitch to an x, y and z points
// (r, phi, theta) => P(x, y, z) (So spherical coords to cartesian coords)
// where Yaw angle = theta
// Pitch angle = phi 
// Radial distance to point = r
// P (x, y, z) - Camera Position orbiting around the target point
// The origin of the spherical coord system is the target point .
// Theta - represents the angle in x-y plane, rotates arond y 
// phi - rotates away from the y-axis
// In classical mathematics, in spherical coords z- points upwards so we have to be careful
// To match openGL coords we have to make some modifications
// For conversions : 
// x = rcos(phi)sin(theta)
// y = rsin(phi)
// z = rcos(phi)cos(theta)
// https://en.wikipedia.org/wiki/Spherical_coordinate_system
void OrbitCamera::updateCameraVectors() 
{
	mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
	mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
	mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);
}