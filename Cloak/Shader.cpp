#include "stdafx.h"
#include "Shader.h"


Shader::Shader(std::string file, GLenum type)
		: mFile(std::move(file)), mType(type), mValid(false) {
	mId = glCreateShader(mType);
}

bool Shader::compile() {
	FILE* fp = fopen(mFile.c_str(), "rt");
	if (fp == NULL) {
		return false;
	}

	std::vector<std::string> lines;
	char line[255];
	while(fgets(line, 255, fp)) {
		lines.push_back(line);
	}
	fclose(fp);

	const char **program = (const char **)malloc(sizeof(char *) * lines.size());
	for(int i = 0; i < lines.size(); i++) {
		program[i] = lines[i].c_str();
	}

	glShaderSource(mId, lines.size(), program, NULL);
	glCompileShader(mId);
	free(program);
	program = NULL;

	int compileStatus;
	glGetShaderiv(mId, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE) {
		return false;
	}
	mValid = true;
	return true;
}

bool Shader::isValid() {
	return mValid;
}

GLenum Shader::getId() {
	return mId;
}

Shader::~Shader(void)
{
	glDeleteShader(mId);
}
