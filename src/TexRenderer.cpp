#include <TexRenderer.h>

TexRenderer::TexRenderer(Texture* texture, Shader* shader)
	: texture(texture), shader(shader) { }

void TexRenderer::render()
{
	if (!texture || !shader) return;

	const auto& texSize = texture->getSize();
	glm::vec2 texSizeV{ texSize[0], texSize[1] };
	glm::vec2 fullSize = scale * texSizeV;

	if (updateModel)
	{
		updateModel = false;

		model = glm::translate(glm::mat4{ 1 }, glm::vec3{pos, 0.0});

		model = glm::translate(model, glm::vec3{ scaleOrigin * texSizeV, 0.0 });
		model = glm::scale(model, glm::vec3{ scale, 1.0 });
		model = glm::translate(model, glm::vec3{ -scaleOrigin * texSizeV, 0.0 });

		model = glm::translate(model, glm::vec3{ rotOrigin * texSizeV, 0.0 });
		model = glm::rotate(model, glm::radians(angle), { 0,0,1 });
		model = glm::translate(model, glm::vec3{ -rotOrigin * texSizeV, 0.0 });

		model = glm::scale(model, glm::vec3{ texSizeV, 1.0 });
	}

	shader->use();
	shader->setUniform("model", model);
	texture->use();
	QuadRenderer::render();
}

void TexRenderer::setPos(const glm::vec2& pos)
{
	if (this->pos == pos) return;
	this->pos = pos;
	updateModel = true;
}
void TexRenderer::setScale(const glm::vec2& scale)
{
	if (this->scale == scale) return;
	this->scale = scale;
	updateModel = true;
}
void TexRenderer::setAngle(float angle)
{
	if (this->angle == angle) return;
	this->angle = angle;
	updateModel = true;
}
void TexRenderer::setRotOrigin(const glm::vec2& rotOrigin)
{
	if (this->rotOrigin == rotOrigin) return;
	this->rotOrigin = rotOrigin;
	updateModel = true;
}
void TexRenderer::setScaleOrigin(const glm::vec2& scaleOrigin)
{
	if (this->scaleOrigin == scaleOrigin) return;
	this->scaleOrigin = scaleOrigin;
	updateModel = true;
}
