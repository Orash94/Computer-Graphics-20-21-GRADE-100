#include <memory>
#include <string>
#define _USE_MATH_DEFINES
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Utils.h"
#include <cmath>

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			vertices.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}

	return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath));
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}



glm::fmat4x4  Utils::TransformationScale(const glm::fvec3 position)
{
	float x = position[0];
	float y = position[1];
	float z = position[2];

	glm::fvec4 vec1 = glm::fvec4(x, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, y, 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, z, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return  glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));

}

glm::fmat4x4 Utils::TransformationTransition(const glm::fvec3 position)
{
	float x = position[0];
	float y = position[1];
	float z = position[2];

	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, x);
	glm::fvec4 vec2 = glm::fvec4(0, 1.0f, 0, y);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 1.0f, z);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return  glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::TransformationRotateX(const float angle)
{

	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, cos(angle), -sin(angle), 0);
	glm::fvec4 vec3 = glm::fvec4(0, sin(angle), cos(angle), 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::TransformationRotateY(const float angle)
{

	glm::fvec4 vec1 = glm::fvec4(cos(angle), 0, sin(angle), 0);
	glm::fvec4 vec2 = glm::fvec4(0, 1, 0, 0);
	glm::fvec4 vec3 = glm::fvec4(-sin(angle), 0, cos(angle), 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::TransformationRotateZ(const float angle)
{
	glm::fvec4 vec1 = glm::fvec4(cos(angle), -sin(angle), 0, 0);
	glm::fvec4 vec2 = glm::fvec4(sin(angle), cos(angle), 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 1.0f, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::TransformationShear(const glm::fvec2 vector)
{
	float shearx = vector[0];
	float sheary = vector[1];

	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, shearx, 0);
	glm::fvec4 vec2 = glm::fvec4(0, 1.0f, sheary, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 1.0f, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::TransformationOrthographic()
{
	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, 1.0f, 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 0, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 0);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::TransformationPerspective(const float d)
{
	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, 1.0f, 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 0, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 1/d, 0);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::SetViewVolumeOrthographicTransformation(const float right, const float left, const float top, const float bottom, const float near, const float far)
{
	glm::fvec4 vec1 = glm::fvec4(2.0f/(right-left), 0, 0, -((right+left)/(right - left)));
	glm::fvec4 vec2 = glm::fvec4(0, 2.0f/(top-bottom), 0, -((top + bottom) / (top - bottom)));
	glm::fvec4 vec3 = glm::fvec4(0, 0, 2/(near - far), -((far + near) / (far - near)));
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::SetViewVolumePerspectiveTransformation(const float right, const float left, const float top, const float bottom, const float near, const float far)
{
	glm::fvec4 vec1 = glm::fvec4(2*near/(right-left), 0, (right+left)/(right-left), 0);
	glm::fvec4 vec2 = glm::fvec4(0, 2*near/(top - bottom), (top + bottom)/(top - bottom), 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, -1*((far+near)/(far-near)), -1*((2*far*near)/(far-near)));
	glm::fvec4 vec4 = glm::fvec4(0, 0, -1.0f, 0);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fmat4x4 Utils::getIdMat()
{
	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, 1.0f, 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 1.0f, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	return glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));
}

glm::fvec3 Utils::applyTransformationToVector(const glm::fvec3 vec, glm::fmat4x4& mat)
{
	glm::fvec4 newv0 = Utils::Euclidean2Homogeneous(vec);
	newv0 = mat * newv0;
	return  Utils::Homogeneous2Euclidean(newv0);
}


glm::fvec3 Utils::Homogeneous2Euclidean(const glm::fvec4 vec)
{
	float x = vec[0];
	float y = vec[1];
	float z = vec[2];
	float w = vec[3];
	return glm::fvec3(x / w, y / w, z / w);
}

glm::fvec4 Utils::Euclidean2Homogeneous(const glm::fvec3 vec)
{
	float x = vec[0];
	float y = vec[1];
	float z = vec[2];

	return glm::fvec4(x, y, z, 1);
}

void Utils::resetMatrix(glm::fmat4x4& mat)
{
	glm::fvec4 vec1 = glm::fvec4(1.0f, 0, 0, 0);
	glm::fvec4 vec2 = glm::fvec4(0, 1.0f, 0, 0);
	glm::fvec4 vec3 = glm::fvec4(0, 0, 1.0f, 0);
	glm::fvec4 vec4 = glm::fvec4(0, 0, 0, 1.0f);

	mat = glm::transpose(glm::fmat4x4(vec1, vec2, vec3, vec4));

}