#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

//-------------------------------------------------
// Abstract Camera Class
//-------------------------------------------------

class Camera
{
public:

	glm::mat4 getViewMatrix() const;
	// yaw - rotation along y-axis (in degress)
	// pitch - rotation along x-axis (in degress)
	//camera rotates around its local axis  
	// since its virtual, added an empty definition so that derived class can implement that
	virtual void rotate(float yaw, float pitch) {};
	virtual void setPosition(const glm::vec3 position) {} // Sets the posiiton of the camera
	virtual void move(const glm::vec3& offsetPos) {} // will move the camera incrementally by a certain offset

	const glm::vec3& getLook() const; // same as forward vector specified in orbit camera class
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getPosition() const;

	float getFOV() const { return mFOV; }
	void setFOV(float fov) { mFOV = fov; } // in degrees

protected:
	Camera();

	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;
	glm::vec3 mLook; // Forwards facing z-axis
	glm::vec3 mRight; // Along camera's x-axis

	const glm::vec3 WORLD_UP;

	//Euler Angles (in radians)
	float mYaw;
	float mPitch;

	// Camera parameters
	float mFOV; //degrees

};

//-------------------------------------------------
// Orbit Camera Class
//-------------------------------------------------

class OrbitCamera : public Camera
{
public:
	OrbitCamera();

	// yaw - rotation along y-axis (in degress)
	// pitch - rotation along x-axis (in degress)
	//camera rotates around its local axis  
	virtual void rotate(float yaw, float pitch);

	void setLookAt(const glm::vec3& target);
	void setRadius(float radius);

private:

	void updateCameraVectors();
	
	float mRadius;
};

//-------------------------------------------------
// FPS Camera Class
//-------------------------------------------------

class FPSCamera : public Camera 
{
public:

	FPSCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = glm::pi<float>(), float pitch = 0.0f); // (yaw) initial angle faces -Z (facing down)

	virtual void setPosition(const glm::vec3& position);
	virtual void rotate(float yaw, float pitch); // in degrees
	virtual void move(const glm::vec3& offsetPos);

private:

	void updateCameraVectors();
};


#endif // CAMERA_H

