#pragma once

#include "Shader.h"

class ShaderProgram
{
public:
	ShaderProgram();
	bool attachShader(Shader *shader);
	bool link();
	void use();

	GLuint getId();
	~ShaderProgram(void);

private: 
	GLuint mId;
	bool mLinked;
};

