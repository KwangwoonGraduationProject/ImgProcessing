#if 1
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <iostream>

//define
#define BSize 18
#define K 1.1
#define H1 1.8
#define H2 0.92
#define ImgPath "test3.png"
#define trim 30
#define Wblock 60
#define Hblock 100

int main()
{
#if 1		//이미지파일로 여는 모드
	IplImage *frame = cvLoadImage(ImgPath, 0);
	int width = frame->width;
	int height = frame->height;
	IplImage *blurImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *subImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *dcNotch = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *thresh1 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	IplImage *thresh2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
	CvScalar TotalAvg, hold1, hold2;
	int x, y, temp1, temp2, temp11, temp22;
	int Hist_w1[1000], Hist_w2[1000], Hist_h1[700], Hist_h2[700];
	int Diff_w1[1000], Diff_w2[1000], Diff_h1[700], Diff_h2[700];

#endif		//이미지파일로 여는 모드



	//DC-Notch 이미지 만드는 부분
	cvSmooth(frame, blurImg, CV_BLUR, BSize, BSize);
	cvAbsDiff(frame, blurImg, dcNotch);
	TotalAvg = cvAvg(frame);
	TotalAvg.val[0] *= K;
	cvAddS(dcNotch, TotalAvg, dcNotch);
	
	//이진화 파트
	hold1 = cvAvg(frame);
	hold1.val[0] *= H1;
	hold2 = cvAvg(dcNotch);
	hold2.val[0] *= H2;

	cvThreshold(frame, thresh1, hold1.val[0], 255, CV_THRESH_BINARY);
	cvThreshold(dcNotch, thresh2, hold2.val[0], 255, CV_THRESH_BINARY_INV);

	//Histogram 계산
	//가로 부분
	for (x = 0; x < width; x++)
	{
		Hist_w1[x] = 0;
		Hist_w2[x] = 0;
		for (y = 0; y < height; y++)
		{
			if (cvGetReal2D(thresh1, y, x) > 200)
			{
				Hist_w1[x]++;
			}
			if (cvGetReal2D(thresh2, y, x) > 200)
			{
				Hist_w2[x]++;
			}
		}
	}

	
	//세로 부분
	for (y = 0; y < height; y++)
	{
		Hist_h1[x] = 0;
		Hist_h2[x] = 0;
		for (x = 0; x < width; x++)
		{
			if (cvGetReal2D(thresh1, y, x) > 200)
			{
				Hist_h1[x]++;
			}
			if (cvGetReal2D(thresh2, y, x) > 200)
			{
				Hist_h2[x]++;
			}
		}
	}

	//미분 부분
	//가로
	for (x = 0; x < 100; x++)
	{
		Diff_w1[x] = 0;
		Diff_w2[x] = 0;
	}
	for (x = 100; x < width-100; x++)
	{
		Diff_w1[x] = Hist_w1[x] - Hist_w1[x - 1];
		Diff_w2[x] = Hist_w2[x] - Hist_w2[x - 1];
	}
	for (x = width-100; x < width; x++)
	{
		Diff_w1[x] = 0;
		Diff_w2[x] = 0;
	}
	//세로
	for (x = 0; x < 150; x++)
	{
		Diff_h1[x] = 0;
		Diff_h2[x] = 0;
	}
	for (x = 150; x < height-50; x++)
	{
		Diff_h1[x] = Hist_h1[x] - Hist_h1[x - 1];
		Diff_h2[x] = Hist_h2[x] - Hist_h2[x - 1];
	}
	for (x = height - 50; x < height; x++)
	{
		Diff_h1[x] = 0;
		Diff_h2[x] = 0;
	}

	//미분 블록단위로 나눠서 최고값 없애기
	//가로
	for (x = 0; x < (width / Wblock); x++)
	{
		temp1 = 0;
		temp2 = 0;
		for (y = 0; y < Wblock; y++)
		{
			if (x*Wblock + y >= width) break;
			else
			{
				if (Diff_w1[x*Wblock + y] > temp1)
				{
					temp11 = x*Wblock + y;
					temp1 = Diff_w1[temp11];
				}
				if (Diff_w2[x*Wblock + y] > temp2)
				{
					temp22 = x*Wblock + y;
					temp2 = Diff_w2[temp22];
				}
			}
		}
		cvLine(thresh1, cvPoint(temp11, 0), cvPoint(temp11, height), CV_RGB(255, 255, 255));
		cvLine(thresh2, cvPoint(temp22, 0), cvPoint(temp22, height), CV_RGB(255, 255, 255));
	}


	//세로
	for (x = 0; x < (height / Hblock); x++)
	{
		temp1 = 0;
		temp2 = 0;
		for (y = 0; y < Hblock; y++)
		{
			if (x*Hblock + y >= height) break;
			else
			{
				if (Diff_h1[x*Hblock + y] > temp1)
				{
					temp11 = x*Hblock + y;
					temp1 = Diff_h1[temp11];
				}
				if (Diff_h2[x*Hblock + y] > temp2)
				{
					temp22 = x*Hblock + y;
					temp2 = Diff_h2[temp22];
				}
			}
		}
		cvLine(thresh1, cvPoint(0, temp11), cvPoint(width, temp11), CV_RGB(255, 255, 255));
		cvLine(thresh2, cvPoint(0, temp22), cvPoint(width, temp22), CV_RGB(255, 255, 255));
	}

	cvNamedWindow("Threshold1", 1);
	cvNamedWindow("Threshold2", 1);
	
	cvShowImage("Threshold1", thresh1);
	cvShowImage("Threshold2", thresh2);

	while (1)
	{
		if (cvWaitKey(33) >= 27) break;
	}
}
#endif