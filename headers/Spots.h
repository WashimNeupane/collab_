#pragma once
#include "opencv2/highgui/highgui.hpp"
#include <opencv2\imgproc.hpp>
#include "../histogram.h"

using namespace cv;
using namespace std;
class Spots
{
public:
	int ID;
	typedef enum
	{
		PASS = 1,
		FAIL = 0,
		INTERMEDIATE = 2
	}SpotAssessment;

	float Circularity;				//circularity = min/max * 100;
	float Convexity;				//convexity = shape_area/hull_area * 100;
	float Consistency;				//consistency = distribution in histogram;
	float Area;						//area = area bounded by the detected boundary for each spot
	float texQualitypc;
	float shapeQualitypc;
	float overallQualitypc;

	BSDhistogram SpotHistogram;
	SpotAssessment TexQuality;
	SpotAssessment ShapeQuality= PASS;
	SpotAssessment OverallQuality;

	//what region is the blood spot in 
	int belongsToSpotCircle;
	vector<bool> inReg[5];

	//identify the 8 differnet types of specimen invalidity criterion as specified in the "BSD_CARDSCAN_SPECIMEN_VALIDITY" file. 
	//bool isInsufficient;
	//bool isScratched;
	//bool isNotDry;
	//bool isSuperSaturated;
	//bool isDiscoloured;

	//bool isClotted;
	//bool isEmpty;					
	//
	////identify some other inconsistencies in the blood spot based on statistical analysis
	//bool isOld;
	//bool isUnevenSaturation;	
	bool isSerum;
	bool isInsufficient;
	bool isNotDry;
	bool isClotted;
	bool isInconsistent;

	Rect SpotBound;
	Rect SetBoundingRectangle();

	Mat spotData;
	vector<Point> Spotcontours;		//coordinates to all the points at the boundary of the spot. Could contain concentric boundary in case of double layered spot
	vector<Point> SpotHull;			//coordinates to the convex hull
	Moments SpotMoments;			//moments for the given bounding shape
	Point2f SpotCenters;			//center of mass of the given bounding shape
	double Spot_min_circle;			//min radius circle centered at the center of mass
	double Spot_max_circle;			//max radius circle centered at the center of mass

	Point2f ComputeMomentCenter(Moments SpotMoments);
	vector<Point> ComputeSpotHull(vector<Point> Spotcontours);
	float ComputeCircularity(Point2f SpotCenters, vector<Point> Spotcontours);
	float ComputeConvexity(vector<Point> SpotHull, Moments SpotMoments);
	float ComputeArea(Moments SpotMoments);
	void ComputeValues();
	void SetConditions(Mat image);
	void Spots::setOverallQuality();
};

inline vector<Spots> sortSpots(vector<Spots> Spotarray)
{
	Spots tempSpotarray;
	for (int i = 0; i < Spotarray.size(); i++)
	{
		// Last i elements are already in place  
		for (int j = 0; j < Spotarray.size() - i - 1; j++) {
			if (Spotarray[j].SpotCenters.x > Spotarray[j + 1].SpotCenters.x)
			{
				tempSpotarray = Spotarray[j];
				Spotarray[j] = Spotarray[j + 1];
				Spotarray[j + 1] = tempSpotarray;
			}
		}
	}
	return Spotarray;
};

