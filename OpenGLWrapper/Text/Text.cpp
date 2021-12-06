#include "Text.h"
#include "../../ScreenSize.h"
#include "../Renderer.h"
using namespace std;
std::vector<Text*> Text::textInstances;


std::tuple<uint*, int> GenerateIndexData(const string& str, const ImageFont& font)
{
	int k = 0;
	uint* a = new uint[str.length() * 6];
	for (int i = 0; i < str.length(); i++)
	{
		uint offset = i * 4;
		a[k] = (0 + offset); k++;
		a[k] = (1 + offset); k++;
		a[k] = (2 + offset); k++;
		a[k] = (0 + offset); k++;
		a[k] = (2 + offset); k++;
		a[k] = (3 + offset); k++;
	}
	return { a,str.length() * 6 };
}

std::tuple<float*, int> GenerateVertexData(const string& str, const ImageFont& font)
{
	int k = 0;
	float* a = new float[16 * str.size()];
	float letterLength = 1 / font.gridSize.y;
	float letterHeight = 1 / font.gridSize.x;
	for (int i = 0; i < str.size(); i++)
	{
		int row = (str[i] - ' ') / (int)font.gridSize.y;
		int col = (str[i] - ' ') % (int)font.gridSize.y;
		a[k] = (-1 + i * 2);                   k++;
		a[k] = (-1);						   k++;
		a[k] = (col * letterLength);		   k++;
		a[k] = (1 - (row + 1) * letterHeight); k++;
		a[k] = (1 + i * 2);					   k++;
		a[k] = (-1);						   k++;
		a[k] = ((col + 1) * letterLength);	   k++;
		a[k] = (1 - (row + 1) * letterHeight); k++;
		a[k] = (1 + i * 2);					   k++;
		a[k] = (1);							   k++;
		a[k] = ((col + 1) * letterLength);	   k++;
		a[k] = (1 - row * letterHeight);	   k++;
		a[k] = (-1 + i * 2);				   k++;
		a[k] = (1);							   k++;
		a[k] = (col * letterLength);		   k++;
		a[k] = (1 - row * letterHeight);	   k++;
	}
	return { a,16 * str.size() };
}

void Text::LoadFontFaceAndShader()
{
	//delete texture;
	//delete shader;

	shader = new Shader("OpenGLWrapper/Text/Text.shader");
	texture = new Texture(font.path);
}

void Text::LoadVertexData()
{
	auto [vbData, vbDataSize] = GenerateVertexData(textStr, font);
	vb = new VertexBuffer(vbDataSize * sizeof(float), vbData);
	auto [ibData, ibDataSize] = GenerateIndexData(textStr, font);
	ib = new IndexBuffer(ibDataSize, ibData);

	VertexBufferLayout vbl;
	vbl.Push<float>(2);
	vbl.Push<float>(2);
	va = new VertexArray();
	va->AddLayout(*vb, vbl);

}

void Text::RenderText()
{
	Renderer::Draw(*va, *ib, *shader, *texture, { "samp",0 }, GL_TRIANGLES);
}

Text::Text(string str, ImageFont font) : font(font), textStr(str)
{
	textInstances.push_back(this);
	LoadFontFaceAndShader();
	LoadVertexData();
}
Text::~Text()
{
	for (int i = 0; i < textInstances.size(); i++)
		if (textInstances[i] == this)
			textInstances.erase(textInstances.begin() + i);
}

void Text::Render()
{
	for (auto text : textInstances)
	{
		text->shader->SetUniformMat3f("MVP", text->transform.GetMatrix());
		text->RenderText();
	}
}

