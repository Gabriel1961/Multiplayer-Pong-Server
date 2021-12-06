#pragma once
#include <vector>
#include <GL/glm/ext/matrix_float3x3.hpp>
#include "../VertexArray.h"
#include "../IndexBuffer.h"
#include "../VertexBuffer.h"
#include "../Texture.h"
#include "../Shader.h"
#include "ImageFont.h"
#include <string>
#include "../Transform.h"
class Text
{
private:
	VertexBuffer* vb = 0;
	IndexBuffer* ib = 0;
	VertexArray* va = 0;
	Shader* shader = 0;
	Texture* texture = 0;
	static std::vector<Text*> textInstances;
	void LoadFontFaceAndShader();
	void LoadVertexData();
	void RenderText();
	std::string textStr;
	glm::mat4x2 uvCoords;
public:
	const ImageFont font;
	Transform transform;

	Text(std::string str, ImageFont font = ImageFont("OpenGLWrapper/Text/Text.png", { 252,108 }, { 6,18 }));
	~Text();
	static void Render();
};
