#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <iostream>
#include "glm/ext.hpp"
#include "Renderer.h"
#include "InitShader.h"
#include "Utils.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
	allocateZBuffer();
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
	delete[] Zbuffer;
}

void Renderer::PutPixel(int i, int j, const float z, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	if (Zbuffer[i][j] > z) {
		color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
		color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
		color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
		Zbuffer[i][j] = z;
	}
}

void Renderer::DrawLine(const glm::fvec3& p1, const glm::fvec3& p2, const glm::vec3& color)
{

	// TODO: Implement bresenham algorithm
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

	int x0 = (int)p1[0], y0 = (int)p1[1];
	int x1 = (int)p2[0], y1 = (int)p2[1];
	float z1 = p1[0], z2 = p2[2];
	if (std::abs(y1 - y0) < std::abs(x1 - x0)) {
		if (x0 > x1)
		{
			plotLineLow(x1, y1, x0, y0,color,p1,p2);
		}
		else
		{
			plotLineLow(x0, y0, x1, y1, color,p1,p2);
		}
	}
	else
	{
		if (y0 > y1) {
			plotLineHigh(x1,y1,x0,y0, color,p1,p2);
		}
		else
		{
			plotLineHigh(x0,y0,x1,y1, color,p1,p2);
		}
	}
}

void Renderer::plotLineLow(int x0, int y0,int  x1, int y1, const glm::vec3& color,glm::vec3 p1, glm::vec3 p2)
{
	int diffX = x1 - x0;
	int diffY = y1 - y0;

	int yi = 1;

	if (diffY < 0) {
		yi = -1;
		diffY = -diffY;
	}

	int d = (2 * diffY) - diffX;
	int y = y0;

	for (int x = x0;x <= x1; x++) {
		glm::vec3 v1v2DirectionVector = getDirectionVector(p1, p2);
		float insideX = x;
		if (insideX == 0)
		{
			insideX += 0.00001;
		}
		float alpha = p1[0] / insideX;
		glm::vec3 p4 = p1 + glm::vec3(alpha * v1v2DirectionVector[0], alpha * v1v2DirectionVector[1], alpha * v1v2DirectionVector[3]);
		float z = p4[2];

		PutPixel(x, y,z, color);
		if (d > 0) {
			y = y + yi;
			d = d + (2 * (diffY - diffX));
		}
		else
		{
			d = d + 2 * diffY;
		}
	}
}

void Renderer::plotLineHigh(int x0, int y0, int x1, int y1, const glm::vec3& color, glm::vec3 p1, glm::vec3 p2)
{
	int diffX = x1 - x0;
	int diffY = y1 - y0;

	int xi = 1;

	if (diffX < 0) {
		xi = -1;
		diffX = -diffX;
	}

	int d = (2 * diffX) - diffY;
	int x = x0;

	for (int y = y0; y <= y1; y++) {
		glm::vec3 v1v2DirectionVector = getDirectionVector(p1, p2);
		float insideX = x;
		if (insideX == 0)
		{
			insideX += 0.00001;
		}
		float alpha = p1[0] / insideX;
		glm::vec3 p4 = p1 + glm::vec3(alpha * v1v2DirectionVector[0], alpha * v1v2DirectionVector[1], alpha * v1v2DirectionVector[3]);
		float z = p4[2];
		PutPixel(x, y,z, color);
		if (d > 0) {
			x = x + xi;
			d = d + (2 * (diffX - diffY));
		}
		else
		{
			d = d + 2 * diffX;
		}
	}
}

void Renderer::DrawTriangle(const glm::fvec3& v1, const glm::fvec3& v2, const glm::fvec3& v3, const glm::vec3& color)
{
	DrawLine( v1, v2, glm::vec3(0,0,0));
	DrawLine(v2, v3, glm::vec3(0,0,0));
	DrawLine(v1, v3, glm::vec3(0, 0, 0));
	ScanConversionTriangle(v1, v2, v3, color);
}

