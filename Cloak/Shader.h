#pragma once

#include "stdafx.h"

class Shader
{
public:
	//inititalize shader with filename and shader type
	Shader(std::string file, GLenum type);

	//compiles the shader and returns true if successful
	bool compile();

	//returns the cached result of the previous compile() call
	bool isValid();

	GLenum getId();

	//destroy shader
	~Shader();

private:
	std::string mFile;
	GLenum mType;
	GLuint mId;
	bool mValid;
};

