#if 0
#include "opencv\highgui.h"
#include "opencv\cv.h"
#include "opencv\cvblob.h"
#include <iostream>

#define K 1.1 
#define Blockk 49
#define TH 0.8
#define TL 0.0000001
#define imgPath "test3.png"

using namespace std;
using namespace cvb;
int main()
{
	IplImage *frame2;
	//CvCapture *capture = cvCaptureFromCAM(0);
	//CvCapture *capture = cvCaptureFromAVI("capture.mpg");
	CvBlobs blob;
    //frame2 = cvQueryFrame(capture);
	frame2 = cvLoadImage(imgPath, 1);
	
	unsigned int numLabel, temp_maxx, temp_maxy, temp_minx, temp_miny, temp_area;
	int width = frame2->width;
	int height = frame2->height;
	int blobSize;
	cout << width << " X " << height << endl;
	IplImage *frame = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *dcNotch = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *blurImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *lowBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *highBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cvavg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cvCons = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *frameLabel2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	IplImage *label = cvCreateImage(cvSize(width, height), IPL_DEPTH_LABEL, 1);
	IplImage *frameLabel;
	cvNamedWindow("realOriginImg", 1);
	cvNamedWindow("DC-Notch", 1);
	cvNamedWindow("HighBinary2", 1);
	cvNamedWindow("LowBinary2", 1);
	cvNamedWindow("Label2", 1);
	cvNamedWindow("Label", 1);
	cvNamedWindow("Sub", 1);
	cvNamedWindow("Label1_2", 1);

	while (1)
	{
		if (cvWaitKey(33) >= 27) break;
		//frame2 = cvQueryFrame(capture);
		cvCvtColor(frame2, frame, CV_RGB2GRAY);
		//cvNot(frame, frame);

		///dc-notch Part
		cvSmooth(frame, blurImg, CV_BLUR, Blockk, Blockk);
		cvNamedWindow("Blur", 1);
		cvShowImage("Blur", blurImg);
		cvSub(frame, blurImg, dcNotch);
		cvAbs(dcNotch, dcNotch);
		cvShowImage("Sub", dcNotch);

		//cvSet(cvCons, cvScalarAll(cvAvg(dcNotch).val[0] * 5));
		//cvAdd(dcNotch, cvCons, dcNotch);
		//cvShowImage("DC-Notch", dcNotch);

		cvEqualizeHist(dcNotch, dcNotch);
		CvScalar avera = cvAvg(dcNotch);
		
		///이진화 Part
		CvScalar cvav = cvAvg(dcNotch);
		cvThreshold(dcNotch, highBin, 100, 255, CV_THRESH_BINARY);

		//cvNot(frame, frame);
		cvAdd(dcNotch, cvCons, dcNotch);
		cvav = cvAvg(dcNotch);
		cvThreshold(dcNotch, lowBin, cvav.val[0]*1.0, 255, CV_THRESH_BINARY_INV);

		///블롭 라벨링 Part
		numLabel = cvLabel(highBin, label, blob);
		
		frameLabel = cvLoadImage(imgPath, 1);
		//frameLabel = cvQueryFrame(capture);
		//cvNot(frameLabel, frameLabel);
		
		cvRenderBlobs(label, blob, frame, frameLabel2);

		blobSize = blob.size();
		for (int i = 1; i < blobSize; i++)
		{
			temp_maxx = blob[i]->maxx;
			temp_maxy = blob[i]->maxy;
			temp_minx = blob[i]->minx;
			temp_miny = blob[i]->miny;
			temp_area = blob[i]->area;
			if (temp_area>500)
				if ((double)(temp_maxy - temp_miny) / (temp_maxx - temp_minx) >= 1.5 && (double)(temp_maxy - temp_miny) / (temp_maxx - temp_minx) <= 3.0)
				{
					cout << "Area : " << temp_area << endl;
					cout << i << " -> " << "maxx : " << temp_maxx << " maxy : " << temp_maxy << " minx : " << temp_minx << " miny : " << temp_miny << endl;
					cvDrawRect(frameLabel, cvPoint(temp_minx, temp_miny), cvPoint(temp_maxx, temp_maxy), CV_RGB(255, 0, 0), 2, 8, 0);
				}
		}

		cvShowImage("Label2", frameLabel2);
		numLabel = cvLabel(lowBin, label, blob);
		cvRenderBlobs(label, blob, frame, frameLabel2);

		cvShowImage("Label", frameLabel);
		cvShowImage("Label1_2", frameLabel2);
		cvShowImage("realOriginImg", frame);
		cvShowImage("DC-Notch", dcNotch);
		cvShowImage("HighBinary2", highBin);
		cvShowImage("LowBinary2", lowBin);
	}


	return 0;
}
#endif


