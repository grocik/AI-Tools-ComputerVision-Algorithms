#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/aruco/dictionary.hpp>
#include <opencv2/aruco.hpp>

using namespace std;
using namespace cv;

const double MARKER_MM = 25;
std::vector<cv::Point2f> fromPixelsToMm(std::vector<cv::Point2f> original)
{
    std::vector<cv::Point2f> result;
    double distances = 0;
    for (int i = 0; i < 4; i++) {
        cv::Point2f v = (original[i] - original[(i + 1) % 4]);

        distances += std::sqrt(v.x * v.x + v.y * v.y);
    }
    distances = distances / 4;

    for (int i = 0; i < 4; i++) {
        result.push_back({static_cast<float>(MARKER_MM * (original[i].x / distances)), static_cast<float>(MARKER_MM * (original[i].y / distances))});
    }
    return result;
}

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

        if (ids.size() == 2) {
            auto a = corners[0][0].x;
            auto b = corners[0][0].y;
            auto c = corners[1][0].x;
            auto d = corners[1][0].y;

            cv::line(frame,cv::Point(a,b),cv::Point(c,d),cv::Scalar(255,0,0),2);

            auto found = corners.at(0);
            auto found2 = corners.at(1);
            auto p = fromPixelsToMm(found2)[0];
            auto r = fromPixelsToMm(found)[0];
            auto distance = sqrt(pow(p.x - r.x, 2) + pow(p.y - r.y, 2));
            cout << "distance = " << distance << endl;
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
