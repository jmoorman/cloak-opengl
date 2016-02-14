#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;
uniform vec3 lightPosition;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

smooth out vec3 theNormal;
smooth out vec3 theLight;
smooth out vec2 theTexture;

void main()
{
	mat4 MVP = projectionMatrix * modelViewMatrix;
	gl_Position = MVP * vec4(inPosition, 1.0);
	vec4 vRes = normalMatrix*vec4(inNormal, 0.0);
    theNormal = vRes.xyz; 
	theLight = lightPosition;
	theTexture = inTexCoord;
}