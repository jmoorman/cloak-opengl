#include "stdafx.h"

#include "TextureLoader.h"

const std::string TextureLoader::TEXTURE_PATH = "../data/textures/";

TextureLoader::TextureLoader(void)
{
}


TextureLoader::~TextureLoader(void)
{
	for(auto it = mTextureMap.begin(); it != mTextureMap.end(); ++it) {
		glDeleteTextures(1, &it->second);
	}
}

GLuint TextureLoader::GetTexture(const std::string& filename) {
	using boost::filesystem3::path;
	using boost::filesystem3::exists;
	using boost::filesystem3::is_directory;
	using boost::filesystem3::is_regular_file;

	
	if(mTextureMap.count(filename) != 0) {
		return mTextureMap[filename];
	}

	SDL_Surface *source = NULL;

	std::cout << "Loading texture " << TEXTURE_PATH + filename << std::endl;
	path filePath(TEXTURE_PATH);
	if(!exists(filePath) || !is_directory(filePath)) {
		return 0;
	}
	
	path file(TEXTURE_PATH + filename);
	if(!exists(file) || !is_regular_file(file)) {
		return 0;
	}

	source = IMG_Load((TEXTURE_PATH + filename).c_str());
	if(source == NULL) {
		std::cout << IMG_GetError();
		return 0;
	}

	SDL_Surface *image = SDL_CreateRGBSurface(
			SDL_SWSURFACE, source->w, source->h, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF,
			0x0000FF00,
			0x00FF0000,
			0xFF000000
#else
			0xFF000000,
			0x00FF0000,
			0x0000FF00,
			0x000000FF
#endif
			);
	SDL_Rect area;
	area.x = 0;
	area.y = 0;
	area.w = source->w;
	area.h = source->h;
	SDL_BlitSurface(source, &area, image, &area);
	SDL_FreeSurface(source);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, image->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(image);
	mTextureMap[filename] = texture;
	return texture;
}
