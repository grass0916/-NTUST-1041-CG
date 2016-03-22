#pragma once
#include "OpenglWidget.h"
#include <vector>

using namespace std;

class Stroke { // Data structure for holding painterly strokes.
public:
	Stroke(void);
	Stroke(unsigned int radius, unsigned int x, unsigned int y,
		unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	// data
	unsigned int radius, x, y;	// Location for the stroke
	unsigned char r, g, b, a;	// Color
};

class Application : public OpenglWidget {
public:
	Application();
	~Application();
	unsigned char* To_RGB(void);
	void RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb);

	// File
	void loadSecondaryImge(QString filePath);
	void openImage(QString filePath);
	void saveImage(QString filePath);
	void reload();

	// Color
	void Gray();
	void Quant_Uniform();
	string convertColorToString(int red, int green, int blue, int dimReduce); // Added.
	void Quant_Populosity();

	// Dithering
	void Dither_Threshold();
	void Dither_Bright();
	void Dither_Random();
	void Dither_Cluster();
	void Dither_FS();
	float getDitheringClosedRGB(float pixel, int pColor); // Added.
	void Dither_Color();

	// Filter
	float ** create2dFilter(int sx, int sy, float filterData[]); // Added.
	float ** create2dFilter(int sx, int sy, vector<float> filterData); // Added.
	int getBoxFilterAvg(unsigned char * rgb, int startRGB, float ** filter, int filterSize, float weights); // Added.
	void Filter_Box();
	void Filter_Bartlett();
	void Filter_Gaussian();
	vector<float> getBinomialFilter(int n); // Added.
	void Filter_Gaussian_N(unsigned int n);
	void Filter_Edge();
	void Filter_Enhance();

	// Size
	void Half_Size();
	void Double_Size();
	double BilinearInterpolation(int q11, int q12, int q21, int q22, int x1, int x2, int y1, int y2, double x, double y);
	void Resize(float scale);
    void Rotate(float angleDegrees);

	// Composing
	enum _CompImage { _over, _in, _out, _atop, _xor };
	void Comp_image(int tMethod);
	void Comp_Over();
	void Comp_In();
	void Comp_Out();
	void Comp_Atop();
	void Comp_Xor();

	// Special
	void NPR_Paint();
	void Paint_Stroke(const Stroke& s);
	void NPR_Paint_Layer(unsigned char *tCanvas, unsigned char *tReferenceImage, int tBrushSize);

	// Bonus
	void Filter_Edge_Detection();
	
protected:
	QImage mImageSrc, mImageSrcSecond;
	QImage mImageDst;

	void createScene(void);

	void renew();

	bool bRenew;

	// Image data
	unsigned char* imgData,*imgData2;
	int imgWidth,imgWidth2;
	int imgHeight,imgHeight2;

	// you should swap the rr and bb if you read img_data directly without using To_RGB(), cause OGRE is in BGR format
	enum { RR = 2, GG = 1, BB = 0, AA = 3 };
	enum { BLACK = 0, WHITE = 255 };

	Qt_Opengl_Framework * ui_instance;
};
