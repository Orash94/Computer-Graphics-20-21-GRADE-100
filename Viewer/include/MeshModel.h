#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"


class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	int GetVerticesCount() const;
	const std::string& GetModelName() const;
	const glm::vec3& GetVertexAtIndex(int i) const;

	

	std::vector<Face> getFaces() const;
	void outputFacesAndVertices();

	glm::vec3 getScale();
	glm::vec3 getRotate();
	glm::vec3 getTranslate();
	glm::fmat4x4 getObjectTransformation();

	glm::vec3 getWorldScale();
	glm::vec3 getWorldRotate();
	glm::vec3 getWorldTranslate();
	glm::fmat4x4 getWorldTransformation();


	float getMaxDitancePoints();
	void getMiddleOfModel();
	void setMinMaxVertices();

	void setObjectTransformationUpdates(const glm::vec3 nScale, const glm::vec3 nRotate, const glm::vec3 nTrasnlate);
	void setWorldTransformationUpdates(const glm::vec3 nScale, const glm::vec3 nRotate, const glm::vec3 nTrasnlate);
	void setObjectTransformation(const glm::fmat4x4 transform);
	void setWorldTransformation(const glm::fmat4x4 transform);

	void SetColor(const glm::vec3 _color);
	glm::vec3 GetColor()const;

	void setFrame(glm::fvec3 center, glm::fmat3x3 CoordinateSystem);
	void updateFrame( glm::fmat4x4 transform);

	const glm::vec3& getCenter();
	const glm::fmat3x3& getCoordinateSystem();

	bool displayBoundingBox = false;
	bool displayFaceNormals = false;

private:
	
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 Rotate = glm::vec3(0, 0, 0);
	glm::vec3 Translate = glm::vec3(0, 0, 0);

	glm::vec3 WorldScale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 WorldRotate = glm::vec3(0, 0, 0);
	glm::vec3 WorldTranslate = glm::vec3(0, 0, 0);

	glm::fmat4x4 ObjectTransformation;
	glm::fmat4x4 WorldTransformation;

	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;

	float minX_ = FLT_MAX;
	float minY_ = FLT_MAX;
	float minZ_ = FLT_MAX;
	float maxX_ = FLT_MIN;
	float maxY_ = FLT_MIN;
	float maxZ_ = FLT_MIN;
	

	std::vector<glm::vec3> verticesNormals_;
	std::vector<glm::vec3> facesNormals_;
	std::vector<glm::vec3> facesCenters_;
	
	
	glm::vec3 color;


	glm::vec3 center;
	glm::fmat3x3 CoordinateSystem;
};
