#version 330

smooth in vec3 theNormal;
smooth in vec3 theLight;
smooth in vec2 theTexture;

uniform sampler2D texSampler;

out vec4 outputColor;

void main()
{
	vec4 vTexColor = texture2D(texSampler, theTexture);
	vec3 color = vec3(0.5, 0.5, 0.5);
	float ambient = 0.1;
	float fDiffuseIntensity = max(0.0, dot(normalize(theNormal), -normalize(theLight))); 
	//outputColor = vec4(color * (fDiffuseIntensity + ambient), 1.0);
	outputColor = vTexColor;// * (fDiffuseIntensity + ambient);
}