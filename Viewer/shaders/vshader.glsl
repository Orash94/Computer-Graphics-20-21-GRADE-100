#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
//layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
//uniform mat4 model;	//inverse(modelWorldTransformation)*modelTransformation
//uniform mat4 view;	// viewVolumeTransformation
//uniform mat4 projection;	// AfterProjectTransforamtion * projectionTransformation

uniform mat4 finalTransformation;
uniform mat4 normalTransformation;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
//out vec2 fragTexCoords;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(finalTransformation * vec4(pos, 1.0f));

	//normal transformation
	vec3 temp = vec3(normalTransformation * vec4(normal,1.0f));
	vec3 pos_zero= vec3(normalTransformation * vec4(0.0f, 0.0f, 0.0f, 1.0f));
	fragNormal = temp -  pos_zero;

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	//fragTexCoords = texCoords;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = vec4(finalTransformation * vec4(pos, 1.0f));
//	gl_Position = gl_Position /gl_Position.w;
}