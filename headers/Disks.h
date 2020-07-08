#pragma once
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

	class Disks
	{
	public:
		Mat Data,hst;
		Rect DiskBoundRect;
		int hhisto[256];
		int shisto[256];
		int vhisto[256];
		int epf[256];
		int PUNCHPATTERN;

		vector<double> statistics;
		vector<Point> PunchCircleCenters;
		vector<Point> cen;

		float RADIUS = 1.6*30;
		float spot_margin =0;
		
		void generateGrid(string);
		//void Disks::DrawPunchLocations(BSDhistogram histogramCollection, Mat src_img);
		void ComputePunchProperties(Mat img);
		Rect GetRegionOfInterest(Point);
		void writeStats2CSV(string filename);

	};

