#include <SVGRenderer.h>

resvg_options* SVGRenderer::opt = nullptr;

SVGRenderer::SVGRenderer(const std::string& svg, Shader* shader)
{
	this->shader = shader;
	setSVG(svg);
}

void SVGRenderer::setSVG(const std::string& svg)
{
	if (!opt)
	{
		opt = resvg_options_create();
		resvg_options_load_system_fonts(opt);
	}

	if (this->svg)
		resvg_tree_destroy(this->svg);

	int err = resvg_parse_tree_from_file(svg.c_str(), opt, &this->svg);

	if (err)
		this->svg = nullptr;

	updateModel = true;
	texture = &svgTex;
}

void SVGRenderer::setSVGQuality(float quality)
{
	svgQuality = quality;
	updateModel = true;
}

void SVGRenderer::render()
{
	if (!texture || !shader) return;

	if (updateModel && svg)
	{
		updateModel = false;

		resvg_size svgSize = resvg_get_image_size(svg);
		glm::vec2 svgSizeV{ svgSize.width, svgSize.height };
		glm::vec2 fullSize
		{
			glm::ceil(svgSize.width * scale.x * svgQuality),
			glm::ceil(svgSize.height * scale.y * svgQuality),
		};

		model = glm::translate(glm::mat4{ 1 }, glm::vec3{ pos, 0.0 });

		model = glm::translate(model, glm::vec3{ scaleOrigin * svgSizeV, 0.0 });
		model = glm::scale(model, glm::vec3{ scale, 1.0 });
		model = glm::translate(model, glm::vec3{ -scaleOrigin * svgSizeV, 0.0 });

		model = glm::translate(model, glm::vec3{ rotOrigin * svgSizeV, 0.0 });
		model = glm::rotate(model, glm::radians(angle), { 0,0,1 });
		model = glm::translate(model, glm::vec3{ -rotOrigin * svgSizeV, 0.0 });

		model = glm::scale(model, glm::vec3{ svgSizeV, 1.0 });

		if ((svgTex.getSize()[0] != (int)fullSize.x || svgTex.getSize()[1] != (int)fullSize.y)
			&& (int)fullSize.x > 0 && (int)fullSize.y > 0)
		{
			char* data = new char[(int)fullSize.x * (int)fullSize.y * 4](0);

			auto transform = resvg_transform_identity();
			transform.a = (int)fullSize.x / svgSize.width;
			transform.d = (int)fullSize.y / svgSize.height;

			resvg_render(svg, transform, (int)fullSize.x, (int)fullSize.y, (char*)data);

			svgTex.init(data, Texture::Format{ GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE }, (int)fullSize.x, (int)fullSize.y, true);

			delete[] data;
		}
	}

	shader->use();
	shader->setUniform("model", model);
	texture->use();
	QuadRenderer::render();
}

SVGRenderer::~SVGRenderer()
{
	resvg_tree_destroy(svg);
	resvg_options_destroy(opt);
}
