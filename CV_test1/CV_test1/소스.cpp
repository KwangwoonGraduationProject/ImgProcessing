#if 0
#include <iostream>
#include "opencv\highgui.h"
#include "opencv\cv.h"


using namespace std;

int main()
{
	IplImage *frame;

	frame = cvLoadImage("test3.png", 0);
	int width = frame->width;
	int height = frame->height;

	IplImage *enhance = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	
	cvNamedWindow("Origin", 1);
	cvShowImage("Origin", frame);
	cvNamedWindow("output", 1);
	cvShowImage("output", enhance);
	

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

#if 0
#include <iostream>
#include <opencv\highgui.h>
#include "opencv\cv.h"
#include <vector>

int main() {
	IplImage *frame;
	
	frame = cvLoadImage("test5.png", 0);
	//cvEqualizeHist(frame, frame);
	int width = frame->width;
	int height = frame->height;

	IplImage *cvcvAvg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cvtotalAvg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *dcnotch = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *cv22 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *lowBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *highBin = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);

	/////////////////////////////////////////////////////////////
	//projection 
	int *hor_proj, *ver_proj;
	hor_proj = new int[height];
	ver_proj = new int[width];
	int hor_max = 0, ver_max = 0;  //각 프로젝션의 최대값 저장
	int hist_view_size = 100;  //히스토그램 막대의 최대 크기 지정
	cvNamedWindow("Horizontal projection", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Horizontal projection", 2 * width, 0);
	cvNamedWindow("Vertical projection", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Vertical projection", width, height);
	/////////////////////////////////////////////////////////////

	cvSet(cv22, cvScalarAll(1.5));

	cvNamedWindow("Test", 1);
	cvShowImage("Test", frame);

	cvNamedWindow("AvgTest", 1);
	cvNamedWindow("LowBinary", 1);
	cvNamedWindow("HighBinary", 1);
	cvNamedWindow("Sub", 1);
	cvNamedWindow("Blur", 1);
	cvSmooth(frame, cvcvAvg, CV_BLUR, 18);
	cvShowImage("Blur", cvcvAvg);
	cvAvg(frame, cvtotalAvg);
	cvSub(frame, cvcvAvg, dcnotch);
	cvShowImage("Sub", dcnotch);
	cvDiv(cvtotalAvg, cv22, cvtotalAvg);
	cvAdd(dcnotch, cvtotalAvg, dcnotch);
	cvLine(dcnotch, cvPoint(100, 100), cvPoint(400, 400), CV_RGB(255, 0, 0));
	cvShowImage("AvgTest", dcnotch);

	//int hold1 = cvMean()

	cvThreshold(frame, lowBin, 50.0, 255.0,CV_THRESH_BINARY_INV);
	cvShowImage("LowBinary", lowBin);

	cvThreshold(dcnotch, highBin, 108.0, 255.0, CV_THRESH_BINARY);
	cvShowImage("HighBinary", highBin);
	
	///////////////////////////////////////////////////////////////
	//vertical, horizontal projection
	//초기화
	for (int i = 0; i<height; ++i)
		hor_proj[i] = 0;
	for (int i = 0; i<width; ++i)
		ver_proj[i] = 0;
	hor_max = 0;
	ver_max = 0;

	//프로젝션 계산
	for (int i = 0; i<height; ++i)
	{
		for (int j = 0; j<width; ++j)
		{
			if (cvGetReal2D(highBin, i, j) != 0) //edge 픽셀이면
			{
				hor_proj[i]++;
				ver_proj[j]++;
				if (hor_proj[i] > hor_max)  //최대값 갱신
					hor_max = hor_proj[i];
				if (ver_proj[j] > ver_max)
					ver_max = ver_proj[j];
			}
		}
	}

	//draw
	IplImage *hor_proj_view, *ver_proj_view;
	hor_proj_view = cvCreateImage(cvSize(hist_view_size, height), 8, 1);
	ver_proj_view = cvCreateImage(cvSize(width, hist_view_size), 8, 1);
	cvZero(hor_proj_view);
	cvZero(ver_proj_view);

	//draw 
	for (int i = 0; i<height; ++i)
	{
		for (int j = 0; j<hor_proj[i] * hist_view_size / hor_max; ++j)  //노멀라이즈 한 값까지
		{
			cvSetReal2D(hor_proj_view, i, j, 255);
		}
	}
	for (int i = 0; i<width; ++i)
	{
		for (int j = 0; j<ver_proj[i] * hist_view_size / ver_max; ++j)  //노멀라이즈 한 값까지
		{
			cvSetReal2D(ver_proj_view, j, i, 255);
		}
	}
	cvShowImage("Horizontal projection", hor_proj_view);
	cvShowImage("Vertical projection", ver_proj_view);
	if (hor_proj_view)
		cvReleaseImage(&hor_proj_view);
	if (ver_proj_view)
		cvReleaseImage(&ver_proj_view);
	///////////////////////////////////////////////////////////////




	while (1)
	{
		if (cvWaitKey(33) >= 27) break;
	}
	cvDestroyWindow("Test");

	return 0;
}
#endif