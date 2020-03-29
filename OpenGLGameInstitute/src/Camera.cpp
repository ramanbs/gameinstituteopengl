#include "Camera.h"
#include "glm/gtx/transform.hpp"

// Default Camera Values
const float DEF_FOV = 45.0f;

Camera::Camera()
	:mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mTargetPos(glm::vec3(0.0f, 0.0f, 0.0f)),
	mUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mRight(glm::vec3(0.0f, 0.0f, 0.0f)),
	WORLD_UP(glm::vec3(0.0f, 1.0f, 0.0f)),
	mYaw(0.0f),
	mPitch(0.0f),
	mFOV(DEF_FOV)

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

const glm::vec3& Camera::getLook() const
{
	return mLook;
}

const glm::vec3& Camera::getRight() const
{
	return mRight;
}

const glm::vec3& Camera::getUp() const
{
	return mUp;
}

//-------------------------------------------------
// Orbit Camera Class
//-------------------------------------------------

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

//-------------------------------------------------
// FPS Camera Class
//-------------------------------------------------

FPSCamera::FPSCamera(glm::vec3 position, float yaw, float pitch) 
{
	mPosition = position;
	mYaw = yaw;
	mPitch = pitch;
}

// Set the camera's position
void FPSCamera::setPosition(const glm::vec3& position) 
{
	mPosition = position;
}

//Move the camera's position
void FPSCamera::move(const glm::vec3& offsetPos) 
{
	mPosition += offsetPos;
	updateCameraVectors();
}

// Rotate camera using Yaw and Pitch angles passed in degrees (similar to Orbit Camera)
void FPSCamera::rotate(float yaw, float pitch) 
{
	mYaw += glm::radians(yaw);
	mPitch += glm::radians(pitch);

	// Constrain the pitch
	//TODO : why not exactly 90 degrees what happens ?
	mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

	updateCameraVectors();
}
// Calculates the vectors from the Camera's (updated) Euler Angles
void FPSCamera::updateCameraVectors() 
{
	//Spherical to Cartesian Coordinates
	// https://en.wikipedia.org/wiki/Spherical_coordinate_system (Note: Our coordinates sys has Y up not Z)
	// calculate the view direction vector based on yaw and pitch angles (roll not considered)

	glm::vec3 look;
	look.x = cosf(mPitch) * sinf(mYaw);
	look.y = sinf(mPitch);
	look.z = cosf(mPitch) * cosf(mYaw);

	mLook = glm::normalize(look);

	//Re-calculate the Right and Up vector. For simplicity the Right vector will 
	// be assumed horizontal w.r.t. the world's Up vector
	// When the camera is moving we would always wanna know the up and right vector of the camera. For this we do the cross product between the directionw where the camera is looking and the world up vector which gives us the right vector. Cross product between the right and the look vector will give Up vector of the camera.
	//TODO :: visulaize these calculations
	mRight = glm::normalize(glm::cross(mLook, WORLD_UP));
	mUp = glm::normalize(glm::cross(mRight, mLook)); // while pitching the camera the Up vector direction changes so need recalculate the Up vector

	mTargetPos = mPosition + mLook;
}

