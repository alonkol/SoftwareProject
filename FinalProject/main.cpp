#include <iostream>
#include "SPImageProc.h"
#include <opencv2/imgproc.hpp>//calcHist
#include <opencv2/core.hpp>//Mat
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
using namespace cv;

int main()
{
    Mat src;

	/// Load image
	src = imread("./images/img6.png", CV_LOAD_IMAGE_COLOR);
    imshow("img", src);
    waitKey(0);

    return 0;
}
