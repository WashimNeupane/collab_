#pragma once
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;
class FilterPaper
{
public:
	__int16 WIDTH; 
	__int16 HEIGHT;
	__int16 ORIGIN_X;
	__int16 ORIGIN_Y;
	__int16 gridWidth;

	Mat FilterPaperOriginal;
	Mat FilterPaperThresholded;
	Mat FilterPaperGray;
	Mat FilterPaperOriginal_8BIT;
	Mat FilterPaperGray_8BIT;
	Mat mask;
	Mat FilterPaperEnhanced;
	Mat FilterPaperEnhancedBlurred;
	Mat FilterPaperOriginalBlurred;

	void generateImages();
	void getFilterPaperRegion();
	void setFilterPaperDimension(__int16, __int16, __int16, __int16);
	void setFilterPaperDimension();
	void generateMask();
	vector<vector<Point>> findSpotContours(int min, int max);
	Rect region_of_interest = Rect(ORIGIN_X,ORIGIN_Y, WIDTH, HEIGHT);
};
