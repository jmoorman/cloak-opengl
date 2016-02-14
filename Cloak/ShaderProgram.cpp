#include "stdafx.h"
#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(void) : mLinked(false)
{
	mId = glCreateProgram();
}

bool ShaderProgram::attachShader(Shader *shader) {
	if (!shader->isValid()) {
		return false;
	}
	glAttachShader(mId, shader->getId());
	return true;
}

bool ShaderProgram::link() {
	glLinkProgram(mId);
	int linkStatus;
	glGetProgramiv(mId, GL_LINK_STATUS, &linkStatus);
	mLinked = (linkStatus == GL_TRUE);
	return mLinked;
}

void ShaderProgram::use() {
	if(mLinked) {
		glUseProgram(mId);
	}
}

GLuint ShaderProgram::getId() {
	return mId;
}

ShaderProgram::~ShaderProgram(void)
{
	glDeleteProgram(mId);
}
