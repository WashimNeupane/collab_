#include <fstream>
#include <iostream>
#include "Spots.h"
#include "Disks.h"
#include "Filterpaper.h"
#include "../histogram.h"
#include "../util.h"
#include <ctime>       /* time */

using namespace cv;
using namespace std;
using namespace BSDImaging;

vector<Spots> Spotar;

/** @function main */
int main(int argc, char** argv)
{
	//Read the image from a specified directory. 
	BSDReadImage(argc);
	srand(std::time(0));

	//Define FilterPaper coords      
	newCard.FilterPaperOriginal = image;
	Mat regg;
	newCard.FilterPaperOriginal.copyTo(regg);

	regg.convertTo(regg, -1, 3, 0);
	GaussianBlur(regg, regg, Size(25, 25), 5, 5);
	imwrite("Regg.png", regg);

	newCard.setFilterPaperDimension(0, 5, image.cols, image.rows - 22);
	newCard.getFilterPaperRegion();
	newCard.generateImages();

	//Define the general spotarray for debug
	Spotar = GenerateSpotProperties(newCard);

	//Display DBS properties/categorisation
	DrawContours(Spotar);
	waitKey();
	destroyAllWindows();

	//Compute possible punch locations. 
	GeneratePunchLocations(Spotar);

	printf("Function terminated...\n");
	printf("Press any key to exit console.");
	cin.get();
	return(0);
}

