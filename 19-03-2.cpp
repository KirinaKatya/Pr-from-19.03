#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main() {
    Mat image = imread("C:/Users/Êàòÿ/Desktop/ÐÇÎ/xilichurl.jpg");
    if (image.empty()) {
        cout << "Failed to load image!" << endl;
        return -1;
    }

    Mat blurredImage;
    GaussianBlur(image, blurredImage, Size(5, 5), 0);

    Mat grayImage;
    cvtColor(blurredImage, grayImage, COLOR_BGR2GRAY);

    Mat binaryImage;
    threshold(grayImage, binaryImage, 0, 255, THRESH_BINARY | THRESH_OTSU);

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    Mat canny_output;
    Canny(grayImage, canny_output, 50, 150);
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    Scalar color = Scalar(0, 255, 0); 
    for (size_t i = 0; i < contours.size(); i++) {
        drawContours(drawing, contours, static_cast<int>(i), color, 1, LINE_8, hierarchy, 0);
    }

    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", image);

    namedWindow("Detected Shapes", WINDOW_NORMAL);
    imshow("Detected Shapes", drawing);

    waitKey(0);
    return 0;
}
