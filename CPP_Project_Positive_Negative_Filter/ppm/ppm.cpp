#include "ppm.h"
#include <iostream>
#include <fstream>

using namespace std;

float * imaging::ReadPPM(const char * filename, int * w, int * h)
{
	ifstream imgFile(filename, ios::in | ios::binary);
	if (!imgFile) {
		cerr << "Cannot open image file." << endl;
		return nullptr;
	}

	char* format = new char[2];
	unsigned int width, height, maxCap;
	imgFile >> format >> width >> height >> maxCap;

	if (format[0]!='P' || format[1] != '6') {
		cout << "The .ppm file is not of type P6." << endl;
		return nullptr;
	}
	if (width != unsigned int(width) && height != unsigned int(height)) {
		cout << "The width and/or height of the file are missing." << endl;
		return nullptr;
	}
	if (maxCap != unsigned int(maxCap) || maxCap > 255) {
		cout << "The maximum storage capacity is missing or is greater than 255." << endl;
		return nullptr;
	}

	*w = width;
	*h = height;

	imgFile.seekg((int)imgFile.tellg() + 1); // skip new line character
	
	int numVals = 3 * width * height; // theu number of values we expect to read
	
	unsigned char *valsArray = new unsigned char[numVals]; // allocate an array of appropriate size
	
	imgFile.read((char*)valsArray, numVals); // read all values

	// Making the float 0-1 array
	float* floatValsArray = new float[numVals];
	for (int i = 0; i < numVals; i++) {
		floatValsArray[i] = valsArray[i] / 255.0f;
	}

	imgFile.close();

	delete[] valsArray;

	return floatValsArray;
}

bool imaging::WritePPM(const float * data, int w, int h, const char * filename)
{
	ofstream newFile(filename, ios::trunc | ios::binary);
	if (!newFile) {
		cerr << "Cannot create image file." << endl;
		return false;
	}
	if (data==nullptr) {
		cerr << "No data to write." << endl;
		return false;
	}

	newFile << "P6 " << w << " " << h << " 255\n";

	for (int i = 0; i < 3*w*h; i++) {
		newFile << unsigned char(data[i]*255.0f+0.5f);
	}
	newFile.close();

	return true;
}
