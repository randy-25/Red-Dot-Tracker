#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
    VideoCapture cap("Red Dot.mp4"); 

    if (!cap.isOpened())
    {
        cout << "Video tidak ditemukan" << endl;
        return -1;
    }
    namedWindow("reddot", WINDOW_FULLSCREEN);

    while (true)
    {
        Mat frame;
        bool framevid = cap.read(frame);

        //meresize ukuran video menjadi lebih kecil
        double scale_down = 0.5;
        Mat frame_down;
        resize(frame, frame_down, Size(), scale_down, scale_down, INTER_LINEAR);

        Mat hsv; //mengubah video menjadi HSV
        cvtColor(frame_down, hsv, COLOR_BGR2HSV);

        Scalar lower = Scalar(0, 100, 100); //Memberikan batas atas dan bawah untuk treshold agar warna merah dapat terdeteksi
        Scalar upper = Scalar(10, 255, 255);

        Mat mask;
        inRange(hsv, lower, upper, mask); //treshold gambar untuk mendeteksi merah


        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); //mencari contournya

        //mencari index contour terbesar untuk penggambaran lingkaran di sekitar red dot
        int largest_contour_index = 0;
        int largest_contour_size = 0;
        for (int i = 0; i < contours.size(); i++)
        {
            int size = contours[i].size();
            if (size > largest_contour_size)
            {
                largest_contour_index = i;
                largest_contour_size = size;
            }
        }
        Point2f center;
        float radius;
        minEnclosingCircle(contours[largest_contour_index], center, radius); //mencari radius dan pusat lingkaran dari red dot
        circle(frame_down, center, radius, Scalar(68, 255, 0), 2); //menggambar lingkaran dengan warna hijau

        imshow("reddot", frame_down);

        // Break ketika esc ditekan
        if (waitKey(10) == 27)
            break;
    }
    cap.release();
    destroyAllWindows();
    return 0;
}