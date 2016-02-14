#pragma once

#include "stdafx.h"

class TextureLoader
{
public:
	TextureLoader(void);
	~TextureLoader(void);
	
	GLuint GetTexture(const std::string& filename);

	static const std::string TEXTURE_PATH;
private:

	std::map<std::string, GLuint> mTextureMap;
};

