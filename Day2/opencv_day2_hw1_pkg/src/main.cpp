#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

// Opening 함수
Mat myOpen(const Mat& src, const Mat& kernel, int iterations = 1)
{
   Mat eroded, dilated;

   erode(src, eroded, kernel, Point(-1,-1), iterations);
   dilate(eroded, dilated, kernel, Point(-1,-1), iterations);

   return dilated;
}

// Closing 함수
Mat myClose(const Mat& src, const Mat& kernel, int iterations = 1)
{
   Mat dilated, eroded;

   dilate(src, dilated, kernel, Point(-1,-1), iterations);
   erode(dilated, eroded, kernel, Point(-1,-1), iterations);

   return eroded;
}

int main()
{
   Mat image = imread("/home/eungang/Downloads/image1.jpg");

   // 가우시안 블러
   Mat blurred;
   GaussianBlur(image, blurred, Size(7, 7), 0);

   // hsv로
   Mat hsv;
   cvtColor(blurred, hsv, COLOR_BGR2HSV);

   Mat red1, red2, mask_red;
   Mat mask_green, mask_blue;

   // 빨강
   inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), red1);
   inRange(hsv, Scalar(160, 100, 100), Scalar(180, 255, 255), red2);
   bitwise_or(red1, red2, mask_red);

   // 초록
   inRange(hsv, Scalar(30, 100, 120), Scalar(85, 255, 255), mask_green);
   
   // 파랑
   inRange(hsv, Scalar(90, 100, 100), Scalar(130, 255, 255), mask_blue);

   // 커널 생성
   Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));

   // open 후 close
   Mat processed_red = myOpen(mask_red, kernel, 3); 
   Mat final_red = myClose(processed_red, kernel, 3);

   Mat processed_green = myOpen(mask_green, kernel, 5);
   Mat final_green = myClose(processed_green, kernel, 5);

   Mat processed_blue = myOpen(mask_blue, kernel, 3);
   Mat final_blue = myClose(processed_blue, kernel, 3);

   // 윈도우 생성
   namedWindow("Final Red", WINDOW_AUTOSIZE);
   namedWindow("Final Green", WINDOW_AUTOSIZE);
   namedWindow("Final Blue", WINDOW_AUTOSIZE);

   // 이미지 표시
   imshow("Final Red", final_red);
   imshow("Final Green", final_green);
   imshow("Final Blue", final_blue);

   waitKey(0);
   destroyAllWindows();
   return 0;
}
