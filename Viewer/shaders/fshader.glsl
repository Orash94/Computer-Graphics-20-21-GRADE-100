#version 330 core

//uniform sampler2D texture;

struct Material
{
	//sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant

	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecualrColor;
};

// We set this field's properties from the C++ code
uniform Material material;


uniform vec4 modelColor;
uniform vec4 wirmframe_color;
uniform vec3 eye;

uniform vec4 lightPos [10];

uniform vec3 lightAmbientColors[10];
uniform vec3 lightDiffuseColors[10];
uniform vec3 lightSpecularColors[10];
uniform vec4 lightSpecularColorsAlpha[10];


uniform vec4 lightType [10];
uniform int lightsCount;

//in vec2 fragTexCoords;
in vec3 orig_fragPos;
// Inputs from vertex shader (after interpolation was applied)
in vec4 fragPos;
in vec4 fragNormal;

// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	//vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	//frag_color = vec4(orig_fragPos,1);
	//frag_color = vec4(1,0,0,1);


	vec3 AmbientColor = material.AmbientColor;
	vec3 DiffuseColor = material.DiffuseColor;
	vec3 SpecualrColor = material.SpecualrColor;

	vec3 N = normalize(fragNormal.xyz / fragNormal.w);		//normal of point 

	
	vec3 IA = vec3(0.0f);
	vec3 ID = vec3(0.0f);
	vec3 IS = vec3(0.0f);

	for (int i=0; i<lightsCount; i++) {
		vec3 LightDirection ;
		if(lightType[i] == vec4(0)){
			// point 
			LightDirection = (lightPos[i].xyz /lightPos[i].w) - (fragPos.xyz/fragPos.w) ;
		}else{
			//parallel
			LightDirection = (lightPos[i].xyz/lightPos[i].w) ;
		}
		vec3 Eye  =  -(fragPos.xyz /fragPos.w) ; // if we assume eye is at (0,0,0)
		vec3 Reflection = normalize(-reflect(LightDirection,N));

		IA = AmbientColor * lightAmbientColors[i];


		ID = DiffuseColor * lightDiffuseColors[i] * max(dot(N,LightDirection),0.0);
		ID = clamp(ID, 0.0, 1.0);

		IS = SpecualrColor* lightSpecularColors[i] * pow(max(dot(Reflection,Eye),0.0),lightSpecularColorsAlpha[i][0]) ;
		IS = clamp(IS, 0.0, 1.0); 
	}
	if (lightsCount != 0)
		frag_color = vec4(IA + ID + IS,1) ;
	else
		frag_color = modelColor;


}


