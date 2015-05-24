#include <iostream>
#include <opencv\highgui.h>
#include "opencv\cv.h"

int main() {
	IplImage *frame;
	
	frame = cvLoadImage("test3.png", 0);
	int width = frame->width;
	int height = frame->height;

	IplImage *cvcvAvg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cvtotalAvg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *dcnotch = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cv22 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *lowBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *highBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	cvSet(cv22, cvScalarAll(2));

	cvNamedWindow("Test", 1);
	cvShowImage("Test", frame);

	cvNamedWindow("AvgTest", 1);
	cvNamedWindow("LowBinary", 1);
	cvNamedWindow("HighBinary", 1);
	cvSmooth(frame, cvcvAvg, CV_BLUR, 25);
	cvAvg(frame, cvtotalAvg);
	cvSub(frame, cvcvAvg, dcnotch);
	cvDiv(cvtotalAvg, cv22, cvtotalAvg);
	cvAdd(dcnotch, cvtotalAvg, dcnotch);
	cvShowImage("AvgTest", dcnotch);

	cvThreshold(dcnotch, lowBin, 125.0, 255.0,CV_THRESH_BINARY);
	cvShowImage("LowBinary", lowBin);

	cvThreshold(dcnotch, highBin, 230.0, 255.0, CV_THRESH_BINARY);
	cvShowImage("HighBinary", highBin);
	

	while (1)
	{
		if (cvWaitKey(33) >= 27) break;
	}
	cvDestroyWindow("Test");

	return 0;
}