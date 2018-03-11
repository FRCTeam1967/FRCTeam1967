#include <opencv2/highgui/highgui.hpp>

int main()
{

    cv::Mat img = cv::imread("/usr/local/cuda/samples/3_Imaging/recursiveGaussian/data/lena.ppm");

    cv::imshow("img", img);
    cv::waitKey(); 	

    cv::VideoCapture input(0);


    // cv::VideoCapture input("nvcamerasrc ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720,format=(string)I420, framerate=(fraction)30/1 ! nvvidconv flip-method=0 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");


#if 0
    cv::Mat img(512, 512, CV_8UC3, cv::Scalar(0));

    cv::putText(img,
        "Hello, OpenCV on Jetson!",
        cv::Point(10, img.rows / 2),
        cv::FONT_HERSHEY_DUPLEX,
        1.0,
        CV_RGB(118, 185, 0),
        2);

    cv::imshow("Hello Janksters!", img);
    cv::waitKey(); 	
#endif
}
