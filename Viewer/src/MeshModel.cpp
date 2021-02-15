#include "MeshModel.h"
#include "Utils.h"
#include <cmath>
#include <iostream>
#include <iomanip>

MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> textureCoords,const std::string& model_name) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name)
{
	ObjectTransformation = Utils::getIdMat();
	WorldTransformation = Utils::getIdMat();

	color = glm::vec3(1, 1, 1);
	ambientColor = glm::vec3(0, 0, 0);
	diffuseColor = glm::vec3(0, 0, 0);
	specularColor = glm::vec3(1, 1, 1);


	setModelInMiddle();
	setFrame(glm::fvec3(0.0f, 0.0f, 0.0f), Utils::getIdMat());

	//outputFacesAndVertices();
	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;

			Vertex vertex;
			vertex.position = vertices_[vertexIndex];

			int vertexNormalIndex = currentFace.GetNormalIndex(j) -1;
			vertex.normal = normals[vertexNormalIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}
			else {
				vertex.textureCoords = glm::vec2(0, 0);
			}

			modelVertices.push_back(vertex);
		}
	}
}

MeshModel::~MeshModel()
{
}

void MeshModel::SetColor(const glm::vec3 _color)
{
	color = _color;
}

glm::vec3 MeshModel::GetColor() const
{
	return color;
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


void MeshModel::setFaceNormal(int index, const glm::fvec3 normal)
{
	faces_[index].setFaceNormal(normal);
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

std::vector<glm::vec3> MeshModel::getFacesNormals()
{
	return facesNormals_;
}

std::vector<glm::vec3> MeshModel::getVerticesNormals()
{
	return verticesNormals_;
}

std::vector<glm::vec3> MeshModel::getVerticesNormalsPerFace() const
{
	return normals_;
}

std::vector<glm::vec3> MeshModel::getFacesCenters()
{
	return facesCenters_;
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

glm::fmat4x4 MeshModel::getObjectTransformation() const
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

glm::fmat4x4 MeshModel::getWorldTransformation() const
{
	return WorldTransformation;
}

glm::fmat4x4 MeshModel::getTransformation() const
{
	return glm::inverse(getWorldTransformation()) * getObjectTransformation();
}



void MeshModel:: setMinMaxVertices() {

	float minX = FLT_MAX;
	float minY = FLT_MAX;
	float minZ = FLT_MAX;

	float maxX = FLT_MIN;
	float maxY = FLT_MIN;
	float maxZ = FLT_MIN;

	for (int i = 0; i <= vertices_.size() - 1; i++) {
		glm::fvec3 vertic = vertices_.at(i);
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

void MeshModel::setVerteciesNormals()
{
	verticesNormals_.clear();
	int verticesCount = vertices_.capacity();
	int faceCount = faces_.capacity();

	int* verticesNormalCount	= new int[verticesCount];
	glm::vec3* verticesSum		= new glm::vec3[verticesCount];

	//initialisation of arrays;
	for (int j = 0; j < verticesCount; j++)
	{
		verticesNormalCount[j]	= 0;
		verticesSum[j]		= glm::vec3(0,0,0);
	}

	//loop over all faces
	for (int j = 0; j < faceCount; j++)
	{
		Face& face = faces_.at(j);

		for (int k = 0; k < 3; k++) {
			int index = face.GetVertexIndex(k) - 1;
			verticesNormalCount[index]++;
		}

		glm::vec3 normal = face.getFaceNormal();


		// add face normals to all three vertices in face
		for (int k = 0; k < 3; k++) {
			int index = face.GetVertexIndex(k) - 1;
			verticesSum[index] += normal;
		}

	}


	// calculate vertices normals ; we take the sum of all adjesent face normals of vertex and we  devide by the number of
	// faces
	for (int j = 0; j < verticesCount; j++)
	{
		float x = verticesSum[j][0];
		float y = verticesSum[j][1]; 
		float z = verticesSum[j][2];

		if (verticesNormalCount != 0) {

			x /= verticesNormalCount[j];
			y /= verticesNormalCount[j];
			z /= verticesNormalCount[j];

			//add to vertex normal to  mesh model
			verticesNormals_.push_back(glm::vec3(x, y, z));
		}
	}

	delete[] verticesNormalCount;
	delete[] verticesSum;
}



void MeshModel::setObjectTransformationUpdates(const glm::vec3 nScale, const glm::vec3 nRotate, const glm::vec3 nTrasnlate)
{
	scale = nScale;
	Rotate = nRotate;
	Translate = nTrasnlate;

	glm::fmat4x4 id = Utils::getIdMat();
	glm::fmat4x4 scale = Utils::TransformationScale(getScale());
	glm::fmat4x4 translate = Utils::TransformationTransition(getTranslate());
	glm::fmat4x4 rotateX = Utils::TransformationRotateX(glm::radians(getRotate()[0]));
	glm::fmat4x4 rotateY = Utils::TransformationRotateY(glm::radians(getRotate()[1]));
	glm::fmat4x4 rotateZ = Utils::TransformationRotateZ(glm::radians(getRotate()[2]));

	setObjectTransformation(translate * scale * rotateZ * rotateY * rotateX );
}

void MeshModel::setWorldTransformationUpdates(const glm::vec3 nScale, const glm::vec3 nRotate, const glm::vec3 nTrasnlate)
{
	WorldScale = nScale;
	WorldRotate = nRotate;
	WorldTranslate = nTrasnlate;

	
	glm::fmat4x4 id = Utils::getIdMat();
	glm::fmat4x4 scale = Utils::TransformationScale(getWorldScale());
	glm::fmat4x4 translate = Utils::TransformationTransition(getWorldTranslate());
	glm::fmat4x4 rotateX = Utils::TransformationRotateX(glm::radians(getWorldRotate()[0]));
	glm::fmat4x4 rotateY = Utils::TransformationRotateY(glm::radians(getWorldRotate()[1]));
	glm::fmat4x4 rotateZ = Utils::TransformationRotateZ(glm::radians(getWorldRotate()[2]));

	//setWorldTransformation(translate * scale * rotateZ * rotateY * rotateX );
	setWorldTransformation(glm::inverse(translate) * glm::inverse(scale) * glm::inverse(rotateZ) * glm::inverse(rotateY) * glm::inverse(rotateX));
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

void MeshModel::setFrame(glm::fvec3 center, glm::fmat3x3 CoordinateSystem)
{
	this->center = center;
	this->CoordinateSystem = CoordinateSystem;
}

void MeshModel::updateFrame( glm::fmat4x4 transform)
{

	glm::fvec3 v0 = Utils::applyTransformationToVector(glm::fvec3(1.0f,0.0f,0.0f) , transform);
	glm::fvec3 v1 = Utils::applyTransformationToVector(glm::fvec3(0.0f,1.0f, 0.0f), transform);
	glm::fvec3 v2 = Utils::applyTransformationToVector(glm::fvec3(0.0f, 0.0f,1.0f), transform);

	CoordinateSystem = glm::fmat3x3(v0, v1, v2);
}

const glm::fvec3& MeshModel::getCenter() const
{
	glm::fmat4x4 final = normalTransformation;
	return Utils::applyTransformationToVector(center , final);
}

const glm::fmat3x3& MeshModel::getCoordinateSystem()
{
	return CoordinateSystem;
}

glm::fmat4x4 MeshModel::getNormalTransformation() const
{
	return normalTransformation;
}

GLuint MeshModel::getVBO() const
{
	return vbo;
}

GLuint MeshModel::getVAO() const
{
	return vao;
}

std::vector<Vertex> MeshModel::getVertexes() const
{
	return modelVertices;
}


float MeshModel::getMaxDitancePoints()
{
	


	float deltaX = maxX_ - minX_;
	float deltaY = maxY_ - minY_;
	float deltaZ = maxZ_ - minZ_;

	return glm::max(glm::max(deltaX, deltaY), deltaZ);

}


void MeshModel::setModelInMiddle()
{
	setMinMaxVertices();

	float deltaX = -((minX_ + maxX_) / 2);
	float deltaY = -((minY_ + maxY_) / 2);
	float deltaZ = -((minZ_ + maxZ_) / 2);
	glm::vec3 middled = glm::vec3(deltaX, deltaY, deltaZ);


	//center verices and verices normla
	for (int i = 0; i <= vertices_.size() - 1; i++) {
		vertices_[i] += middled;
	}
}


