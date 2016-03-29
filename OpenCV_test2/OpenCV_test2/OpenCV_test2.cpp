//Reserching of histograms.
//Video was downloading, you can choose any frame as for template for informaition extraction. 
//In separate window you can see the color histogram of each frame, which you select.
//I want to divide the video on parts with the help of color histogram comparing.
//I'll get 4 basic types of garbage frames: white, gray, almost red and halfly white and red. 
//This algorithm will not be ideal, but it is will be very simple, and I hope working.
#include <cstdlib>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


Mat frame;
Mat image;

void Saving()
{

}


void showColorHist(const Mat &temp)  //function of color histrogram forming
{
	vector<Mat> bgr_planes;  // Separate the image in 3 places ( B, G and R )
	split(temp, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}
	static int count(0);
	stringstream ss;
	ss << count;
	string countStr = ss.str();
	string imageName = "hist_" + countStr;
	string FullPath = imageName + ".jpg";// "D:\\VS2015\\Projects\\OpenCV_test2\\" +
	imwrite(FullPath, histImage);      //saving histograms
	cout << " image has been saved " << std::endl;
	++count;

	// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);
}

static void onMouse(int event, int x, int y, int, void*)  //getting the frame from left button press
{
	static int count(0);
	if (event == EVENT_LBUTTONDOWN) 
	{

		stringstream ss;
		ss << count;
		string countStr = ss.str();
		string imageName = "image_" + countStr;
		string FullPath = imageName + ".jpg";// "D:\\VS2015\\Projects\\OpenCV_test2\\" +
		imwrite(FullPath, frame);
		cout << " image has been saved " << std::endl;
		++count;
		image = imread(FullPath, 1);
		namedWindow("Frame", CV_WINDOW_AUTOSIZE);
		imshow("Frame", image);
		showColorHist(image);
	}
}


int main()
{
	VideoCapture cap("test3.mp4"); // open the video file for reading

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
	cout << "Frame per seconds : " << fps << endl; //cout frames per seconds

	namedWindow("Video 1 for task from Aifil", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	cv::setMouseCallback("Video 1 for task from Aifil", onMouse, 0);  //work with mouse

	while (true)
	{
		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}

		imshow("Video 1 for task from Aifil", frame); //show the frame in "Video 1 for task from Aifil" window
		if (waitKey(30) == 27) //waiting for 'esc' key press for 30 ms. If 'Esc' key is pressed, break loop
		{
			cout << "Esc key is pressed by user" << endl;
			break;
		}
	}

//	cin.get();
	return 0;

}