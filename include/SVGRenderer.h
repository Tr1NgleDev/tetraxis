#pragma once

#include <basicIncludes.h>
#include <TexRenderer.h>
#include <resvg.h>

class SVGRenderer : public TexRenderer
{
protected:
	static resvg_options* opt;
	resvg_render_tree* svg = nullptr;
	Texture svgTex{};
	float svgQuality = 1.0f;

public:
	void setTexture(Texture*) = delete;

	SVGRenderer() {}
	SVGRenderer(const std::string& svg, Shader* shader);
	void setSVG(const std::string& svg);
	void setSVGQuality(float quality);
	float getSVGQuality() const { return svgQuality; }
	void render();
	~SVGRenderer();
};
