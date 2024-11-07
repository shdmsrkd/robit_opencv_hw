#include <rclcpp/rclcpp.hpp>
#include <opencv2/opencv.hpp>
#include <cmath>

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("arm_node");

    cv::namedWindow("Manipulator", cv::WINDOW_AUTOSIZE);
    
    cv::Mat background(400, 400, CV_8UC3);    

    double l1 = 75; 
    double l2 = 75;   
    double theta1 = 0.0;
    double theta2 = M_PI/4;
    
    auto timer = node->create_wall_timer( std::chrono::milliseconds(50),[&]() 
        {
            background = cv::Scalar(255, 255, 255);
            
            int x1 = background.cols/2 + l1 * cos(theta1);
            int y1 = background.rows/2 + l1 * sin(theta1);
            
            int x2 = x1 + l2 * cos(theta1 + theta2);
            int y2 = y1 + l2 * sin(theta1 + theta2);
            
            cv::line(background, cv::Point(background.cols/2, background.rows/2), cv::Point(x1, y1), cv::Scalar(0,0,255), 2);
            cv::line(background, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255,0,0), 2);
            
            cv::circle(background, cv::Point(background.cols/2, background.rows/2), 5, cv::Scalar(0,0,0), -1);
            cv::circle(background, cv::Point(x1, y1), 5, cv::Scalar(0,0,0), -1);
            cv::circle(background, cv::Point(x2, y2), 5, cv::Scalar(0,0,0), -1);
            

            theta1 += 0.05;
            theta2 += 0.08;
            
            cv::imshow("Manipulator", background);
            cv::waitKey(1);
        }
    );
    
    rclcpp::spin(node);
    
    rclcpp::shutdown();
    return 0;
}
