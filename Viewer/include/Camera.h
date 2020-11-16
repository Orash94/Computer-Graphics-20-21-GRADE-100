#pragma once
#include <glm/glm.hpp>
#include "Utils.h"
#include "MeshModel.h"
class Camera : public MeshModel
{
public:
	Camera(MeshModel& mesh, glm::vec3& eye_, glm::vec3& at_, glm::vec3& up_);
	virtual ~Camera();

	void SetCameraLookAt(const glm::vec3& eye_, const glm::vec3& at_, const glm::vec3& up_);
	glm::fmat4x4 lookAt(glm::vec3& eye, glm::vec3& at, glm::vec3& up);
	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	glm::vec3 getEye()const;
	glm::vec3 getAt()const;
	glm::vec3 getUp()const;

	

private:
	glm::mat4x4 view_transformation_;
	glm::mat4x4 view_transformation_inverse;
	glm::mat4x4 projection_transformation_;
	glm::vec3 eye;
	glm::vec3 at;
	glm::vec3 up;
};
