#pragma once
#include <string>
#include <Common.h>
#include <unordered_map>
#include <../ScreenSize.h>
class Texture
{
private:
	int m_Height, m_Width, m_BPP;
public:
	std::string m_FilePath = "";
	unsigned int m_RendererID;

	std::string type;

	Texture(const std::string& _FilePath);
	Texture(const Texture& o);
	Texture() {}
	Texture(uint heigth, uint witdth, GLenum antiAliasing = GL_NEAREST, GLenum format = GL_RGBA32F);
	Texture(ARGBColor* buffer, GLenum antiAliasing = GL_NEAREST);
	~Texture();

	Texture& operator=(const Texture& o);

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	int  GetHeight()const { return m_Height; }
	int  GetWidth()const { return m_Width; }
};
struct TextureRef
{
	Texture ref;
	int refCount;
	static void Decrement(const std::string& path);
};
