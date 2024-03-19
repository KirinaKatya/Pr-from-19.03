#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

string getShapeName(const vector<Point>& contour) {
    string shapeName;
    double peri = arcLength(contour, true);
    vector<Point> approx;
    approxPolyDP(contour, approx, 0.04 * peri, true);
    int vertices = static_cast<int>(approx.size());
    if (vertices == 3) {
        shapeName = "Triangle";
    }
    else if (vertices == 4) {
        shapeName = "Square";
    }
    else {
        shapeName = "Circle";
    }
    return shapeName;
}

int main() {
    Mat image = imread("C:/Users/Катя/Desktop/Python/figures.png");
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
    Canny(grayImage, canny_output, 50, 90);
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    RNG rng(12345);
    for (size_t i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
        string shapeName = getShapeName(contours[i]);
        Moments M = moments(contours[i]);
        Point centroid(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));
        putText(drawing, shapeName, centroid, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 2);
    }

    namedWindow("Original Image", WINDOW_NORMAL);
    imshow("Original Image", image);

    namedWindow("Detected Shapes", WINDOW_NORMAL);
    imshow("Detected Shapes", drawing);

    waitKey(0);
    return 0;
}
