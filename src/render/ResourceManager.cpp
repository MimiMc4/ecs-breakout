// -----------------------------------------------------------------------------
// ORIGIN: Code adapted from LearnOpenGL.com
// -----------------------------------------------------------------------------

/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include <render/ResourceManager.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;
ma_engine							ResourceManager::audioEngine;
std::map<std::string, ma_sound*>		ResourceManager::Sounds;

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::initAudio() {
	ma_engine_config config = ma_engine_config_init();
	config.channels = 2;  // Estéreo
	config.sampleRate = 48000;
	config.periodSizeInFrames = 256;

	if (ma_engine_init(&config, &audioEngine) != MA_SUCCESS) {
		std::cerr << "Error al iniciar el motor de audio.\n";
	}
	ma_engine_set_volume(&audioEngine, 0.7f);
}

ma_sound* ResourceManager::LoadSound(const char* file, std::string name, bool loopable) {
	Sounds[name] = loadSoundFromFile(file, loopable);
	return Sounds[name];
}

ma_sound* ResourceManager::GetSound(std::string name) {
	return Sounds[name];
}

void ResourceManager::Clear()
{
	// (properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);

	// Limpiar sonidos (¡IMPORTANTE: Liberar memoria del heap!)
	for (auto& iter : Sounds) {
		// iter tiene campos first y second que apuntan a cada elemento de la tupla
		if (iter.second) {  // Verificar que el puntero no sea nullptr
			ma_sound_stop(iter.second);      
			ma_sound_uninit(iter.second);   
			delete iter.second;               
			iter.second = nullptr;            
		}
	}
	Sounds.clear();

	// Opcional: Detener y liberar el motor de audio
	ma_engine_uninit(&audioEngine);

}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	// 2. now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
	// create texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// load image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	// now generate texture
	texture.Generate(width, height, data);
	// and finally free image data
	stbi_image_free(data);
	return texture;
}

ma_sound* ResourceManager::loadSoundFromFile(const char* file, bool loopable) {
	ma_sound* sound = new ma_sound();

	if (ma_sound_init_from_file(&audioEngine, file, 0, NULL, NULL, sound) != MA_SUCCESS) {
		delete sound;
		std::cerr << "Error al cargar el sonido:\n" << file;
	}

	if (loopable) {
		ma_sound_set_looping(sound, MA_TRUE);
	}

	return sound;
}
