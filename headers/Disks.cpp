#include "Disks.h"
#include "../histogram.h"

void Disks::generateGrid(string TYPE)
{
	if (TYPE == "HEXAGONAL") {
		PUNCHPATTERN = 0;
		Rect rectangle = DiskBoundRect;

		int width = rectangle.x + rectangle.width;
		int height = rectangle.y + rectangle.height;

		bool isEvenRow = false; int count = 0;
		float HexRadius_noMargin = RADIUS;
		float s = (2 * (HexRadius_noMargin + spot_margin)) / (sqrt(3));
		float HexRadius_Margin = HexRadius_noMargin + spot_margin;
		float x0 = HexRadius_Margin + rectangle.x;
		float y0 = s / 2 + rectangle.y;

		for (float i = y0; i < height + RADIUS / 2; i = i + HexRadius_Margin)
		{
			if (count % 2 == 0) { isEvenRow = true; }
			else { isEvenRow = false; }

			if (isEvenRow == true)
			{
				for (float j = x0; j <= width + RADIUS / 2; j = j + (3 * s))
				{
					PunchCircleCenters.push_back(Point(j, i));
				}
			}
			else
			{
				for (float j = x0 + (3 * s) / 2; j <= width + RADIUS / 2; j = j + (3 * s))
				{
					PunchCircleCenters.push_back(Point(j, i));
				}
			}
			count++;
		}
	}
	else if (TYPE == "RECTANGULAR")
	{
		PUNCHPATTERN = 1;
		Rect rectangle = DiskBoundRect;
		int width = rectangle.x + rectangle.width;
		int height = rectangle.y + rectangle.height;

		int x0, y0;
		x0 = DiskBoundRect.x + (RADIUS / 2); y0 = DiskBoundRect.y + (RADIUS / 2);

		for (int i = x0; i <= width - (RADIUS / 2); i = i + (RADIUS / 5))
		{
			for (int j = y0; j <= height - (RADIUS / 2); j = j + (RADIUS / 5))
			{
				PunchCircleCenters.push_back(Point(i, j));
			}
		}
	}
}




void Disks::ComputePunchProperties(Mat src_img)
{
	BSDhistogram PunchAreaTexture;
	double stat;

	for (int i = 0; i < PunchCircleCenters.size(); i++)
	{
		Rect rrr = GetRegionOfInterest(PunchCircleCenters[i]);
		Mat roi = src_img(rrr);
		Data = roi;
		stat = PunchAreaTexture.ComputeRegionConsistency(roi);
		if (stat != 0) {
			statistics.push_back(stat);
			cen.push_back(PunchCircleCenters[i]);
		}
		//rectangle(src_img, rrr, Scalar(55, 65, 65), 2, 8, 0);
		//imshow("I", src_img);
		//waitKey();
	}
}

void Disks::writeStats2CSV(string filename)

{
	ofstream myfile;
	myfile.open(filename.c_str());
	myfile << cv::format(Mat(statistics), cv::Formatter::FMT_CSV) << std::endl;
	myfile.close();
}

Rect Disks::GetRegionOfInterest(Point pt)
{
	float radius = RADIUS;
	Point cen = pt;
	Rect r(cen.x - (radius), cen.y - (radius), 2 * radius, 2 * radius);
	return r;
}

