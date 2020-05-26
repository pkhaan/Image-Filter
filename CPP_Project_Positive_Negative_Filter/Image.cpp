#include "Image.h"
#include "Color.h"
#include "ppm\ppm.h"
#include <iostream>
using namespace std;

imaging::Color * imaging::Image::getRawDataPtr()
{
	return buffer;
}

imaging::Color imaging::Image::getPixel(unsigned int x, unsigned int y) const
{
	if (x<0 || x>width - 1 || y<0 || y>height - 1)
		return Color(0, 0, 0);
	else {
		return buffer[y*width + x];
	}
}

void imaging::Image::setPixel(unsigned int x, unsigned int y, Color & value)
{
	if (x<0 || x>width - 1 || y<0 || y>height - 1)
		return;
	else
		buffer[y*width + x] = value;
}

void imaging::Image::setData(const Color *& data_ptr)
{
	if (width == 0 || height == 0 || buffer == nullptr) return;
	else
		for (unsigned int i = 0; i < width*height; i++)
			buffer[i] = Color(data_ptr[i].r, data_ptr[i].g, data_ptr[i].b);

}

imaging::Image::Image()
{
	width = 0;
	height = 0;
	buffer = nullptr;
}

imaging::Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	buffer = new Color[width*height];
}

imaging::Image::Image(unsigned int width, unsigned int height, const Color * data_ptr)
{
	this->width = width;
	this->height = height;
	buffer = new Color[width*height];
	setData(data_ptr);
}

imaging::Image::Image(const Image & src)
{
	width = src.getWidth();
	height = src.getHeight();
	buffer = new Color[width*height];
	memcpy(buffer, src.buffer, width* height * sizeof(Color));

}

imaging::Image::~Image()
{
	delete[] buffer;
}

imaging::Image & imaging::Image::operator=(const Image & right)
{
	if (&right == this)
		return *this;
	if (buffer != nullptr)
		delete[] buffer;
	width = right.getWidth();
	height = right.getHeight();
	buffer = new Color[width*height];
	const Color* rawD = ((Image)right).getRawDataPtr();
	setData(rawD);
	return *this;
}

bool imaging::Image::load(const std::string & filename, const std::string & format)
{
	if (filename.size() >= format.size() && filename.compare(filename.size() - format.size(), format.size(), format) == 0) {
		width = height = 0; buffer = nullptr;
		float* valsArray = ReadPPM(filename.c_str(), (int*)&width, (int*)&height);
		if (valsArray != nullptr) {
			buffer = new Color[width*height];
			int k = 0;
			for (unsigned int i = 0; i < width*height; i++) {
				buffer[i] = Color(valsArray[k], valsArray[k + 1], valsArray[k + 2]);
				k += 3;
			}
			delete[] valsArray;
			return true;
		}
		else {
			delete[] valsArray;
			return false;
		}
	}
	else return false;
}

bool imaging::Image::save(const std::string & filename, const std::string & format)
{
	if (filename.size() >= format.size() && filename.compare(filename.size() - format.size(), format.size(), format) == 0
		&& buffer != nullptr) {
		float* valsArray = new float[3 * width*height];

		int k = 0;
		Color white = Color(1.0f, 1.0f, 1.0f);
		for (unsigned int i = 0; i < width*height; i++) {
			Color negColor = white - buffer[i];
			valsArray[k] = negColor.r;
			valsArray[k + 1] = negColor.g;
			valsArray[k + 2] = negColor.b;
			k += 3;
		}

		bool result = WritePPM(valsArray, width, height, filename.c_str());
		delete[] valsArray;
		return result;
	}
	else {
		return false;
	}
}