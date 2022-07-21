#pragma once
#include <wavefront/wavefront.h>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/ext.hpp>
#include <SDL2/SDL.h>
#include "shader.h"

class GameObject
{
public:
	GameObject(const std::string& _modelPath, Shader _shader, glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale);

	void DrawGameObject(Shader& _shader, SDL_Window* window, glm::vec3 lightPos, glm::mat4 _viewMatrix);
	void Rotate(glm::vec3 _rot);
	void Translate(glm::vec3 _pos);

	glm::vec3 GetRotation() { return rotation; };
	void SetRotation(glm::vec3 _rot) { rotation = _rot; };

	glm::vec3 GetPosition() { return position; };
	void SetPosition(glm::vec3 _pos) { position = _pos; };

	void SetTexture(const std::string& _texturePath);

private:

	WfModel objectModel = { 0 };
	glm::vec3 rotation = glm::vec3(0.0f), position = glm::vec3(0.0f), scale = glm::vec3(0.0f);
	glm::quat rotationQuat;

	int w, h;

	GLuint projectionLoc;
	GLuint modelLoc;
	GLuint viewLoc;
};