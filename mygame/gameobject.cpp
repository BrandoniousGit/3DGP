#include "gameobject.h"
#include <stdexcept>

//#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

GameObject::GameObject(const std::string& _modelPath, Shader _shader, glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale)
{
	if (WfModelLoad(_modelPath.c_str(), &objectModel) != 0)
	{
		throw std::runtime_error("game brokey");
	}

	position = _pos;
	rotation = _rot;
	scale = _scale;

	projectionLoc = glGetUniformLocation(_shader.getProgId(), "u_Projection");
	viewLoc = glGetUniformLocation(_shader.getProgId(), "u_View");
	modelLoc = glGetUniformLocation(_shader.getProgId(), "u_Model");
}

void GameObject::SetTexture(const std::string& _texturePath)
{
	int w = 0;
	int h = 0;

	//loading the texture file and returning the width and height.
	unsigned char* data = stbi_load(_texturePath.c_str(), &w, &h, NULL, 4);

	//checking the file loaded correctly
	if (!data)
	{
		throw std::runtime_error("texture not loaded");
	}

	//generating the texture and setting the ID
	GLuint textId = 0;
	glGenTextures(1, &textId);

	//checking the texture initialised correctly
	if (!textId)
	{
		throw std::exception();
	}

	//binding the texture to the current object
	glBindTexture(GL_TEXTURE_2D, textId);

	//attaching the texture image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	//clearing the texture image as its no longer needed
	free(data);

	//creating a mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	//unbinding the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//returning the texture ID
	objectModel.textureId = textId;
}

void GameObject::DrawGameObject(Shader& _shader, SDL_Window* window, glm::vec3 lightPos, glm::mat4 _viewMatrix)
{
	SDL_GetWindowSize(window, &w, &h);

	_shader.Use();

	glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)w / (float)h, 0.1f, 1000.0f);
	glm::mat4 model = glm::mat4(1.0f);

	rotationQuat = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);
	rotationQuat = glm::angleAxis(glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) * rotationQuat;
	rotationQuat = glm::angleAxis(glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) * rotationQuat;
	rotationQuat = glm::angleAxis(glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) * rotationQuat;

	glm::mat4 rotationMat = glm::mat4(1.0f), translationMat = glm::mat4(1.0f), scaleMat = glm::mat4(1.0f);
	translationMat = glm::translate(translationMat, position);
	rotationMat = glm::mat4_cast(rotationQuat);
	scaleMat = glm::scale(scaleMat, scale);

	model = translationMat * rotationMat * scaleMat;

	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(objectModel.vaoId);

	glBindTexture(GL_TEXTURE_2D, objectModel.textureId);

	glDrawArrays(GL_TRIANGLES, 0, objectModel.vertexCount);

	_shader.NoUse();
}

void GameObject::Rotate(glm::vec3 _rot)
{
	rotation += _rot;
}

void GameObject::Translate(glm::vec3 _pos) 
{
	position += _pos;
}