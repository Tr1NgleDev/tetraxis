#pragma once

#include <basicIncludes.h>
#include <QuadRenderer.h>
#include <Shader.h>
#include <Texture.h>

class TexRenderer
{
protected:
	glm::vec2 pos{ 0 };
	glm::vec2 scale{ 1 };
	float angle = 0;
	glm::vec2 rotOrigin{ 0.5f };
	glm::vec2 scaleOrigin{ 0.5f };
	glm::mat4 model{ 1 };
	bool updateModel = true;
	Texture* texture = nullptr;
public:
	Shader* shader = nullptr;

	TexRenderer(Texture* texture = nullptr, Shader* shader = nullptr);
	void setTexture(Texture* texture) { this->texture = texture; }
	void setPos(const glm::vec2& pos);
	void setPos(float x, float y) { setPos({ x, y }); }
	void setScale(const glm::vec2& scale);
	void setScale(float x, float y) { setScale({ x, y }); }
	void setAngle(float angle);
	void setRotOrigin(const glm::vec2& rotOrigin);
	void setRotOrigin(float x, float y) { setRotOrigin({ x, y }); }
	void setScaleOrigin(const glm::vec2& scaleOrigin);
	void setScaleOrigin(float x, float y) { setScaleOrigin({ x, y }); }
	glm::vec2 getPos() const { return pos; }
	glm::vec2 getScale() const { return scale; }
	float getAngle() const { return angle; }
	glm::vec2 getRotOrigin() const { return rotOrigin; }
	glm::vec2 getScaleOrigin() const { return scaleOrigin; }
	void render();
};