void Renderer::DrawBoundingBox(MeshModel& model, const Scene& scene, glm::fmat4x4 trasformation , const glm::vec3& color)
{
	float deltaX = (model.maxX_ - model.minX_) /2;
	float deltaY = (model.maxY_ - model.minY_) / 2;
	float deltaZ = (model.maxZ_ - model.minZ_) / 2;
	glm::fvec3 vecArray[8] = {
	glm::fvec3(deltaX, deltaY, deltaZ),
	glm::fvec3(deltaX, deltaY, -deltaZ),
	glm::fvec3(deltaX, -deltaY, deltaZ),
	glm::fvec3(deltaX, -deltaY, -deltaZ),
	glm::fvec3(-deltaX, deltaY, deltaZ),
	glm::fvec3(-deltaX, deltaY, -deltaZ),
	glm::fvec3(-deltaX, -deltaY, deltaZ),
	glm::fvec3(-deltaX, -deltaY, -deltaZ)
	};
	
	for (int i = 0; i < 8; i++) {
		vecArray[i] =  Utils::applyTransformationToVector(vecArray[i], trasformation);
	}
	/*cubes look like this:
	   e-------f
	  /|      /|
	 / |#1   / |
	a--|----b  |
	|  g----|--h
	| / #2   | /
	c-------d*/

	//face #1 up
	DrawLine(vecArray[0], vecArray[1], color);
	DrawLine(vecArray[0], vecArray[2], color);
	DrawLine(vecArray[3], vecArray[1], color);
	DrawLine(vecArray[3], vecArray[2], color);

	//face #2 down
	DrawLine(vecArray[7], vecArray[6], color);
	DrawLine(vecArray[7], vecArray[5], color);
	DrawLine(vecArray[4], vecArray[6], color);
	DrawLine(vecArray[4], vecArray[5], color);

	//connectors of two faces
	DrawLine(vecArray[0], vecArray[4], color);
	DrawLine(vecArray[1], vecArray[5], color);
	DrawLine(vecArray[2], vecArray[6], color);
	DrawLine(vecArray[3], vecArray[7], color);

}

glm::vec3 Renderer::DrawFaceNormal(MeshModel& mesh, Face& face, glm::fmat4x4 trasformation, const glm::vec3& color)
{
		glm::vec3 vectorArray[3];

		for (int k = 0; k < 3; k++) {
			int index = face.GetVertexIndex(k) - 1;
			vectorArray[k] = mesh.GetVertexAtIndex(index);
		}

		glm::fvec3 v0 = Utils::applyTransformationToVector(vectorArray[0], trasformation);
		glm::fvec3 v1 = Utils::applyTransformationToVector(vectorArray[1], trasformation);
		glm::fvec3 v2 = Utils::applyTransformationToVector(vectorArray[2], trasformation);
		

	
		float EdgeLength = glm::distance(v0, v1) / 2; //normals length
		glm::fvec3 ActualCenter = (v0 + v1 + v2) / 3.0f; //center of face
		glm::vec3 Actualnormal = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		glm::vec3 normalizedNormal = Actualnormal;

		Actualnormal = Actualnormal * EdgeLength;
		//face normals check
		if (mesh.displayFaceNormals) {
			DrawLine(ActualCenter, ActualCenter + Actualnormal, glm::fvec3(1, 1, 1));
		}
		return normalizedNormal;
}

void Renderer::DrawVerticesNormal(MeshModel& mesh, glm::fmat4x4 trasformation, const glm::vec3& color , float normalLength)
{
	for (int i = 0; i < mesh.GetVerticesCount(); i++) {
		glm::fvec3 v = mesh.GetVertexAtIndex(i);
		v = Utils::applyTransformationToVector(v, trasformation);

		glm::fvec3 vn = mesh.getVerticesNormals()[i];
		vn = vn * normalLength;
		DrawLine(v, v + vn, color);
	}
	
}

void Renderer::allocateZBuffer()
{
	Zbuffer = new float* [viewport_height_];
	for (int i = 0; i < viewport_height_; i++)
		Zbuffer[i] = new float[viewport_width_];
	for (int i = 0; i < viewport_height_; i++)
		for (int j = 0; j < viewport_width_; j++)
			Zbuffer[i][j] = FLT_MAX;
}

void Renderer::ScanConversionTriangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color)
{
	float minX = getMin(p1.x, p2.x, p3.x);
	float maxX = getMax(p1.x, p2.x, p3.x);
	float minY = getMin(p1.y, p2.y, p3.y);
	float maxY = getMax(p1.y, p2.y, p3.y);

	for (int i = minX; i < maxX; i++) {
		for (int j = minY; j < maxY; j++) {
			if (isInsideTheTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, i, j))
				//if (Zbuffer[i][j] >= ZpointComputation(p1, p2, p3, glm::vec2(i, j))) {
					PutPixel(i, j, ZpointComputation(p1, p2, p3, glm::vec2(i, j)), color);
				//	Zbuffer[i][j] = ZpointComputation(p1, p2, p3, glm::vec2(i, j));
				//}
		}

	}

}

float Renderer::getMin(float x, float y, float z)
{
	float temp;
	if (x < y)
		temp = x;
	else
	{
		temp = y;
	}
	if (z < temp)
		temp = z;
	return temp;
}

float Renderer::getMax(float x, float y, float z)
{
	float temp;
	if (x > y)
		temp = x;
	else
	{
		temp = y;
	}
	if (z > temp)
		temp = z;
	return temp;
}

