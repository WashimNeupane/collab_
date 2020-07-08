#include "Spots.h"

Point2f Spots::ComputeMomentCenter(Moments mu) {
	SpotCenters = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
	return SpotCenters;
}

float Spots::ComputeCircularity(Point2f mc,vector<Point> contours) 
{
    ///  Get the mass centers:
    float min_circle =100000;
    float max_circle =0;

    //Circularity
    for (int i = 0; i < contours.size(); i++)
    {
        Point2f ds;
        Point2f norm2val;
        vector<double> eucledian_Dist(contours.size(),0);

        ds = contours[i];
        norm2val = ds - mc;

        eucledian_Dist[i] = (sqrt(pow((norm2val.x), 2) + pow((norm2val.y), 2)));

        if (eucledian_Dist[i] < min_circle) { min_circle = eucledian_Dist[i]; }
        if (eucledian_Dist[i] > max_circle) { max_circle = eucledian_Dist[i]; }
    }
    Circularity = ((min_circle / max_circle));
    Spot_min_circle = min_circle;
    Spot_max_circle = max_circle;

    if (Circularity < 0.45) 
    {
        ShapeQuality = FAIL;
    }
    return Circularity;
}

float Spots::ComputeArea(Moments mu) 
{
    Area = mu.m00* ((89.0*21.0)/(600* 2400));
    if (Area < 40 || Area >250)
    { ShapeQuality = FAIL; 
    isInsufficient = true; }

    shapeQualitypc = ((1 - isInsufficient) * (Circularity + Convexity))/2;
    return Area;
}

vector<Point> Spots::ComputeSpotHull(vector<Point> contours) 
{
    //convex hulls
    convexHull(contours, SpotHull, false);
    return SpotHull;
}

float Spots::ComputeConvexity(vector<Point> hull,Moments mu) 
{
    double HullArea;
    HullArea = contourArea(hull);
    Convexity = (pow(((mu.m00) / HullArea), 2));

    if (Convexity < 0.80)
    { ShapeQuality = FAIL; }
    return Convexity;
}

void Spots::ComputeValues()
{
    SpotMoments= moments(Spotcontours, true);
    ComputeMomentCenter(SpotMoments);
    ComputeCircularity(SpotCenters, Spotcontours);
    ComputeSpotHull(Spotcontours);
    ComputeConvexity(SpotHull,SpotMoments);
    ComputeArea(SpotMoments);
}

