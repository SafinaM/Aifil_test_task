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
#include <windows.h>

using namespace cv;
using namespace std;


Mat frame;
Mat image;
vector <Mat> matHist;
Mat H1;
Mat H2;


Mat showColorHist(const Mat &src)  //function of color histrogram forming
{
	//vector<Mat> bgr_planes;  // Separate the image in 3 places ( B, G and R )
	//split(temp, bgr_planes);

	///// Establish the number of bins
	//int histSize = 256;

	///// Set the ranges ( for B,G,R) )
	//float range[] = { 0, 256 };
	//const float* histRange = { range };

	//bool uniform = true; bool accumulate = false;

	//Mat b_hist, g_hist, r_hist;

	///// Compute the histograms:
	//calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	//calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	//calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	//// Draw the histograms for B, G and R
	//int hist_w = 512; int hist_h = 400;
	//int bin_w = cvRound((double)hist_w / histSize);

	//Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	///// Normalize the result to [ 0, histImage.rows ]
	//normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	///// Draw for each channel
	//for (int i = 1; i < histSize; i++)
	//{
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
	//		Scalar(255, 0, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
	//		Scalar(0, 255, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
	//		Scalar(0, 0, 255), 2, 8, 0);
	//}

	Mat hsv;
	
	cvtColor(src, hsv, COLOR_BGR2HSV);
	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int hbins = 30, sbins = 32;
	int histSize[] = { hbins, sbins };
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	MatND hist;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = { 0, 1 };
	calcHist(&hsv, 1, channels, Mat(), // do not use mask
		hist, 2, histSize, ranges,
		true, // the histogram is uniform
		false);
	double maxVal = 0;
	minMaxLoc(hist, 0, &maxVal, 0, 0);
	int scale = 10;
	Mat histImg = Mat::zeros(sbins*scale, hbins * 10, CV_8UC3);
	for (int h = 0; h < hbins; h++)
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal * 255 / maxVal);
			rectangle(histImg, Point(h*scale, s*scale),
				Point((h + 1)*scale - 1, (s + 1)*scale - 1),
				Scalar::all(intensity),
				CV_FILLED);
		}
//	namedWindow("Source", 1);
//	imshow("Source", src);
	namedWindow("H-S Histogram", 1);
	imshow("H-S Histogram", histImg);

	return hist;

	//static int count(0);
	//stringstream ss;
	//ss << count;
	//string countStr = ss.str();
	//string imageName = "hist_" + countStr;
	//string FullPath = imageName + ".jpg";// "D:\\VS2015\\Projects\\OpenCV_test2\\" +
	//imwrite(FullPath, histImg);      //saving histograms
	//cout << " image has been saved " << std::endl;
	//++count;

	// Display
//	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
//	imshow("calcHist Demo", histImg);
}
auto getHist(const Mat &src)
{
		Mat hsv;
		cvtColor(src, hsv, COLOR_BGR2HSV);
		// Quantize the hue to 30 levels
		// and the saturation to 32 levels
		int hbins = 30, sbins = 32;
		int histSize[] = { hbins, sbins };
		// hue varies from 0 to 179, see cvtColor
		float hranges[] = { 0, 180 };
		// saturation varies from 0 (black-gray-white) to
		// 255 (pure spectrum color)
		float sranges[] = { 0, 256 };
		const float* ranges[] = { hranges, sranges };
		MatND hist;
		// we compute the histogram from the 0-th and 1-st channels
		int channels[] = { 0, 1 };
		calcHist(&hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false);
		return hist;
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
		H1 = getHist(image);  //!!! was frame get historfram of definite frame
		matHist.push_back(H1);
		Sleep(100);
//		namedWindow("Frame", CV_WINDOW_AUTOSIZE);
//		imshow("Frame", image);
		showColorHist(image);
	}
}
void formMatHistFromFiles()
{
	int nImages = 26;
	int count = 0;
	stringstream ss;
	ss << count;
	for (int i = 0; i < nImages; i++)
	{
		Mat temp;
		string countStr = ss.str();
		string imageName = "im_" + countStr;
		string FullPath = imageName + ".jpg";// "D:\\VS2015\\Projects\\OpenCV_test2\\" +
		temp = imread(FullPath, 1);
		H1 = getHist(temp);  //get historfram of definite frame
		matHist.push_back(H1);

	}
}

int main()
{
	VideoCapture cap("test1.mp4"); // open the video file for reading

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the video file" << endl;
		return -1;
	}

	//cap.set(CV_CAP_PROP_POS_MSEC, 300); //start the video at 300ms

	double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
	cout << "Frame per seconds : " << fps << endl; //cout frames per seconds

	namedWindow("Video 1 for task from Aifil", CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	setMouseCallback("Video 1 for task from Aifil", onMouse, 0);  //work with mouse
	formMatHistFromFiles();
	while (true)
	{
		bool bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read the frame from video file" << endl;
			break;
		}
		H2 = showColorHist(frame);
		if (!matHist.empty())
		{
			for(auto x: matHist)
			{
				cout << compareHist(x, H2, HISTCMP_CHISQR_ALT) << endl;
				Point center(250, 250);
				if (compareHist(x, H2, HISTCMP_CHISQR_ALT) < 800000)
				{
					circle(frame, center, 30, Scalar(0, 0, 255), -1);
				//	Sleep(300);
				}	
			}
			
		//	cout << compareHist(H1, H2, CV_COMP_CHISQR) << endl;
		//	cout << compareHist(H1, H2, CV_COMP_BHATTACHARYYA) << endl;
		//	cout << compareHist(H1, H2, CV_COMP_INTERSECT) << endl;
		//	Sleep(200);
			
		}
	//	Sleep(50);

		imshow("Video 1 for task from Aifil", (frame)); //show the frame in "Video 1 for task from Aifil" window
		if (waitKey(30) == 27) //waiting for 'esc' key press for 30 ms. If 'Esc' key is pressed, break loop
		{
			cout << "Esc key is pressed by user" << endl;
			break;
		}
	}

//	cin.get();
	return 0;

}