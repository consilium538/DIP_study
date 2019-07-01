#include "intensity.hpp"

cv::Mat intensityLUT(
	std::function<unsigned char(unsigned char)> func
)
{
	cv::Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i)
		p[i] = func(i);
	return lookUpTable.clone();
}

cv::Mat intensityTransform(
	cv::Mat img,
	std::function<unsigned char(unsigned char)> func
)
{
	cv::Mat returnImg;
	cv::LUT(img, intensityLUT(func), returnImg);
	return returnImg.clone();
}

cv::Mat intensityTransform_iter(
	cv::Mat img,
	std::function<double(double)> func
)
{
	cv::Mat returnImg = img.clone();
	int channels = img.channels();
	int nRows = img.rows;
	int nCols = img.cols * channels;
	double* p;

	if (img.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	for (int i = 0; i < nRows; i++)
	{
		p = returnImg.ptr<double>(i);
		for (int j = 0; j < nCols; j++)
		{
			p[j] = func(p[j]);
		}
	}

	return returnImg.clone();
}

unsigned char inverse(unsigned char input)
{
	return 255 - input;
};

std::function<unsigned char(unsigned char)> logtransform(double c)
{
	return [=](int input) { return (unsigned char)(c * log(1 + input)); };
}

std::function<unsigned char(unsigned char)> gammatransform(double c, double r)
{
    return [=](int input) { return (unsigned char)(c * pow(input,r)); };
}
