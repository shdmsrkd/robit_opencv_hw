#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat image = imread("/home/eungang/Downloads/image1.jpg");

    Mat blurred;
    GaussianBlur(image, blurred, Size(7, 7), 0);

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

    // 모폴로지를 위한 커널
    Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));

    Mat final_red, final_green, final_blue;

    // open 후 close (빨강)
    morphologyEx(mask_red, final_red, MORPH_OPEN, kernel, Point(-1,-1), 3);
    morphologyEx(final_red, final_red, MORPH_CLOSE, kernel, Point(-1,-1), 3);

    // open 후 close (초록)
    morphologyEx(mask_green, final_green, MORPH_OPEN, kernel, Point(-1,-1), 5);
    morphologyEx(final_green, final_green, MORPH_CLOSE, kernel, Point(-1,-1), 5);

    // open 후 close (파랑)
    morphologyEx(mask_blue, final_blue, MORPH_OPEN, kernel, Point(-1,-1), 3);
    morphologyEx(final_blue, final_blue, MORPH_CLOSE, kernel, Point(-1,-1), 3);

    // 레이블링
    Mat labels_red, labels_green, labels_blue;
    Mat stats_red, stats_green, stats_blue;
    Mat centroids_red, centroids_green, centroids_blue;

    int num_labels_red = connectedComponentsWithStats(final_red, labels_red, stats_red, centroids_red);
    int num_labels_green = connectedComponentsWithStats(final_green, labels_green, stats_green, centroids_green);
    int num_labels_blue = connectedComponentsWithStats(final_blue, labels_blue, stats_blue, centroids_blue);

    // 결과 이미지 클론
    Mat result_red = image.clone();
    Mat result_green = image.clone();
    Mat result_blue = image.clone();

    // 바운더리 박스
    // 빨강
    for(int i = 1; i < num_labels_red; i++)
    {
        int area = stats_red.at<int>(i, CC_STAT_AREA);
        if(area > 100)
        {
            Rect obj_rect(stats_red.at<int>(i, CC_STAT_LEFT),
                         stats_red.at<int>(i, CC_STAT_TOP),
                         stats_red.at<int>(i, CC_STAT_WIDTH),
                         stats_red.at<int>(i, CC_STAT_HEIGHT));

            rectangle(result_red, obj_rect, Scalar(0, 0, 255), 2);
        }
    }

    // 초록
    for(int i = 1; i < num_labels_green; i++) {
        int area = stats_green.at<int>(i, CC_STAT_AREA);
        if(area > 100) {
            Rect obj_rect(stats_green.at<int>(i, CC_STAT_LEFT),
                         stats_green.at<int>(i, CC_STAT_TOP),
                         stats_green.at<int>(i, CC_STAT_WIDTH),
                         stats_green.at<int>(i, CC_STAT_HEIGHT));
            rectangle(result_green, obj_rect, Scalar(0, 255, 0), 2);
        }
    }

    // 파랑
    for(int i = 1; i < num_labels_blue; i++)

    {
        int area = stats_blue.at<int>(i, CC_STAT_AREA);
        if(area > 100)
        {
            Rect obj_rect(stats_blue.at<int>(i, CC_STAT_LEFT),
                         stats_blue.at<int>(i, CC_STAT_TOP),
                         stats_blue.at<int>(i, CC_STAT_WIDTH),
                         stats_blue.at<int>(i, CC_STAT_HEIGHT));
            rectangle(result_blue, obj_rect, Scalar(255, 0, 0), 2);
        }
    }

    // 결과 표시
    namedWindow("Red Objects", WINDOW_AUTOSIZE);
    namedWindow("Green Objects", WINDOW_AUTOSIZE);
    namedWindow("Blue Objects", WINDOW_AUTOSIZE);

    imshow("Red Objects", result_red);
    imshow("Green Objects", result_green);
    imshow("Blue Objects", result_blue);

    waitKey(0);
    destroyAllWindows();
    return 0;
}
