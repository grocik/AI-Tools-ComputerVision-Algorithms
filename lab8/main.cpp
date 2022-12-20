#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;/

int main()
{
    VideoCapture camera(0);
    if (!camera.isOpened())
    {
        cerr << "Nie udało się otworzyć kamery!" << endl;
        return 1;
    }

    Mat frame;
    while (true)
    {
        camera >> frame;
        if (frame.empty())
            break;

        Vec3b targetColor(255, 0, 0); // niebieski

        Mat mask;
        inRange(frame, targetColor - Vec3b(50, 50, 50), targetColor + Vec3b(50, 50, 50), mask);

        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        sort(contours.begin(), contours.end(), [](const vector<Point> &c1, const vector<Point> &c2)
             { return contourArea(c1) > contourArea(c2); });

        vector<vector<Point>> largestContours;
        for (int i = 0; i < min(2, (int)contours.size()); i++)
            largestContours.push_back(contours[i]);

        bool inLine = true;
        int margin = 5;

        for (int i = 0; i < largestContours.size(); i++)
        {
            Rect rect = boundingRect(largestContours[i]);
            if (abs(rect.y - largestContours[0][0].y) > margin)
            {
                inLine = false;
            }

            // Sprawdzamy, czy kontur znajduje się w jednej poziomej linii
            // if (abs(rect.y - largestContours[0].y) > margin){
            //     inLine = false;
            // }
            if (inLine)
            {
                Rect rect1 = boundingRect(largestContours[0]);
                Rect rect2 = boundingRect(largestContours[1]);

                int centerX = (rect1.x + rect2.x + rect2.width) / 2;
                int centerY = (rect1.y + rect1.height / 2);

                for (int i = 0; i < largestContours.size(); i++)
                {
                    if (i % 2 == 0)
                    {
                        // Indeks parzysty - rysujemy 2 kreski
                        line(frame, Point(centerX, centerY - 10), Point(centerX, centerY + 10), Scalar(255, 255, 255), 2);
                        line(frame, Point(centerX - 10, centerY), Point(centerX + 10, centerY), Scalar(255, 255, 255), 2);
                    }
                    else
                    {
                        // Indeks nieparzysty - rysujemy 3 kreski
                        line(frame, Point(centerX, centerY - 10), Point(centerX, centerY + 10), Scalar(255, 255, 255), 2);
                        line(frame, Point(centerX - 10, centerY), Point(centerX + 10, centerY), Scalar(255, 255, 255), 2);
                        line(frame, Point(centerX, centerY), Point(centerX + 10, centerY - 10), Scalar(255, 255, 255), 2);
                    }
                }
            }
            imshow("Kamera", frame);
            int key = waitKey(1);
            if (key == 27)
                break;
        }

        
    }
    return 0;
}