/*
* Checking on which side of the half-plane created by the edges the point is
*/
bool Renderer::isInsideTheTriangle(float x1, float y1, float x2, float y2, float x3, float y3, int x, int y)
{


	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = area(x,y,x1,y1, x2,y2);
	d2 = area(x,y, x2,y2, x3,y3);
	d3 = area(x,y, x3,y3, x1,y1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

glm::vec3 Renderer::getDirectionVector(glm::vec3 v1, glm::vec3 v2)
{
	glm::vec3 directionVector = v1 - v2;
	float min = getMin(std::abs(directionVector.x), std::abs(directionVector.y), std::abs(directionVector.z));
	if (min == 0)
		return directionVector;
	directionVector = glm::vec3(directionVector[0] / min, directionVector[1] / min, directionVector[2] / min);
	return directionVector;
}

float Renderer::ZpointComputation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 insidePoint)
{
	glm::vec3 v1v2DirectionVector = getDirectionVector(p1, p2);
	float insideX = insidePoint[0];
	if (insideX == 0)
	{
		insideX += 0.00001;
	}
	float alpha = p1[0]/insideX;
	glm::vec3 p4 = p1 + glm::vec3(alpha*v1v2DirectionVector[0], alpha * v1v2DirectionVector[1], alpha * v1v2DirectionVector[3]) ;
	float z4 = p4[2];

	glm::vec3 v1v3DirectionVector = getDirectionVector(p1, p3);
	
	alpha = p1[0] / insideX;
	glm::vec3 p5 = p1 + glm::vec3(v1v3DirectionVector[0] * alpha, v1v3DirectionVector[1] * alpha, v1v3DirectionVector[2] * alpha);
	float z5 = p5[2];

	glm::vec3 v4v5DirectionVector = getDirectionVector(p4, p5);
	alpha = p4[0] / insideX;
	glm::vec3 p = p4 + glm::vec3(alpha * v4v5DirectionVector[0], alpha * v4v5DirectionVector[1], alpha * v4v5DirectionVector[2]);
	float z = p[2];


	return z;
}

float Renderer::area(int x1, int y1, int x2, int y2, int x3, int y3)
{
	
	return (x1 - x3) * (y2- y3) - (x2 - x3) * (y1 - y3);

}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	allocateZBuffer();
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}



