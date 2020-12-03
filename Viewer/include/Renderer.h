#pragma once
#include "Scene.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void SetViewport(const int width, const int height);
	
private:
	void PutPixel(const int i, const int j, const float z, const glm::vec3& color);
	void DrawLine(const glm::fvec3& p1, const glm::fvec3& p2, const glm::vec3& color);
	void plotLineLow(int x0, int y0, int  x1, int y1, const glm::vec3& color, glm::vec3 p1, glm::vec3 p2);
	void plotLineHigh(int x0, int y0, int x1, int y1, const glm::vec3& color, glm::vec3 p1, glm::vec3 p2);
	void DrawTriangle(const glm::fvec3& v1, const glm::fvec3& v2, const glm::fvec3& v3, const glm::vec3& color);
	void DrawBoundingBox(MeshModel& model , const Scene& scene , glm::fmat4x4 trasformation , const glm::vec3& color);

	void DrawFaceNormal(MeshModel& mesh, glm::vec3 vectorArray[3], const Scene& scene , glm::fmat4x4 trasformation  , const glm::vec3& color);

	glm::vec3 DrawFaceNormal(MeshModel& mesh, Face& face , glm::fmat4x4 trasformation  , const glm::vec3& color);
	void DrawVerticesNormal(MeshModel& mesh, glm::fmat4x4 trasformation, const glm::vec3& color, float normalLength);
	void allocateZBuffer();

	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();

	void ScanConversionTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color);
	float getMin(float x, float y, float z);
	float getMax(float x, float y, float z);
	bool isInsideTheTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int x, int y);
	glm::vec3 getDirectionVector(glm::vec3 v1, glm::vec3 v2);
	float ZpointComputation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 insidePoint);
	float area(int x1, int y1, int x2, int y2, int x3, int y3);
	float* color_buffer_;
	int viewport_width_;
	int viewport_height_;
	float** Zbuffer;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
};
