#version 330 core

uniform sampler2D texture;

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};

// We set this field's properties from the C++ code
uniform Material material;

//in vec2 fragTexCoords;
in vec3 orig_fragPos;
// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;

// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	//vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	//frag_color = vec4(orig_fragPos,1);
	frag_color = vec4(1,0,0,1);

}


