#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"

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

protected:
	Camera();

	glm::vec3 mPosition;
	glm::vec3 mTargetPos;
	glm::vec3 mUp;

	//Euler Angles (in radians)
	float mYaw;
	float mPitch;

};

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

#endif // CAMERA_H

