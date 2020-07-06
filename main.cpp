<<<<<<< HEAD
#include <opencv2/highgui.hpp>
#include <iostream>
 
int main( int argc, char** argv ) {
  
  cv::Mat image;
  image = cv::imread("sample.jpeg");
  
  if(! image.data ) {
      std::cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }
  
  cv::namedWindow( "Display window", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Display window", image );
  
  cv::waitKey(0);
  return 0;
}
=======
//Authors: Washim S. Neupane
>>>>>>> 31b33e75543d78db6c7beac5ba020a8860d8dd27
