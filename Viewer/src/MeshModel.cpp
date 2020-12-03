#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <iostream>
#include <iomanip>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)
{
	ObjectTransformation = Utils::getIdMat();
	WorldTransformation = Utils::getIdMat();

	color = glm::vec3(1, 0, 0);
	setModelInMiddle();
	setFrame(glm::fvec3(0.0f, 0.0f, 0.0f), Utils::getIdMat());
	//outputFacesAndVertices();
	
}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

int MeshModel::GetVerticesCount() const
{
	return vertices_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}


const glm::vec3& MeshModel::GetVertexAtIndex(int i) const
{
	return vertices_[i];
}


std::vector<Face> MeshModel::getFaces() const
{
	return faces_;
}

void MeshModel::outputFacesAndVertices()
{
	std::cout << "\nVertices:" << std::endl;
	for (int k = 0; k < GetVerticesCount(); k++)
	{
		glm::vec3 v = GetVertexAtIndex(k);
		float x = v[0];
		float y = v[1];
		float z = v[2];

		std::cout << "V" << k << " (";
		std::cout << std::fixed << std::setprecision(5) << x << ",";
		std::cout << std::fixed << std::setprecision(5) << y << ",";
		std::cout << std::fixed << std::setprecision(5) << z << ",";
		std::cout << ")" << std::endl;
	}

	std::cout << "Faces:" << std::endl;
	for (int j = 0; j < GetFacesCount(); j++)
	{
		std::cout << "face#" << j << " include these vetices: ";
		Face face = faces_[j];
		for (int i = 0; i < 3; i++)
		{
			int index = face.GetVertexIndex(i) - 1;
			std::cout << "V" << index << "  ";
		}
		std::cout<<std::endl;
	}
}

glm::vec3 MeshModel::getScale()
{
	return scale;
}

glm::vec3 MeshModel::getRotate()
{
	return Rotate;
}

glm::vec3 MeshModel::getTranslate()
{
	return Translate;
}

glm::fmat4x4 MeshModel::getObjectTransformation()
{
	return ObjectTransformation;
}

glm::vec3 MeshModel::getWorldScale()
{
	return WorldScale;
}

glm::vec3 MeshModel::getWorldRotate()
{
	return WorldRotate;
}

glm::vec3 MeshModel::getWorldTranslate()
{
	return WorldTranslate;
}

glm::fmat4x4 MeshModel::getWorldTransformation()
{
	return WorldTransformation;
}


void MeshModel:: setMinMaxVertices() {

	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;

	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	float maxZ = FLT_MIN;

	for (int i = 0; i <= vertices_.size() - 1; i++) {
		glm::fvec3 vertic = vertices_[i];
		float x = vertic[0];
		float y = vertic[1];
		float z = vertic[2];

		if (x < minX) {
			minX = x;
		}
		if (x > maxX) {
			maxX = x;
		}

		if (y < minY) {
			minY = y;
		}
		if (y > maxY) {
			maxY = y;
		}

		if (z < minZ) {
			minZ = z;
		}
		if (z > maxZ) {
			maxZ = z;
		}
	}

	minX_ = minX;
	minY_ = minY;
	minZ_ = minZ;
	maxX_ = maxX;
	maxY_ = maxY;
	maxZ_ = maxZ;

}

void MeshModel::setObjectTransformationUpdates(const glm::vec3 nScale, const glm::vec3 nRotate, const glm::vec3 nTrasnlate)
{
	scale = nScale;
	Rotate = nRotate;
	Translate = nTrasnlate;

	glm::fmat4x4 id = Utils::getIdMat();
	glm::fmat4x4 scale = Utils::TransformationScale(getScale());
	glm::fmat4x4 translate = Utils::TransformationTransition(getTranslate());
	glm::fmat4x4 rotateX = Utils::TransformationRotateX(Utils::degrees2Radians(getRotate()[0]));
	glm::fmat4x4 rotateY = Utils::TransformationRotateY(Utils::degrees2Radians(getRotate()[1]));
	glm::fmat4x4 rotateZ = Utils::TransformationRotateZ(Utils::degrees2Radians(getRotate()[2]));

	setObjectTransformation(translate * scale * rotateZ * rotateY * rotateX * id);
}

void MeshModel::setWorldTransformationUpdates(const glm::vec3 nScale, const glm::vec3 nRotate, const glm::vec3 nTrasnlate)
{
	WorldScale = nScale;
	WorldRotate = nRotate;
	WorldTranslate = nTrasnlate;

	glm::fmat4x4 id = Utils::getIdMat();
	glm::fmat4x4 scale = Utils::TransformationScale(getWorldScale());
	glm::fmat4x4 translate = Utils::TransformationTransition(getWorldTranslate());
	glm::fmat4x4 rotateX = Utils::TransformationRotateX(Utils::degrees2Radians(getWorldRotate()[0]));
	glm::fmat4x4 rotateY = Utils::TransformationRotateY(Utils::degrees2Radians(getWorldRotate()[1]));
	glm::fmat4x4 rotateZ = Utils::TransformationRotateZ(Utils::degrees2Radians(getWorldRotate()[2]));

	setWorldTransformation(translate * scale * rotateZ * rotateY * rotateX * id);
}

void MeshModel::setObjectTransformation(const glm::fmat4x4 transform = Utils::getIdMat())
{
	ObjectTransformation = transform;

}

void MeshModel::setWorldTransformation(const glm::fmat4x4 transform = Utils::getIdMat())
{
	WorldTransformation = transform;
	updateFrame(transform);
}

void MeshModel::SetColor(const glm::vec3 _color)
{
	color = _color;
}

glm::vec3 MeshModel::GetColor() const
{
	return color;
}

void MeshModel::setFrame(glm::fvec3 center, glm::fmat3x3 CoordinateSystem)
{
	this->center = center;
	this->CoordinateSystem = CoordinateSystem;
}

void MeshModel::updateFrame( glm::fmat4x4 transform)
{
	center = Utils::applyTransformationToVector(center, transform);

	glm::fvec3 v0 = Utils::applyTransformationToVector(glm::fvec3(1.0f,0.0f,0.0f) , transform);
	glm::fvec3 v1 = Utils::applyTransformationToVector(glm::fvec3(0.0f,1.0f, 0.0f), transform);
	glm::fvec3 v2 = Utils::applyTransformationToVector(glm::fvec3(0.0f, 0.0f,1.0f), transform);

	CoordinateSystem = glm::fmat3x3(v0, v1, v2);
}

const glm::vec3& MeshModel::getCenter()
{
	return center;
}

const glm::fmat3x3& MeshModel::getCoordinateSystem()
{
	return CoordinateSystem;
}


float MeshModel::getMaxDitancePoints()
{
	


	float deltaX = maxX_ - minX_;
	float deltaY = maxY_ - minY_;
	float deltaZ = maxZ_ - minZ_;

	return glm::max(glm::max(deltaX, deltaY), deltaZ);

}

void MeshModel::getMiddleOfModel()
{
	

	float deltaX = -((minX_ + maxX_) / 2);
	float deltaY = -((minY_ + maxY_) / 2);
	float deltaZ = -((minZ_ + maxZ_) / 2);
	glm::vec3 middled = glm::vec3(deltaX, deltaY, deltaZ);
	for (int i = 0; i <= vertices_.size() - 1; i++) {
		vertices_[i] += middled;
	}
	for (int i = 0; i <= normals_.size() - 1; i++) {
		normals_[i] += middled;
	}

}


