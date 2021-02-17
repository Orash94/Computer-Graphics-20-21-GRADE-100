#version 330 core

//uniform sampler2D texture;

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant

	vec4 AmbientColor;
	vec4 DiffuseColor;
	vec4 SpecualrColor;

	float KA;
	float KD;
	float KS;
	int KSE;
};

// We set this field's properties from the C++ code
uniform Material material;


uniform vec4 modelColor;
uniform vec4 wirmframe_color;
uniform vec3 eye;

uniform vec4 SceneAmbient;
uniform vec4 lightPos [10];
uniform vec4 lightColor [10];
uniform vec4 lightType [10];
uniform int lightsCount;
uniform int isTexture;

in vec2 fragTexCoords;
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


	vec4 AmbientColor = material.AmbientColor;
	vec4 DiffuseColor = material.DiffuseColor;
	vec4 SpecualrColor = material.SpecualrColor;

	vec3 N = normalize(fragNormal.xyz / fragNormal.w);		//normal of point 
	vec3 V = normalize(eye - (fragPos.xyz / fragPos.w));	//camera direction

	
	vec4 IA = vec4(0.0f);
	vec4 ID = vec4(0.0f);
	vec4 IS = vec4(0.0f);

	for (int i=0; i<lightsCount; i++) {
		// ambient is only needed once
		IA = clamp(AmbientColor, 0.0f, 1.0f);

		vec4 lightColor = lightColor[i];
		vec3 pos = lightPos[i].xyz / lightPos[i].w;

		vec3 L;
		vec3 R;							//reflected light direction
		if(lightType[i] == vec4(0)){
			// point 
			L = normalize(pos - (fragPos.xyz / fragPos.w));
			//R = normalize(reflect(L, N));	
			ID = clamp( dot(N, L)* DiffuseColor, 0.0f, 1.0f);
			R = normalize(reflect(L, N));
		}else{
			//parallel
			L = normalize(pos );
			ID = clamp( dot(N, -L)* DiffuseColor, 0.0f, 1.0f);	//pos here is treated as light direction
			R = normalize(reflect(-L, N));
				
		}

		

			


		float RV = max(dot(R, V), 0.0f);
		vec4 sc = material.KS * pow(RV,material.KSE) * SpecualrColor;
		IS = IS + clamp(sc*lightColor, 0.0f, 1.0f);

	}
	if (lightsCount != 0){
		frag_color = clamp(IA + ID + IS , 0.0f, 1.0f);
		if(isTexture){
			vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
			vec4 AmbientColor = vec4(textureColor, 1.0f);
			vec4 DiffuseColor = vec4(textureColor, 1.0f);
			vec4 SpecualrColor = vec4(textureColor, 1.0f);
			frag_color = vec4(textureColor, 1.0f);
		}
	}
	else
	{
		if(isTexture){
			vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
			modelColor = vec4(textureColor, 1.0f);
			frag_color = modelColor;
		}
	}


}