#if 0  //미분시킨거
#include <iostream>
#include "opencv\highgui.h"
#include "opencv\cv.h"
#include <vector>

#define K 1.1 
#define Blockk 50
#define TH 0.8
#define TL 0.0000001

using namespace std;

int main()
{
	IplImage *frame;

	frame = cvLoadImage("test3.png", 0);

	cvNot(frame, frame);

	int width = frame->width;
	int height = frame->height;

	IplImage *temp1 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *temp2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);


	IplImage *dcNotch = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *blurImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *lowBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *highBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cvavg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cvCons = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

	cvSmooth(frame, blurImg, CV_BLUR, Blockk, Blockk);
	/*cvNamedWindow("Blur", 1);
	cvShowImage("Blur", blurImg);*/

	cvSub(frame, blurImg, dcNotch);
	cvAbs(dcNotch, dcNotch);
	/*cvNamedWindow("Sub", 1);
	cvShowImage("Sub", dcNotch);*/

	/*cvSet(cvCons, cvScalarAll(cvAvg(dcNotch).val[0] * 5));
	cvAdd(dcNotch, cvCons, dcNotch);*/

	//cvEqualizeHist(dcNotch, dcNotch);
	//CvScalar avera = cvAvg(dcNotch);

	cvThreshold(dcNotch, highBin, 50, 255, CV_THRESH_BINARY);
	cvThreshold(frame, lowBin, 50, 255, CV_THRESH_BINARY_INV);


	int x, y, x_his[900], y_his[480], x_max = 0, y_max = 0;
	for (x = 0; x < width; x++)
	{
		x_his[x] = 0;
		for (y = 0; y < height; y++)
		{
			if (cvGetReal2D(highBin, y, x) >= 200)
			{
				x_his[x]++;
			}
		}
		if (x_max < x_his[x]) x_max = x_his[x];
		//std::cout << x<<" : " << x_his[x] << std::endl;
	}
	IplImage *xHis = cvCreateImage(cvSize(width, x_max), IPL_DEPTH_8U, 1);
	for (x = 0; x < width; x++)
	{
		cvLine(xHis, cvPoint(x, 0), cvPoint(x, x_his[x]), CV_RGB(0, 0, 0), 1, 8, 0);
	}


	for (y = 0; y < height; y++)
	{
		y_his[y] = 0;
		for (x = 0; x < width; x++)
		{
			if (cvGetReal2D(highBin, y, x) >= 200)
			{
				y_his[y]++;
			}
		}
		if (y_max < y_his[y]) y_max = y_his[y];
		//std::cout << y << " : " << y_his[y] << std::endl;
	}
	IplImage *yHis = cvCreateImage(cvSize(y_max, height), IPL_DEPTH_8U, 1);
	for (y = 0; y < height; y++)
	{
		cvLine(yHis, cvPoint(0, y), cvPoint(y_his[y], y), CV_RGB(0, 0, 0), 1, 8, 0);
	}


	int x_diff[900], y_diff[480], xdMax = 0, ydMax = 0;
	x_diff[0] = 0;
	y_diff[0] = 0;
	for (x = 1; x < width; x++)
	{
		x_diff[x] = abs(x_his[x] - x_his[x - 1]);
		if (xdMax < x_diff[x]) xdMax = x_diff[x];
	}
	for (y = 1; y < height; y++)
	{
		y_diff[y] = abs(y_his[y] - y_his[y - 1]);
		if (ydMax < y_diff[y]) ydMax = y_diff[y];
	}
	cout << xdMax << "&" << ydMax << endl;
	for (x = 0; x < width; x++)
	{
		if (x_diff[x]>(xdMax / 6))
		{
			cvLine(highBin, cvPoint(x, 0), cvPoint(x, height), CV_RGB(255, 255, 255));
		}
	}

	for (y = 0; y < height; y++)
	{
		if (y_diff[y]>(ydMax / 6))
		{
			cvLine(highBin, cvPoint(0, y), cvPoint(width, y), CV_RGB(255, 255, 255));
		}
	}



	///
	x_max = 0;
	y_max = 0;
	for (x = 0; x < width; x++)
	{
		x_his[x] = 0;
		for (y = 0; y < height; y++)
		{
			if (cvGetReal2D(lowBin, y, x) >= 200)
			{
				x_his[x]++;
			}
		}
		if (x_max < x_his[x]) x_max = x_his[x];
		//std::cout << x<<" : " << x_his[x] << std::endl;
	}
	IplImage *xHis2 = cvCreateImage(cvSize(width, x_max), IPL_DEPTH_8U, 1);
	for (x = 0; x < width; x++)
	{
		cvLine(xHis2, cvPoint(x, 0), cvPoint(x, x_his[x]), CV_RGB(0, 0, 0), 1, 8, 0);
	}


	for (y = 0; y < height; y++)
	{
		y_his[y] = 0;
		for (x = 0; x < width; x++)
		{
			if (cvGetReal2D(lowBin, y, x) >= 200)
			{
				y_his[y]++;
			}
		}
		if (y_max < y_his[y]) y_max = y_his[y];
		//std::cout << y << " : " << y_his[y] << std::endl;
	}
	IplImage *yHis2 = cvCreateImage(cvSize(y_max, height), IPL_DEPTH_8U, 1);
	for (y = 0; y < height; y++)
	{
		cvLine(yHis2, cvPoint(0, y), cvPoint(y_his[y], y), CV_RGB(0, 0, 0), 1, 8, 0);
	}


	xdMax = 0;
	ydMax = 0;
	x_diff[0] = 0;
	y_diff[0] = 0;
	for (x = 1; x < width; x++)
	{
		x_diff[x] = abs(x_his[x] - x_his[x - 1]);
		if (xdMax < x_diff[x]) xdMax = x_diff[x];
	}
	for (y = 1; y < height; y++)
	{
		y_diff[y] = abs(y_his[y] - y_his[y - 1]);
		if (ydMax < y_diff[y]) ydMax = y_diff[y];
	}
	cout << xdMax << "&" << ydMax << endl;
	for (x = 0; x < width; x++)
	{
		if (x_diff[x]>(xdMax / 6))
		{
			cvLine(lowBin, cvPoint(x, 0), cvPoint(x, height), CV_RGB(255, 255, 255));
		}
	}

	for (y = 0; y < height; y++)
	{
		if (y_diff[y]>(ydMax / 6))
		{
			cvLine(lowBin, cvPoint(0, y), cvPoint(width, y), CV_RGB(255, 255, 255));
		}
	}
	///

	cvNamedWindow("realOriginImg", 1);
	cvShowImage("realOriginImg", frame);
	cvNamedWindow("DC-Notch", 1);
	cvShowImage("DC-Notch", dcNotch);
	cvNamedWindow("HighBinary2", 1);
	cvShowImage("HighBinary2", highBin);
	cvNamedWindow("LowBinary2", 1);
	cvShowImage("LowBinary2", lowBin);
	cvNamedWindow("XHistogram", 1);
	cvShowImage("XHistogram", xHis);
	cvNamedWindow("YHistogram", 1);
	cvShowImage("YHistogram", yHis);
	cvNamedWindow("XHistogram2", 1);
	cvShowImage("XHistogram2", xHis2);
	cvNamedWindow("YHistogram2", 1);
	cvShowImage("YHistogram2", yHis2);

	while (1)
	{
		if (cvWaitKey(33) >= 27) break;
	}

	return 0;
}
#endif

#if 0
#include "opencv\cv.h"
#include "opencv\highgui.h"

int main()
{
	IplImage *frame;
	CvCapture *capture = cvCaptureFromCAM(0);
	cvNamedWindow("window");

	while (1)
	{
		frame = cvQueryFrame(capture);
		cvShowImage("window", frame);

		if (cvWaitKey(33) >= 27)
			break;
	}

}

#endif