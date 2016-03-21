#include "Application.h"
#include "qt_opengl_framework.h"
#include <map>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <algorithm>

using namespace std;

Application::Application() {

}
Application::~Application() {

}

//****************************************************************************
//
// * 初始畫面，並顯示Ntust.png圖檔
// 
//============================================================================

void Application::createScene(void) {
	
	ui_instance = Qt_Opengl_Framework::getInstance();
	
}

//****************************************************************************
//
// * 打開指定圖檔
// 
//============================================================================

void Application::openImage(QString filePath) {
	mImageSrc.load(filePath);
	mImageDst.load(filePath);

	renew();

	imgData = mImageSrc.bits();
	imgWidth = mImageSrc.width();
	imgHeight = mImageSrc.height();

	ui_instance->ui.label->setFixedHeight(imgHeight);
	ui_instance->ui.label->setFixedWidth(imgWidth);
}

//****************************************************************************
//
// * 刷新畫面
// 
//============================================================================

void Application::renew() {
	ui_instance = Qt_Opengl_Framework::getInstance();

	ui_instance->ui.label->clear();
	ui_instance->ui.label->setPixmap(QPixmap::fromImage(this->mImageDst));

	cout << "Refresh." << endl;
}

//****************************************************************************
//
// * 畫面初始化
// 
//============================================================================

void Application::reload() {
	ui_instance = Qt_Opengl_Framework::getInstance();

	ui_instance->ui.label->clear();
	ui_instance->ui.label->setPixmap(QPixmap::fromImage(mImageSrc));
}

//****************************************************************************
//
// * 儲存圖檔
// 
//============================================================================

void Application::saveImage(QString filePath) {
	mImageDst.save(filePath);
}

//****************************************************************************
//
// * 將圖檔資料轉換為RGB色彩資料
// 
//============================================================================

unsigned char* Application::To_RGB( void ) {
	unsigned char * rgb = new unsigned char[imgWidth * imgHeight * 3];
	int i, j;

	if (! this->imgData )
		return NULL;

	// Divide out the alpha
	for (i = 0; i < imgHeight; i++) {
		int in_offset  = i * imgWidth * 4;
		int out_offset = i * imgWidth * 3;

		for (j = 0 ; j < imgWidth ; j++) {
			RGBA_To_RGB(this->imgData + (in_offset + j * 4), rgb + (out_offset + j * 3));
		}
	}

	return rgb;
}

void Application::RGBA_To_RGB( unsigned char * rgba, unsigned char * rgb ) {
	const unsigned char	BACKGROUND[3] = { 0, 0, 0 };

	unsigned char  alpha = rgba[3];

	if (alpha == 0) {
		rgb[0] = BACKGROUND[0];
		rgb[1] = BACKGROUND[1];
		rgb[2] = BACKGROUND[2];
	} else {
		float alpha_scale = (float) 255 / (float) alpha;
		int	val;
		int	i;

		for (i = 0 ; i < 3 ; i++) {
			val = (int)floor(rgba[i] * alpha_scale);
			if (val < 0)
				rgb[i] = 0;
			else if (val > 255)
				rgb[i] = 255;
			else
				rgb[i] = val;
		}
	}
}

//------------------------Color------------------------

