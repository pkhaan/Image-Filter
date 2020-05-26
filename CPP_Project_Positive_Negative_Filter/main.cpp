#include <iostream>
#include <fstream>
#include <string>
#include "Color.h"
#include "Image.h"
#include "ppm\ppm.h"

using namespace std;
using namespace imaging;

int main(int argc, const char* argv[]) {
	//----------------READ----------------
	string format = "ppm";
	string filenameIn;
	if (argc > 1) {
		filenameIn = argv[1];
	}
	else {
		cout << "Please enter a filename: " << endl;
		cin >> filenameIn;
	}

	Image imageFile;

	if (imageFile.load(filenameIn, format)) {
		cout << "Image dimensions are: " << imageFile.getWidth() << " X " << imageFile.getHeight() << endl;
	}
	else cout << "Could not load file." << endl;

	//----------------WRITE----------------
	string filenameOut = filenameIn.substr(0, filenameIn.find('.')).append("_neg.").append(format);
	if (!imageFile.save(filenameOut, format)) 
		cout << "Could not save file." << endl;

	system("pause");
	return 0;
}