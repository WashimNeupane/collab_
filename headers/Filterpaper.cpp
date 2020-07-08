#include "Filterpaper.h"

void FilterPaper::generateImages()
{
generateMask();
}

void FilterPaper::getFilterPaperRegion()
{
	FilterPaperOriginal = FilterPaperOriginal(Rect(ORIGIN_X, ORIGIN_Y, WIDTH, HEIGHT));
}

void FilterPaper::setFilterPaperDimension(__int16 x, __int16 y, __int16 width, __int16 height)
{
	WIDTH = width;
	HEIGHT= height;
	ORIGIN_X = x;
	ORIGIN_Y = y;
	gridWidth = WIDTH / 5;
}

void FilterPaper::setFilterPaperDimension()
{
	WIDTH  = FilterPaperOriginal.cols - 10;
	HEIGHT = FilterPaperOriginal.rows - 10;
	ORIGIN_X = 10;
	ORIGIN_Y = 10;
	gridWidth = WIDTH / 5;
}

void FilterPaper::generateMask()
{
	extractChannel(FilterPaperOriginal, mask, 0);
	mask = mask < 25000;
}

vector<vector<Point>> FilterPaper::findSpotContours(int min, int max)
{
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	if (max == 0) { max = 10000000000; }
	//Find contours that have sizable areas. Here, sizable area = 5000;
	findContours(mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	int areaCount = 0; int maxArea = 0;
	do
	{
		double area = contourArea(contours[areaCount], false);  //  Find the area of contour
		if (area < min || area>max)
		{
			contours.erase(contours.begin() + areaCount);
			if (area > maxArea) {
				maxArea = areaCount;
			}
			areaCount--;
		}

		areaCount++;
	} while (areaCount != contours.size());
	return contours;
}