void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			Zbuffer[i][j] = FLT_MAX;
			PutPixel(i, j,1001, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	int windowsWidth = viewport_width_;
	int windowsHeight = viewport_height_;
	//allocateZBuffer();

	int centerX = windowsWidth / 2;
	int centerY = windowsHeight / 2;
	int boundingBoxEdgeLength = glm::min(centerX, centerY);

	
	if (scene.getShowAxis()) {

		DrawLine(glm::fvec3(0, centerY, FLT_MIN), glm::fvec3(windowsWidth, centerY, FLT_MIN), glm::fvec3(0, 0, 0));
		DrawLine(glm::fvec3(centerX, 0, FLT_MIN), glm::fvec3(centerX, windowsHeight, FLT_MIN), glm::fvec3(0, 0, 0));

	}
	
	//rendering the MeshModels
	if (scene.GetModelCount() > 0) {
		for (int i = 0; i < scene.GetModelCount(); i++)
		{
			MeshModel& mesh = scene.GetModel(i);
			float proportion = 400.0f/mesh.getMaxDitancePoints();

			
			glm::fmat4x4 scale = Utils::TransformationScale(glm::fvec3(proportion, proportion, proportion));
			glm::fmat4x4 translate = Utils::TransformationTransition(glm::fvec3(centerX, centerY, 0));
			glm::fmat4x4 transformationMatrix = glm::inverse(mesh.getWorldTransformation()) * mesh.getObjectTransformation();


			glm::fmat4x4 finalTransformation =   transformationMatrix * scale   ;

			if (scene.GetCamOrWorldView())  // rendering the active camera view
			{
				Camera& currentCam = scene.GetActiveCamera();
				glm::fmat4x4 inverserCameraTransformation = glm::lookAt(currentCam.getEye(), currentCam.getAt(), currentCam.getUp());
				glm::fmat4x4 viewVolumeTransformation;

				viewVolumeTransformation = currentCam.GetViewTransformation();
				glm::fmat4x4 CameraTransformation = viewVolumeTransformation * inverserCameraTransformation;
				finalTransformation = CameraTransformation * finalTransformation;
				
			}

			//transfer objects to center screen with transalte transformation
			finalTransformation = translate * finalTransformation;
			//bounding box check
			if (mesh.displayBoundingBox) {
				DrawBoundingBox(mesh, scene, finalTransformation, glm::vec3(0, 0, 1));
			}

			std::vector<Face> faces = mesh.getFaces();

			//draw faces
			for (int j = 0; j < mesh.GetFacesCount(); j++)
			{
				Face& face = faces[j];

				glm::vec3 vectorArray[3];

				//extract verices of face
				for (int k = 0; k < 3; k++) {
					int index = face.GetVertexIndex(k) - 1;
					glm::vec3 v =mesh.GetVertexAtIndex(index);
					vectorArray[k] = Utils::applyTransformationToVector(v , finalTransformation);
				}

				
				glm::fvec3  faceNormal = DrawFaceNormal(mesh ,face, finalTransformation, glm::vec3(1, 0, 1));
				mesh.setFaceNormal(j ,faceNormal);
				


				DrawTriangle(vectorArray[0], vectorArray[1], vectorArray[2], glm::vec3(1, 0, 0));
			}

			//call function 
			mesh.setVerteciesNormals();

			//vertices normals check
			if (mesh.displayVerticesNormals) {
				DrawVerticesNormal(mesh, finalTransformation, glm::vec3(0, 0, 0.545), 40.0f);
			}
		}

	}

	//rendering the  camers to the screen
	if (scene.GetCameraCount() > 0 && !scene.GetCamOrWorldView()) {
		for (int i = 0; i < scene.GetCameraCount(); i++) {
			Camera& tempCam = scene.GetCamera(i);
			float proportion = 100.0f / tempCam.getMaxDitancePoints();

			glm::fmat4x4 scale = Utils::TransformationScale(glm::fvec3(proportion, proportion, proportion));
			glm::fmat4x4 translate = Utils::TransformationTransition(glm::fvec3(centerX, centerY, 0));

			glm::fmat4x4 transformationMatrix;

			//check if we are using LookAt or Transformation
			if (tempCam.GetLookAtOrTransformation() == true) {
				 transformationMatrix = glm::inverse(tempCam.getWorldTransformation()) * tempCam.getObjectTransformation();
			}
			else {
				transformationMatrix =  glm::inverse(glm::lookAt(tempCam.getEye(), tempCam.getAt(), tempCam.getUp()));
			}

			glm::fmat4x4 finalTransformation = translate * transformationMatrix * scale;

			std::vector<Face> faces = tempCam.getFaces();

			for (int j = 0; j < tempCam.GetFacesCount(); j++)
			{
				Face& face = faces[j];

				glm::vec3 vectorArray[3];

				for (int k = 0; k < 3; k++) {
					int index = face.GetVertexIndex(k) - 1;
					glm::vec3 v = tempCam.GetVertexAtIndex(index);
					vectorArray[k] = Utils::applyTransformationToVector(v, finalTransformation);
				}


				DrawTriangle(vectorArray[0], vectorArray[1], vectorArray[2], mesh.GetColor());
				
			}

			//call function 
			mesh.setVerteciesNormals();

			//vertices normals check
			if (mesh.displayVerticesNormals) {
				DrawVerticesNormal(mesh, finalTransformation, glm::vec3(0, 0, 0.545), 40.0f);
			}
		}

	}

	//rendering the  camers to the screen
	if (scene.GetCameraCount() > 0 && !scene.GetCamOrWorldView()) {
		for (int i = 0; i < scene.GetCameraCount(); i++) {
			Camera& tempCam = scene.GetCamera(i);
			float proportion = 100.0f / tempCam.getMaxDitancePoints();

			glm::fmat4x4 scale = Utils::TransformationScale(glm::fvec3(proportion, proportion, proportion));
			glm::fmat4x4 translate = Utils::TransformationTransition(glm::fvec3(centerX, centerY, 0));

			glm::fmat4x4 transformationMatrix;

			//check if we are using LookAt or Transformation
			if (tempCam.GetLookAtOrTransformation() == true) {
				 transformationMatrix = glm::inverse(tempCam.getWorldTransformation()) * tempCam.getObjectTransformation();
			}
			else {
				transformationMatrix =  glm::inverse(glm::lookAt(tempCam.getEye(), tempCam.getAt(), tempCam.getUp()));
			}

			glm::fmat4x4 finalTransformation = translate * transformationMatrix * scale;

			std::vector<Face> faces = tempCam.getFaces();

			for (int j = 0; j < tempCam.GetFacesCount(); j++)
			{
				Face& face = faces[j];

				glm::vec3 vectorArray[3];

				for (int k = 0; k < 3; k++) {
					int index = face.GetVertexIndex(k) - 1;
					glm::vec3 v = tempCam.GetVertexAtIndex(index);
					vectorArray[k] = Utils::applyTransformationToVector(v, finalTransformation);
				}

				DrawTriangle(vectorArray[0], vectorArray[1], vectorArray[2], glm::vec3(0, 0, 0));
				
			}
		}
	}
	
	

}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}

void Renderer::SetViewport(int width, int height)
{
	viewport_height_ = height;
	viewport_width_ = width;
	CreateBuffers(width, height);
}