void Spots::SetConditions(Mat image)
{
    double sensitivity;
    bool isPreprocessEnabled = true;
    SpotHistogram.setBitLength(8);
    SpotHistogram.initialiseHistogram();

    if(SpotHistogram.bits == 256)
    {
        SpotHistogram.ComputeHistogram(image, 150, isPreprocessEnabled);
        SpotHistogram.smoothHistogram();
        SpotHistogram.findHistogramMode();
        sensitivity = SpotHistogram.mode * 0.01;
    }
    else if (SpotHistogram.bits==65536)
    {
        SpotHistogram.ComputeHistogram(image, 25000, isPreprocessEnabled);
        SpotHistogram.smoothHistogram();
        SpotHistogram.findHistogramMode();
        sensitivity = SpotHistogram.mode * 0.5;
    }

    SpotHistogram.findHistogramPeaks(sensitivity);

    //Find the dominant peak. 
    SpotHistogram.findDominantPeak();
    SpotHistogram.findMeanStd();
    printf("MODE : %i \n", SpotHistogram.histogramData[SpotHistogram.dominant_peak_index]);
    SpotHistogram.computeRMSE();

    printf("Number of peaks detected in histogram = %d \n", (int)SpotHistogram.histogramPeaks[0].size());
    for (int i = 0; i < SpotHistogram.histogramPeaks[0].size(); i++)
    {
        printf("PEAK %i index : %i \n", i + 1, (int)SpotHistogram.histogramPeaks[0][i]);
    }

    //Check if there are more than 1 peak. If n(peak) != 1, then blood must be layered/inconsistent 
    if (SpotHistogram.histogramPeaks[0].size() > 1 1
        ) 
    {
            isInconsistent = true;
    }

    Consistency = ((1-(SpotHistogram.stdd / (255/6)))/((int)SpotHistogram.histogramPeaks[0].size()));
    printf("CONSISTENCT OF SPOT : %.2f \n", Consistency);

    //Check if blood is clotted
    float sigmoid_c = 100 / (1 + exp(0.1 * ((int)SpotHistogram.histogramPeaks[0][0] - 120)));
    if (SpotHistogram.histogramPeaks[0].size() > 1 || SpotHistogram.cumulative_histogramData[(int)SpotHistogram.histogramPeaks[0][0]] > 0.01 * SpotHistogram.mode) {
        //isInconsistent = true;
        if (SpotHistogram.cumulative_histogramData[(int)SpotHistogram.histogramPeaks[0][0]] > 0.05 * (SpotHistogram.cumulative_histogramData[255]))
        {
            float clot_area;
            clot_area = SpotHistogram.cumulative_histogramData[(int)SpotHistogram.histogramPeaks[1][1]];
            clot_area = (clot_area / SpotHistogram.cumulative_histogramData[255]) * 100;
            printf("Significant regions of supersaturtion detected. Could be clots or layers \n \n");
            printf("The total area that is covered by the first peak region : %.2f   percent \n", clot_area);
        }

        printf("Probability of first peak being a clot                  : %.2f   percent", sigmoid_c);
        if (sigmoid_c < 50)
        {
            printf("(statistically insignificant) \n");
        }
        else {
            printf("(statistically significant) \n");
            isClotted = true;
        }
    }

    //Check if blood is diluted
    float sigmoid_a = 100 / (1 + exp(-0.1 * ((int)SpotHistogram.histogramPeaks[0][SpotHistogram.histogramPeaks[0].size() - 1] - 200)));
    printf("Probability of last peak being diluted                  : %.2f   percent", (sigmoid_a));
    if (sigmoid_a < 50)
    {
        printf("(statistically insignificant) \n");
    }
    else {
        printf("(statistically significant) \n");
        int idxx = SpotHistogram.histogramPeaks[1][SpotHistogram.histogramPeaks[0].size() - 1];
        if (SpotHistogram.cumulative_histogramData[idxx] > 0.75 * SpotHistogram.cumulative_histogramData[255]) { isSerum = true; }
        else{isNotDry = true;}
    }

    printf("\nCLOTTED               :  "); printf(isClotted ? "true" : "false");
    printf("\nINCONSISTENT/LAYERED  :  "); printf(isInconsistent ? "true" : "false");
    printf("\nDILUTED               :  "); printf(isNotDry ? "true" : "false");
    printf("\nHAS SERUM             :  "); printf(isSerum ? "true" : "false");

    if (isClotted == false && isInconsistent == false && isNotDry == false && isSerum ==false)
    {
        TexQuality = PASS;
        printf("\nBLOOD SPOT ASSESSMENT :  PASS \n");
    }
    else
    {
        TexQuality = FAIL;
        printf("\nBLOOD SPOT ASSESSMENT :  FAIL \n");
    }

    printf("\n\n************************************************************************************************ \n");


    texQualitypc = (TexQuality * Consistency);
}

void Spots::setOverallQuality()
{
    if (TexQuality != FAIL && ShapeQuality != FAIL) { OverallQuality = PASS;}
    else { OverallQuality = FAIL;}
    overallQualitypc = (shapeQualitypc + texQualitypc) / 2;
}

Rect Spots::SetBoundingRectangle()
{
 
    int npoints = Mat(Spotcontours).checkVector(2);
    int  xmin = 0, ymin = 0, xmax = -1, ymax = -1, i;
    Point ptxmin, ptymin, ptxmax, ptymax;

    if (npoints == 0)
        "EMPTY";

    const Point* pts = Mat(Spotcontours).ptr<Point>();
    Point pt = pts[0];

    ptxmin = ptymin = ptxmax = ptymax = pt;
    xmin = xmax = pt.x;
    ymin = ymax = pt.y;

    for (i = 1; i < npoints; i++)
    {
        pt = pts[i];

        if (xmin > pt.x)
        {
            xmin = pt.x;
            ptxmin = pt;
        }

        if (xmax < pt.x)
        {
            xmax = pt.x;
            ptxmax = pt;
        }

        if (ymin > pt.y)
        {
            ymin = pt.y;
            ptymin = pt;
        }

        if (ymax < pt.y)
        {
            ymax = pt.y;
            ptymax = pt;
        }
    }
    SpotBound = Rect(xmin, ymin, xmax - xmin + 1, ymax - ymin + 1);
    return SpotBound;
}

