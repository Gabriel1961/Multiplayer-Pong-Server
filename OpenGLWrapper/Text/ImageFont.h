#pragma once
#include <string>
#include <fstream>
class ImageFont
{
public:
	const std::string path;
	const glm::vec2 imageSize;
	const glm::vec2 gridSize;
	/// <param name="path">Path to the image (Only .png and .bmp supported)</param>
	/// <param name="imageSize">Image size in pixels</param>
	/// <param name="gridSize">Represents the number of rows and colmuns of characters in the image</param>
	ImageFont(std::string path, glm::vec2 imageSize, glm::vec2 gridSize)
		: path(path), imageSize(imageSize), gridSize(gridSize)
	{
		std::ifstream fin(path);
		if ((path.find(".bmp") || path.find(".png")) == 0)
			throw std::exception("File format not supported");
		if (!fin.is_open())
			throw std::exception("Incorrect file path for image");
		fin.close();
	}
};