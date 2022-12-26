#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/aruco/dictionary.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture camera(0);
    if (!camera.isOpened())
    {
        cerr << "Nie udało się otworzyć kamery!" << endl;
        return 1;
    }
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams = cv::aruco::DetectorParameters::create();

    Mat frame;
    while (true)
    {

        camera >> frame;
        if (frame.empty())
            break;

        std::vector<int> ids;
        std::vector<std::vector<cv::Point2f>> corners;
        cv::aruco::detectMarkers(frame, dictionary, corners, ids, detectorParams);

        if (ids.size() >= 2) {
            cv::Point2f p1 = corners[0][0];
            cv::Point2f p2 = corners[1][0];

            float distance = cv::norm(p1 - p2);
            std::cout << "Odległość między markrami wynosi: " << distance << std::endl;
        }

        cv::aruco::drawDetectedMarkers(frame, corners, ids);

        imshow("Kamera", frame);

        int key = waitKey(1);
        if (key == 27) // klawisz Esc
            break;
    }
    camera.release();
    destroyAllWindows();
    return 0;
}