///////////////////////////////////////////////////////////////////////////////
//
//  Convert image to grayscale.  Red, green, and blue channels should all 
//  contain grayscale value.  Alpha channel shoould be left unchanged.  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Gray() {
	unsigned char * rgb = To_RGB();

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			int gray = 0.3 * rgb[oRGB + RR] + 0.59 * rgb[oRGB + GG] + 0.11 * rgb[oRGB + BB];
			// Convert to gray.
			imgData[oRGBA + RR] = imgData[oRGBA + GG] = imgData[oRGBA + BB] = gray;
			imgData[oRGBA + AA] = WHITE;
		}
	}
	
	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using uniform quantization.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Quant_Uniform() {
	unsigned char * rgb = this->To_RGB();

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// 8 reds, 8 greens and 4 blues = 256 colors.
			imgData[oRGBA + RR] = (rgb[oRGB + RR] >> 5) << 5;
			imgData[oRGBA + GG] = (rgb[oRGB + GG] >> 5) << 5;
			imgData[oRGBA + BB] = (rgb[oRGB + BB] >> 6) << 6;
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using populosity quantization.  
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

struct GreaterCounts {
    bool operator() (pair<string, int> const & a, pair<string, int> const & b) const {
        return a.second > b.second;
    }
};

string Application::convertColorToString(int red, int green, int blue, int dimReduce) {
	stringstream RGB;
	RGB << setw(3) << setfill('0') << ((red   >> dimReduce) << dimReduce)
		<< setw(3) << setfill('0') << ((green >> dimReduce) << dimReduce)
		<< setw(3) << setfill('0') << ((blue  >> dimReduce) << dimReduce);
	return RGB.str();
}

void Application::Quant_Populosity() {
	unsigned char * rgb = this->To_RGB();

	// [Map] Create color table. Store all color RGB to map container.
	map<string, int> colorTable;
	map<string, int>::iterator it;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Generate the RBG and divide 8 to gray. (#FF00FF is 255000255)
			string RGB = this->convertColorToString((int) rgb[oRGB + RR], (int) rgb[oRGB + GG], (int) rgb[oRGB + BB], 3);
			// Add the count of color in map container.
			it = colorTable.find(RGB);
			if (it == colorTable.end())
				colorTable.insert(pair<string, int> (RGB, 1));
			else
				it->second += 1;
		}
	}
	// [Vector] Sort by counts decreasing using vector container.
	vector<pair<string, int> > v_colorTable(colorTable.begin(), colorTable.end());
	sort(v_colorTable.begin(), v_colorTable.end(), GreaterCounts());
	// [Vector] Filter top 256.
	if (v_colorTable.size() > 256)
		v_colorTable.erase(v_colorTable.begin() + 256, v_colorTable.end());
	// [Map] Clean color table. Convert vector to map container.
	colorTable.erase(colorTable.begin(), colorTable.end());
	for (vector<pair<string, int> >::iterator it = v_colorTable.begin(); it != v_colorTable.end(); it++)
		colorTable.insert(pair<string, int> (it->first, it->second));
	// Assign the new color via color table with euclidean distance.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Get the current RGB of pixel.
			string RGB = this->convertColorToString((int) rgb[oRGB + RR], (int) rgb[oRGB + GG], (int) rgb[oRGB + BB], 3);
			it = colorTable.find(RGB);
			// This color is in color table.
			if (it != colorTable.end()) {
				imgData[oRGBA + RR] = atoi(it->first.substr(0, 3).c_str());
				imgData[oRGBA + GG] = atoi(it->first.substr(3, 3).c_str());
				imgData[oRGBA + BB] = atoi(it->first.substr(6, 3).c_str());
				imgData[oRGBA + AA] = WHITE;
				continue;
			}
			// This color isn't in color table, need to find closest color with euclidean distance.
			int closestR, closestG, closestB;
			double closestDistance = sqrt(3 * pow(256.0, 2));
			int r1 = (int) rgb[oRGB + RR], g1 = (int) rgb[oRGB + GG], b1 = (int) rgb[oRGB + BB];
			for (it = colorTable.begin(); it != colorTable.end(); it++) {
				int r2 = atoi(it->first.substr(0, 3).c_str()),
					g2 = atoi(it->first.substr(3, 3).c_str()),
					b2 = atoi(it->first.substr(6, 3).c_str());
				double distance = sqrt(pow(((double) r1 - r2), 2) + pow(((double) g1 - g2), 2) + pow(((double) b1 - b2), 2));
				if (distance < closestDistance) {
					closestR = r2;
					closestG = g2;
					closestB = b2;
					closestDistance = distance;
				}
			}
			imgData[oRGBA + RR] = closestR;
			imgData[oRGBA + GG] = closestG;
			imgData[oRGBA + BB] = closestB;
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

//------------------------Dithering------------------------

///////////////////////////////////////////////////////////////////////////////
//
//  Dither the image using a threshold of 1/2.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Dither_Threshold() {
	unsigned char * rgb = this->To_RGB();

	// Convert to gray then using intensities to turn black or white.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			float gray = 0.3 * rgb[oRGB + RR] + 0.59 * rgb[oRGB + GG] + 0.11 * rgb[oRGB + BB];
			float intensity = gray / 256;
			// Threshold is 0.5 intensity.
			imgData[oRGBA + RR] = imgData[oRGBA + GG] = imgData[oRGBA + BB] = intensity > 0.5 ? WHITE : BLACK;
			imgData[oRGBA + AA] = WHITE;
		}
	}
	
	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Dither the image while conserving the average brightness.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Dither_Bright() {
	unsigned char * rgb = this->To_RGB();

	// Calculate the average of intensity.
	float intensitySum = 0, intensityAvg = 0;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			float gray = 0.3 * rgb[oRGB + RR] + 0.59 * rgb[oRGB + GG] + 0.11 * rgb[oRGB + BB];
			intensitySum += gray / 256;
		}
	}
	intensityAvg = intensitySum / (imgWidth * imgHeight);

	// Convert to gray then using intensities to turn black or white.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			float gray = 0.3 * rgb[oRGB + RR] + 0.59 * rgb[oRGB + GG] + 0.11 * rgb[oRGB + BB];
			float intensity = gray / 256;
			// Threshold is average intensity.
			imgData[oRGBA + RR] = imgData[oRGBA + GG] = imgData[oRGBA + BB] = intensity > intensityAvg ? WHITE : BLACK;
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Dither image using random dithering.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Dither_Random() {
	unsigned char * rgb = this->To_RGB();

	// Add random variables of time.
	srand(time(NULL));
	// Convert to gray then using intensities to turn black or white.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			double gray = 0.3 * rgb[oRGB + RR] + 0.59 * rgb[oRGB + GG] + 0.11 * rgb[oRGB + BB];
			// Random intensity is [-0.2, 0.2].
			double intensity = gray / 256 + (-0.2 + (((double) rand()) / (double) RAND_MAX) * 0.4);
			// Threshold is 0.5 intensity.
			imgData[oRGBA + RR] = imgData[oRGBA + GG] = imgData[oRGBA + BB] = intensity > 0.5 ? WHITE : BLACK;
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Perform clustered differing of the image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Dither_Cluster() {
	unsigned char * rgb = this->To_RGB();

	// Assign the mask matrix.
	float mask[4][4] = {
		{ 0.7059, 0.3529, 0.5882, 0.2353 },
		{ 0.0588, 0.9412, 0.8235, 0.4118 },
		{ 0.4706, 0.7647, 0.8824, 0.1176 },
		{ 0.1765, 0.5294, 0.2941, 0.6471 }
	};

	// Convert to gray then using intensities to turn black or white.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			float gray = 0.3 * rgb[oRGB + RR] + 0.59 * rgb[oRGB + GG] + 0.11 * rgb[oRGB + BB];
			float intensity = gray / 256;
			// Get intensity of mask.
			float maskIntensity = mask[i % 4][j % 4];
			// Threshold is mask intensity.
			imgData[oRGBA + RR] = imgData[oRGBA + GG] = imgData[oRGBA + BB] = intensity > maskIntensity ? WHITE : BLACK;
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Perform Floyd-Steinberg dithering on the image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Dither_FS() {
	// Convert to black or white.
	this->Gray();

	unsigned char * rgb = this->To_RGB();

	// If zigZagFlag is true, j will strat from the rightest.
	bool zigZagFlag = false;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = zigZagFlag ? imgWidth - 1 : 0; j >= 0 && j < imgWidth; j += zigZagFlag ? -1 : 1) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			float oldPixel = rgb[oRGB + RR];
			float newPixel = (oldPixel / 256) > 0.5 ? WHITE : BLACK;
			// pColor is RR, GG and BB.
			for (int pColor = 0; pColor < 3; pColor++) {
				// Assign the new pixel color.
				imgData[oRGBA + pColor] = newPixel;
				imgData[oRGBA + AA] = WHITE;
				// Add error.
				float propagationErr = oldPixel - newPixel;
				// Right.
				if (j + 1 < imgWidth)
					rgb[oRGB + 3 + pColor] += 7.0 / 16 * propagationErr;
				// Left bottom.
				if (i + 1 < imgHeight && j - 1 > 0)
					rgb[oRGB + 3 * imgWidth - 3 + pColor] += 3.0 / 16 * propagationErr;
				// Bottom.
				if (i + 1 < imgHeight)
					rgb[oRGB + 3 * imgWidth + pColor] += 5.0 / 16 * propagationErr;
				// Right bottom.
				if (i + 1 < imgHeight && j + 1 < imgWidth)
					rgb[oRGB + 3 * imgWidth + 3 + pColor] += 1.0 / 16 * propagationErr;
			}
		}
		// Toggle the flag.
		zigZagFlag = zigZagFlag ? false : true;
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using Floyd-Steinberg dithering over
//  a uniform quantization - the same quantization as in Quant_Uniform.
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

float Application::getDitheringClosedRGB(float pixel, int pColor) {
	// Static color sets.
	float red[8]   = { 0, 36, 73, 109, 146, 182, 219, 255 },
		  green[8] = { 0, 36, 73, 109, 146, 182, 219, 255 },
		  blue[4]  = { 0, 85, 170, 255 };
	float newPixel;
	if (pColor == RR)
		return red[(int) roundf(pixel / (256.0 / 7))];
	else if (pColor == GG)
		return green[(int) roundf(pixel / (256.0 / 7))];
	else if (pColor == BB)
		return blue[(int) roundf(pixel / (256.0 / 3))];
}

void Application::Dither_Color() {
	unsigned char * rgb = this->To_RGB();

	// If zigZagFlag is true, j will strat from the rightest.
	bool zigZagFlag = false;
	for (int i = 0; i < imgHeight; i++) {
		for (int j = zigZagFlag ? imgWidth - 1 : 0; j >= 0 && j < imgWidth; j += zigZagFlag ? -1 : 1) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// pColor is RR, GG and BB.
			for (int pColor = 0; pColor < 3; pColor++) {
				float oldPixel = rgb[oRGB + pColor];
				float newPixel = getDitheringClosedRGB(oldPixel, pColor);
				// Assign the new pixel color.
				imgData[oRGBA + pColor] = newPixel;
				imgData[oRGBA + AA] = WHITE;
				// Add error.
				float propagationErr = oldPixel - newPixel;
				// Right.
				if (j + 1 < imgWidth)
					rgb[oRGB + 3 + pColor] += 7.0 / 16 * propagationErr;
				// Left bottom.
				if (i + 1 < imgHeight && j - 1 > 0)
					rgb[oRGB + 3 * imgWidth - 3 + pColor] += 3.0 / 16 * propagationErr;
				// Bottom.
				if (i + 1 < imgHeight)
					rgb[oRGB + 3 * imgWidth + pColor] += 5.0 / 16 * propagationErr;
				// Right bottom.
				if (i + 1 < imgHeight && j + 1 < imgWidth)
					rgb[oRGB + 3 * imgWidth + 3 + pColor] += 1.0 / 16 * propagationErr;
			}
		}
		// Toggle the flag.
		zigZagFlag = zigZagFlag ? false : true;
	}

	// Based on quant uniform.
	this->Quant_Uniform();

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

//------------------------Filter------------------------

///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 box filter on this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

float ** Application::create2dFilter(int sx, int sy, float filterData[]) {
	// Dynamic allocation for 2d array.
	float ** filter;
	filter = (float **) malloc(sx * sizeof(float *));
	for (int i = 0; i < sx; i++)
		filter[i] = (float *) malloc(sy * sizeof(float));
	// Assign the values.
	for (int i = 0; i < sy; i++)
		for (int j = 0; j < sx; j++)
			filter[i][j] = filterData[i * sx + j];
	return filter;
}

float ** Application::create2dFilter(int sx, int sy, vector<float> filterData) {
	// Dynamic allocation for 2d array.
	float ** filter;
	filter = (float **) malloc(sx * sizeof(float *));
	for (int i = 0; i < sx; i++)
		filter[i] = (float *) malloc(sy * sizeof(float));
	// Assign the values.
	for (int i = sy - 1; i >= 0; i--) {
		for (int j = sx - 1; j >= 0; j--) {
			filter[i][j] = filterData.back();
			filterData.pop_back();
		}
	}
	return filter;
}

int Application::getBoxFilterAvg(unsigned char * rgb, int startRGB, float ** filter, int filterSize, float weights) {
	int maskWidth  = filterSize,
		maskHeight = filterSize;
	// Calculate the sum of color.
	int colorSum = 0;
	for (int i = 0; i < maskHeight; i++) {
		for (int j = 0; j < maskWidth; j++) {
			int oRGB = startRGB + i * this->imgWidth * 3 + j * 3;
			if (oRGB < 0 || oRGB > this->imgHeight * this->imgWidth * 3)
				colorSum += 0;
			else
				colorSum += rgb[oRGB] * filter[i][j];
		}
	}
	return (colorSum / weights);
}

void Application::Filter_Box() {
	unsigned char * rgb = this->To_RGB();

	// Set the filter data for building filter.
	int maskWidth = 5, maskHeight = 5;
	float filterData[25] = {
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1
	};
	float ** filter = this->create2dFilter(maskHeight, maskWidth, filterData);

	// Convert to new color using average of filtering.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Calculate start index to filtering.
			int startIndex = oRGB - 2 * imgWidth * 3 - 2 * 3;
			// pColor is RR, GG and BB.
			for (int pColor = 0; pColor < 3; pColor++)
				imgData[oRGBA + pColor] = this->getBoxFilterAvg(rgb, startIndex + pColor, filter, 5, 25);
			imgData[oRGBA + AA] = WHITE;
		}
	}
	
	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 Bartlett filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Filter_Bartlett() {
	unsigned char * rgb = this->To_RGB();

	// Set the filter data for building filter.
	int maskWidth = 5, maskHeight = 5;
	float filterData[25] = {
		1, 2, 3, 2, 1,
		2, 4, 6, 4, 2,
		3, 6, 9, 6, 3,
		2, 4, 6, 4, 2,
		1, 2, 3, 2, 1
	};
	float ** filter = this->create2dFilter(maskHeight, maskWidth, filterData);

	// Convert to new color using average of filtering.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Calculate start index to filtering.
			int startIndex = oRGB - 2 * imgWidth * 3 - 2 * 3;
			// pColor is RR, GG and BB.
			for (int pColor = 0; pColor < 3; pColor++)
				imgData[oRGBA + pColor] = this->getBoxFilterAvg(rgb, startIndex + pColor, filter, 5, 81);
			imgData[oRGBA + AA] = WHITE;
		}
	}
	
	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Filter_Gaussian() {
	unsigned char * rgb = this->To_RGB();

	// Set the filter data for building filter.
	int maskWidth = 5, maskHeight = 5;
	float filterData[25] = {
		1,  4,  6,  4, 1,
		4, 16, 24, 16, 4,
		6, 24, 36, 24, 6,
		4, 16, 24, 16, 4,
		1,  4,  6,  4, 1
	};
	float ** filter = this->create2dFilter(maskHeight, maskWidth, filterData);

	// Convert to new color using average of filtering.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Calculate start index to filtering.
			int startIndex = oRGB - 2 * imgWidth * 3 - 2 * 3;
			// pColor is RR, GG and BB.
			for (int pColor = 0; pColor < 3; pColor++)
				imgData[oRGBA + pColor] = this->getBoxFilterAvg(rgb, startIndex + pColor, filter, 5, 256);
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Perform NxN Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////

vector<float> Application::getBinomialFilter(int n) {
	// Set the filter data for building filter.
	vector<float> filterData;
	// Binomial coefficient.
	int biCoef[n];
    for (int i = 0; i < n ; i++) {
    	// If this row not enough column for size, skip it.
        if (i + 1 < n)
            continue;
        // Calculate the coefficients.
        for (int j = 0, coef = 1; j <= i; j++) {
            coef = (! j || ! i) ? 1 : (coef * (i - j + 1) / j);
            biCoef[j] = coef;
        }
    }
    // Generate the filter array.
    for (int i = 0; i < n ; i++) {
        for (int j = 0; j < n; j++) {
            if (i == 0 || i == n - 1)
                filterData.push_back(biCoef[j]);
            else if (j == 0 || j == n - 1)
                filterData.push_back(biCoef[i]);
            else
                filterData.push_back(biCoef[i] * biCoef[j]);
        }
    }
	return filterData;
}

void Application::Filter_Gaussian_N(unsigned int n) {
	unsigned char * rgb = this->To_RGB();

	// Set the filter data for building filter.
	float ** filter = this->create2dFilter(n, n, this->getBinomialFilter(n));
	float weights = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			weights += filter[i][j];

	// Convert to new color using average of filtering.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Calculate start index to filtering.
			int startIndex = oRGB - 2 * imgWidth * 3 - 2 * 3;
			// pColor is RR, GG and BB.
			for (int pColor = 0; pColor < 3; pColor++)
				imgData[oRGBA + pColor] = this->getBoxFilterAvg(rgb, startIndex + pColor, filter, n, weights);
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Perform 5x5 edge detect (high pass) filter on this image.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Filter_Edge() {
	unsigned char * rgb = this->To_RGB();

	// Set the filter data for building filter.
	int maskWidth = 5, maskHeight = 5;
	float filterData[25] = {
		-1,  -4,  -6,  -4, -1,
		-4, -16, -24, -16, -4,
		-6, -24, 220, -24, -6,
		-4, -16, -24, -16, -4,
		-1,  -4,  -6,  -4, -1
	};
	float ** filter = this->create2dFilter(maskHeight, maskWidth, filterData);

	// Convert to new color using average of filtering.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Calculate start index to filtering.
			int startIndex = oRGB - 2 * imgWidth * 3 - 2 * 3;
			// pColor is RR, GG and BB.
			for (int pColor = 0; pColor < 3; pColor++)
				imgData[oRGBA + pColor] = this->getBoxFilterAvg(rgb, startIndex + pColor, filter, 5, 256);
			imgData[oRGBA + AA] = WHITE;
		}
	}
	
	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

void Application::Filter_Edge_Detection() {
	unsigned char * rgb = this->To_RGB();

	this->Filter_Edge();
	this->Gray();

	// Convert to new color using average of filtering.
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGBA = i * imgWidth * 4 + j * 4;
			// Calculate start index to filtering.
			imgData[oRGBA + RR] = 255 - imgData[oRGBA + RR];
			imgData[oRGBA + GG] = 255 - imgData[oRGBA + GG];
			imgData[oRGBA + BB] = 255 - imgData[oRGBA + BB];
			imgData[oRGBA + AA] = WHITE;
		}
	}

	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Perform a 5x5 enhancement filter to this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Filter_Enhance() {
	unsigned char * rgb = this->To_RGB();


	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

//------------------------Size------------------------

///////////////////////////////////////////////////////////////////////////////
//
//  Halve the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Half_Size() {
	unsigned char * rgb = To_RGB();

	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = (2 * i) * imgWidth * 3 + (2 * j) * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// .
			imgData[oRGBA + RR] = (i < imgHeight / 2 && j < imgWidth / 2) ? rgb[oRGB + RR] : WHITE;
			imgData[oRGBA + GG] = (i < imgHeight / 2 && j < imgWidth / 2) ? rgb[oRGB + GG] : WHITE;
			imgData[oRGBA + BB] = (i < imgHeight / 2 && j < imgWidth / 2) ? rgb[oRGB + BB] : WHITE;
			imgData[oRGBA + AA] = WHITE;
		}
	}
	
	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  Double the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Double_Size() {
	unsigned char * rgb = To_RGB();

	int newImgHeight = 2 * imgHeight,
		newImgWidth  = 2 * imgWidth;

	for (int i = 0; i < imgHeight; i++) {
		cout << "i: " << i;
		for (int j = 0; j < imgWidth; j++) {
			int oRGB  = i * imgWidth * 3 + j * 3;
			int oRGBA = i * imgWidth * 4 + j * 4;
			// .
			imgData[oRGBA + RR] = rgb[oRGB + RR];
			imgData[oRGBA + GG] = rgb[oRGB + GG];
			imgData[oRGBA + BB] = rgb[oRGB + BB];
			imgData[oRGBA + AA] = WHITE;

			cout << ",  \tR:" << (int) rgb[oRGB + RR] << "\tG:" << (int) rgb[oRGB + GG] << "\tB:" << (int) rgb[oRGB + BB];
		}
		cout << endl;
	}

/*	
	for (int i = 0; i < imgHeight; i++) {
		for (int j = 0; j < imgWidth; j++) {
			int oRGB   = i * imgWidth * 3 + j * 3;
			int oRGBA1 = (2 * i)     * imgWidth * 4 + (2 * j)     * 4;
			int oRGBA2 = (2 * i)     * imgWidth * 4 + (2 * j + 1) * 4;
			int oRGBA3 = (2 * i + 1) * imgWidth * 4 + (2 * j)     * 4;
			int oRGBA4 = (2 * i + 1) * imgWidth * 4 + (2 * j + 1) * 4;
			// .
			imgData[oRGBA1 + RR] = imgData[oRGBA2 + RR] = imgData[oRGBA3 + RR] = imgData[oRGBA4 + RR] = rgb[oRGB + RR];
			imgData[oRGBA1 + GG] = imgData[oRGBA2 + GG] = imgData[oRGBA3 + GG] = imgData[oRGBA4 + GG] = rgb[oRGB + GG];
			imgData[oRGBA1 + BB] = imgData[oRGBA2 + BB] = imgData[oRGBA3 + BB] = imgData[oRGBA4 + BB] = rgb[oRGB + BB];
			imgData[oRGBA1 + AA] = imgData[oRGBA2 + AA] = imgData[oRGBA3 + AA] = imgData[oRGBA4 + AA] = WHITE;
		}
	}
*/

	delete[] rgb;
	mImageDst = QImage(imgData, newImgWidth, newImgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//  resample_src for resize and rotate
//
///////////////////////////////////////////////////////////////////////////////

void Application::resample_src(int u, int v, float ww, unsigned char * rgba) {

}

///////////////////////////////////////////////////////////////////////////////
//
//  Scale the image dimensions by the given factor.  The given factor is 
//	assumed to be greater than one.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Resize(float scale) {
	unsigned char * rgb = To_RGB();

	int newImgHeight = scale * imgHeight,
		newImgWidth  = scale * imgWidth;

	for (int x = 0; x < newImgHeight; x++) {
		for (int y = 0; y < newImgWidth; y++) {
			int oRGB  = (2 * x) * imgWidth * 3 + (2 * y) * 3;
			int oRGBA = x * imgWidth * 4 + y * 4;
			// .
			imgData[oRGBA + RR] = rgb[oRGB];
			imgData[oRGBA + GG] = rgb[oRGB];
			imgData[oRGBA + BB] = rgb[oRGB];
			imgData[oRGBA + AA] = WHITE;
		}
	}
	
	delete[] rgb;
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

//////////////////////////////////////////////////////////////////////////////
//
//  Rotate the image clockwise by the given angle.  Do not resize the 
//  image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////

void Application::Rotate(float angleDegrees) {
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

//------------------------Composing------------------------


void Application::loadSecondaryImge( QString filePath )
{
	mImageSrcSecond.load(filePath);

	renew();

	imgData2 = mImageSrcSecond.bits();
	imgWidth2 = mImageSrcSecond.width();
	imgHeight2 = mImageSrcSecond.height();
}

//////////////////////////////////////////////////////////////////////////
//
//	Composite the image A and image B by Over, In, Out, Xor and Atom. 
//
//////////////////////////////////////////////////////////////////////////
void Application::Comp_image( int tMethod )
{
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite the current image over the given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Over()
{
	if (imgHeight == imgHeight2 && imgWidth == imgWidth2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "in" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_In()
{
	if (imgHeight == imgHeight2 && imgWidth == imgWidth2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "out" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Out()
{
	if (imgHeight == imgHeight2 && imgWidth == imgWidth2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite current image "atop" given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Atop()
{
	if (imgHeight == imgHeight2 && imgWidth == imgWidth2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image with given image using exclusive or (XOR).  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::Comp_Xor()
{
	if (imgHeight == imgHeight2 && imgWidth == imgWidth2)
	{

	}
	else
	{
		std::cout << "Images not the same size" << std::endl;
	}
}

//------------------------NPR------------------------

///////////////////////////////////////////////////////////////////////////////
//
//      Run simplified version of Hertzmann's painterly image filter.
//      You probably will want to use the Draw_Stroke funciton and the
//      Stroke class to help.
// Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
void Application::NPR_Paint()
{
	mImageDst = QImage(imgData, imgWidth, imgHeight, QImage::Format_ARGB32);
	renew();
}

void Application::NPR_Paint_Layer( unsigned char *tCanvas, unsigned char *tReferenceImage, int tBrushSize )
{

}

///////////////////////////////////////////////////////////////////////////////
//
//      Helper function for the painterly filter; paint a stroke at
// the given location
//
///////////////////////////////////////////////////////////////////////////////
void Application::Paint_Stroke( const Stroke& s )
{
	int radius_squared = (int)s.radius * (int)s.radius;
	for (int x_off = -((int)s.radius); x_off <= (int)s.radius; x_off++) 
	{
		for (int y_off = -((int)s.radius); y_off <= (int)s.radius; y_off++) 
		{
			int x_loc = (int)s.x + x_off;
			int y_loc = (int)s.y + y_off;

			// are we inside the circle, and inside the image?
			if ((x_loc >= 0 && x_loc < imgWidth && y_loc >= 0 && y_loc < imgHeight)) 
			{
				int dist_squared = x_off * x_off + y_off * y_off;
				int offset_rgba = (y_loc * imgWidth + x_loc) * 4;

				if (dist_squared <= radius_squared) 
				{
					imgData[offset_rgba + RR] = s.r;
					imgData[offset_rgba + GG] = s.g;
					imgData[offset_rgba + BB] = s.b;
					imgData[offset_rgba + AA] = s.a;
				} 
				else if (dist_squared == radius_squared + 1) 
				{
					imgData[offset_rgba + RR] = (imgData[offset_rgba + RR] + s.r) / 2;
					imgData[offset_rgba + GG] = (imgData[offset_rgba + GG] + s.g) / 2;
					imgData[offset_rgba + BB] = (imgData[offset_rgba + BB] + s.b) / 2;
					imgData[offset_rgba + AA] = (imgData[offset_rgba + AA] + s.a) / 2;
				}
			}
		}
	}
}





///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke() {}

///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke(unsigned int iradius, unsigned int ix, unsigned int iy,
	unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia) :
radius(iradius),x(ix),y(iy),r(ir),g(ig),b(ib),a(ia)
{
}



