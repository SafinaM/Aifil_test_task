//Reserching of histograms.
//Video was downloading, you can choose any frame as for template for informaition extraction. 
//In separate window you can see the color histogram of each frame, which you select.
//I want to divide the video on parts with the help of color histogram comparing.
//I'll get 4 basic types of garbage frames: white, gray, almost red and halfly white and red. 
//This algorithm will not be ideal, but it is will be very simple, and I hope working.
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <windows.h>

using namespace cv;
using namespace std;

struct Edge
{
	int x = 0;
	int y = 0;
};
class CutBorders
{
private:
	Mat frame;
	Mat image;
	vector <Mat> matHist;
	vector <Mat> matHistB;
	Mat H1;                      //value for histogram of reference sequence
	Mat H2;                      //histogram of video file frames
	vector <Edge> edges;         //vector with borders of the input video file
	int L, R;                    //temporary values for left and right sides of range
	
public:
	VideoCapture *cap;
    int slider_pos;
	
	CutBorders()
	{
		slider_pos = 0;
		string temp;
		cout << "Enter the name of video file from the root directory: ";  //for example test1.mp4
		cin >> temp;
		cap = new VideoCapture(temp);
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

	auto showColorHist(const Mat &src)  //function of color histrogram forming
	{
		Mat hsv;
		int hbins = 30, sbins = 32;

		MatND hist = getHist(src);

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
		namedWindow("H-S Histogram", 1);
		imshow("H-S Histogram", histImg);
		return hist;
	}

	void initDataFromFile()
	{
		FileStorage out("my_histogram_file.xml", cv::FileStorage::READ);         //downloading results from xml-file
		if (!out.isOpened())
		{
			cout << "Unable to open file storage!" << endl;
		}
		matHist.clear();
		matHistB.clear();
		int frameCount = (int)out["myHist"];
		out["myHist"] >> matHist;         //the vector with the histogrames of white and mixed frames
		out["myHist"] >> matHistB;
		out.release();
	}

	void saveNewDataForBasicFrames()
	{
		FileStorage fs("my_histogram_file.xml", cv::FileStorage::WRITE);
		if (!fs.isOpened())
		{
			cout << "unable to open file storage!" << endl;
		}
		fs << "myHist" << matHist;
		fs << "myHistB" << matHistB;
		fs.release();
	}

	bool comparingHists()
	{
		
		if (!matHist.empty())
		{
			int k = 0;
			for (auto it : matHist)
			{
				k++;
				if (compareHist(it, H2, HISTCMP_CHISQR_ALT) < 800000)
				{
					cout << compareHist(it, H2, HISTCMP_CHISQR_ALT) << " 1 " << k << endl;				
					return true;
				}
			}
		}
		if (!matHistB.empty())
		{
			int k = 0;
			for (auto it : matHistB)
			{
				k++;
				if (compareHist(it, H2, HISTCMP_CHISQR_ALT) < 300000)
				{
	    			cout  << compareHist(it, H2, HISTCMP_CHISQR_ALT) << " 2 " << k << endl;
					return true;
				}
			}
		}
		return false;
	}

	void findBorders()
	{
		int fWidth = (int)cap->get(CV_CAP_PROP_FRAME_WIDTH);
		int fHeight = (int)cap->get(CV_CAP_PROP_FRAME_HEIGHT);
		VideoWriter out_cap("result1.avi", CV_FOURCC('M', 'P', '4', '2'), 30, Size(fWidth, fHeight));
		int nFrames = (int)cap->get(CV_CAP_PROP_FRAME_COUNT);
		while (true)
		{
			bool bSuccess = cap->read(frame); // read a new frame from video

			if (!bSuccess) //if not success, break loop
			{
				cout << "Cannot read the frame from video file" << endl;
				break;
			}
			H2 = showColorHist(frame);  //showing of current frame's histogram in separate window
			int iFrame = (int)cap->get(CV_CAP_PROP_POS_FRAMES);        //number of current frame
			
			if (comparingHists())      //extracting range from separate points, all digits - frame numbers
			{
				int temp = (int)cap->get(CV_CAP_PROP_POS_FRAMES);       //number of current frame of  border
				if (L == 0)        
				{
					L = temp;                  
				}
				else if (temp - L >= 30 && L != 0)
				{
					R = temp;
				}
				
				Point center(250, 250);
				circle(frame, center, 30, Scalar(0, 0, 255), -1);
				temp = 0;
			}
			if ( R == 0 && L != 0 && iFrame - L >= 160 )   //160 frames = 8 seconds
			{
				L = 0;
			}
			if (R - L >= 400)
			{
				R = 0;
				L = 0;
			}
			
			if (R != 0 && L != 0 && iFrame - L >= 300)
			{
				Edge *p = new Edge;
				int deltaL = 40;
				int deltaR = 20;

				if (L - deltaL > 1)
					L -= deltaL;
				else
					L = 1;
				p->x = L;

				if (R + deltaR < nFrames)
					R += deltaR;
				else
					R = nFrames;
				p->y = R;

				edges.push_back(*p);       //vector with frame ranges 
				showResults();             //show results on screen
				L = 0;
				R = 0;
			}

			imshow("Video 1 for task from Aifil", (frame)); //show the frame in "Video 1 for task from Aifil" window
			cvSetTrackbarPos("Slider", "Video 1 for task from Aifil", ++slider_pos);
		
			if (waitKey(1) == 27) //waiting for 'esc' key press for 30 ms. If 'Esc' key is pressed, break loop
			{
				cout << "Esc key is pressed by user" << endl;
				exit(0);
			}
		}
	}

	void workWithMouse(int count)
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
		H1 = getHist(frame);  //!!! get histogram of definite frame
		matHistB.push_back(H1);
		Sleep(100);
		showColorHist(image);
	}

	void showResults()
	{
		int i = 1;
		for (auto it : edges)
		{
			cout << "Frame numbers " << i << ") " << it.x << " - " << it.y << endl;
			i++;
		}
	}
	void openFileSetTrackbar();

	void saveResultsToFile()
	{
		ofstream in("Results.txt");

		int i = 1;
		for (auto it : edges)
		{
			in << "Frame numbers " << i << ") " << it.x << " - " << it.y << endl;
			i++;
		}
	}
};

CutBorders cut;

static void onMouse(int event, int x, int y, int, void*)
{
	static int count(0);
	if (event == EVENT_LBUTTONDOWN)
	{
		cut.workWithMouse(count);
	}
}

void on_trackbar(int, void*)
{
	int position = cut.slider_pos;
	cut.cap->set(CV_CAP_PROP_POS_FRAMES, position);
}

void CutBorders::openFileSetTrackbar()
{
	if (!cap->isOpened())  // if not success, exit program
	{
		cerr << "Cannot open the video file" << endl;
		exit(1);
	}
	int nFrame = (int)cap->get(CV_CAP_PROP_FRAME_COUNT);
//	cap->set(CV_CAP_PROP_POS_FRAMES, 12700);

	namedWindow("Video 1 for task from Aifil", CV_WINDOW_AUTOSIZE); //create a window called "Video 1 for task from Aifil"
	if (nFrame != 0)
	{
		createTrackbar("Position", "Video 1 for task from Aifil", &slider_pos, nFrame, on_trackbar);
	}
	setMouseCallback("Video 1 for task from Aifil", onMouse, 0);  //work with mouse
}

int main()
{
	cut.initDataFromFile();
	cut.openFileSetTrackbar();
	cut.findBorders();
//	cut.saveNewDataForBasicFrames(); //by default write to array with the "black frames"
	cut.showResults();
	cut.saveResultsToFile();
	cin.get();
	return 0;
}