#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
    Mat background = imread("/home/eungang/Downloads/image2.png");
    Mat watermark = imread("/home/eungang/Downloads/image3.png");

    Rect roi(0, 0, watermark.cols, watermark.rows);
    
    roi.width = min(roi.width, background.cols);
    roi.height = min(roi.height, background.rows);
    
    Mat destinationROI = background(roi);
    
    Mat watermark_roi = watermark(Rect(0, 0, roi.width, roi.height));
    
    addWeighted(destinationROI, 1.0, watermark_roi, 0.5, 0.0, destinationROI);

    namedWindow("Watermarked background", WINDOW_AUTOSIZE);
    imshow("Watermarked background", background);
    waitKey(0);

    return 0;
}
