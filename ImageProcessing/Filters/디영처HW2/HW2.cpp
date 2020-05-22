#include <opencv2\opencv.hpp>
#include <iostream>
#include <algorithm>
#define HEIGHT 256
#define WIDTH 256

using namespace std;
using namespace cv;

void histogramEqualization(unsigned char**,unsigned char**);
void AverageFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void SmoothFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth);
void SharpenFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth);
void MedianFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);

//2차원 배열을 동적할당
unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal) {
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++) {
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}

//2차원 배열 할당해제
void MemFree2D(unsigned char** mem, int h) {
	for (int n = 0; n < h; n++) { delete[] mem[n]; }
	delete[] mem;
}

//원본 영상의 가장자리 값을 필터 크기에 맞추어 확장
unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++) {//원본 데이터를 Pad의 중앙에 복사
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}

	for (int h = 0; h < nPadSize; h++) {//위 아래 가장자리 값 복사
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++) {//좌, 우 가장자리 값 복사
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}

	for (int h = 0; h < nPadSize; h++) {//4개의 귀퉁이 값 복사
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}
	return Pad;
}
int main() {
	unsigned char **lenaY;//원본 영상 데이터
	unsigned char **equalizedLena;//Histogram equalization을 적용한 데이터
	unsigned char **aveFilteredLenna;
	unsigned char **smoothFilteredLenna;
	unsigned char **sharpenFilteredLenna;
	unsigned char **medianFilteredLenna;
	unsigned char **pad;
	FILE *fpIn, *fpOut;
	int medianFilterSize = 0;

	//////////////////Histogram Equalization////////////////////
	fpIn = fopen("input.raw", "rb");
	lenaY = MemAlloc2D(HEIGHT, WIDTH, 0);
	for (int h = 0; h < HEIGHT; h++)
		fread(lenaY[h], sizeof(unsigned char), WIDTH, fpIn);
	fpOut = fopen("EQlena.raw", "wb");
	equalizedLena = MemAlloc2D(HEIGHT, WIDTH, 0);
	histogramEqualization(lenaY, equalizedLena);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(equalizedLena[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpIn);
	fclose(fpOut);
	
	//////////////////Average Filter/////////////////
	fpIn = fopen("lena256.raw", "rb");
	lenaY = MemAlloc2D(HEIGHT, WIDTH, 0);
	for (int h = 0; h < HEIGHT; h++)
		fread(lenaY[h], sizeof(unsigned char), WIDTH, fpIn);	
	aveFilteredLenna = MemAlloc2D(HEIGHT, WIDTH, 0);
	///////////Filter1//////////
	fpOut = fopen("AverageLenaFilter1.raw", "wb");
	pad = Padding(lenaY, HEIGHT, WIDTH, 3);
	AverageFilter(pad, aveFilteredLenna, HEIGHT, WIDTH, 3);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(aveFilteredLenna[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpOut);
	MemFree2D(pad, HEIGHT + 2);
	///////////Filter2//////////
	fpOut = fopen("AverageLenaFilter2.raw", "wb");
	pad = Padding(lenaY, HEIGHT, WIDTH, 7);
	AverageFilter(pad, aveFilteredLenna, HEIGHT, WIDTH, 7);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(aveFilteredLenna[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpOut);
	MemFree2D(pad, HEIGHT + 6);

	/////////////////Smooth Filter////////////////
	fpOut = fopen("SmoothFilteredLena.raw", "wb");
	smoothFilteredLenna = MemAlloc2D(HEIGHT, WIDTH, 0);
	pad = Padding(lenaY, HEIGHT, WIDTH, 3);	
	SmoothFilter(pad, smoothFilteredLenna, HEIGHT, WIDTH);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(smoothFilteredLenna[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpOut);
	MemFree2D(pad, HEIGHT + 2);
	fclose(fpIn);

	/////////////////Sharpen Filter/////////////////
	fpIn = fopen("AverageLenaFilter1.raw", "rb");
	fpOut = fopen("SharpenFilterLena.raw", "wb");
	for (int h = 0; h < HEIGHT; h++)
		fread(lenaY[h], sizeof(unsigned char), WIDTH, fpIn);
	sharpenFilteredLenna = MemAlloc2D(HEIGHT, WIDTH, 0);
	pad = Padding(lenaY, HEIGHT, WIDTH, 3);
	SharpenFilter(pad, sharpenFilteredLenna, HEIGHT, WIDTH);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(sharpenFilteredLenna[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpIn);
	fclose(fpOut);
	MemFree2D(pad, HEIGHT + 2);

	/////////////////Median Filter////////////////
	cout << "Input Median filter size : ";
	cin >> medianFilterSize;
	/////////////////Median filter for lena256_n5////////////////
	fpIn = fopen("lena256_n5.raw", "rb");
	fpOut = fopen("MedianLena_n5.raw", "wb");
	for (int h = 0; h < HEIGHT; h++)
		fread(lenaY[h], sizeof(unsigned char), WIDTH, fpIn);
	medianFilteredLenna = MemAlloc2D(HEIGHT, WIDTH, 0);
	pad = Padding(lenaY, HEIGHT, WIDTH, medianFilterSize);
	MedianFilter(pad, medianFilteredLenna, HEIGHT, WIDTH, medianFilterSize);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(medianFilteredLenna[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpIn);
	fclose(fpOut);
	MemFree2D(pad, HEIGHT + medianFilterSize - 1);
	MemFree2D(medianFilteredLenna, HEIGHT);
	/////////////////Median filter for lena256_n10////////////////
	fpIn = fopen("lena256_n10.raw", "rb");
	fpOut = fopen("MedianLena_n10.raw", "wb");
	for (int h = 0; h < HEIGHT; h++)
		fread(lenaY[h], sizeof(unsigned char), WIDTH, fpIn);
	medianFilteredLenna = MemAlloc2D(HEIGHT, WIDTH, 0);
	pad = Padding(lenaY, HEIGHT, WIDTH, medianFilterSize);
	MedianFilter(pad, medianFilteredLenna, HEIGHT, WIDTH, medianFilterSize);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(medianFilteredLenna[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpIn);
	fclose(fpOut);
	MemFree2D(pad, HEIGHT + medianFilterSize - 1);
	MemFree2D(medianFilteredLenna, HEIGHT);

	/////////////////Median filter for lena256_n25////////////////
	fpIn = fopen("lena256_n25.raw", "rb");
	fpOut = fopen("MedianLena_n25.raw", "wb");
	for (int h = 0; h < HEIGHT; h++)
		fread(lenaY[h], sizeof(unsigned char), WIDTH, fpIn);
	medianFilteredLenna = MemAlloc2D(HEIGHT, WIDTH, 0);
	pad = Padding(lenaY, HEIGHT, WIDTH, medianFilterSize);
	MedianFilter(pad, medianFilteredLenna, HEIGHT, WIDTH, medianFilterSize);
	for (int h = 0; h < HEIGHT; h++)
		fwrite(medianFilteredLenna[h], sizeof(unsigned char), WIDTH, fpOut);
	fclose(fpIn);
	fclose(fpOut);
	MemFree2D(pad, HEIGHT + medianFilterSize - 1);
	MemFree2D(medianFilteredLenna, HEIGHT);

	MemFree2D(lenaY,HEIGHT);
	MemFree2D(equalizedLena, HEIGHT);
	MemFree2D(aveFilteredLenna, HEIGHT);
	MemFree2D(smoothFilteredLenna, HEIGHT);
	MemFree2D(sharpenFilteredLenna, HEIGHT);
	return 0;
}

void histogramEqualization(unsigned char **original, unsigned char **eqImg)
{										//원본           eq결과를 담을 변수
	int histogram[256];
	double Hist_CDF[256] = { 0.0 };

	memset(histogram, 0, sizeof(int) * 256);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			histogram[original[h][w]]++;
		}
	}

	//누적 분포 함수
	Hist_CDF[0] = (double)histogram[0] / (HEIGHT * WIDTH);
	for (int n = 1; n < 256; n++) {
		Hist_CDF[n] = (double)histogram[n] / (HEIGHT * WIDTH) + Hist_CDF[n - 1];		
	}

	for (int n = 0; n < 256; n++)
		for (int h = 0; h < HEIGHT; h++)
			for (int w = 0; w < WIDTH; w++)
				if (original[h][w] == n) 
					eqImg[h][w] = round(Hist_CDF[n] * 255);
}

void AverageFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	int originalStart_h = (int)nFilterSize / 2;//패딩 영상에서 원본 영상의 픽셀 시작 높이 정보 
	int originalStart_w = (int)nFilterSize / 2;//패딩 영상에서 원본 영상의 픽셀 시작 너비 정보 
	int filterStart_h, filterStart_w;//각 픽셀 당 필터의 좌상단 기준점
	int sumOfPixelInFilter = 0;//filter내의 픽셀값 총합

	for (int h = originalStart_h; h < (nHeight + (int)nFilterSize / 2); h++) {
		for (int w = originalStart_w; w < (nWidth + (int)nFilterSize / 2); w++) {
			filterStart_h = h - (int)nFilterSize / 2;
			filterStart_w = w - (int)nFilterSize / 2;
			for (int i = 0; i < nFilterSize; i++)
				for (int j = 0; j < nFilterSize; j++)
					sumOfPixelInFilter += In_Pad[filterStart_h + i][filterStart_w + j];
			Out[h - (int)nFilterSize / 2][w - (int)nFilterSize / 2] = sumOfPixelInFilter / (nFilterSize*nFilterSize);
			sumOfPixelInFilter = 0;
		}
	}
}

void SmoothFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth) {
	int filterStart_h, filterStart_w;//각 픽셀 당 필터의 좌상단 기준점
	int sumOfPixelInFilter = 0;//filter내의 픽셀값 총합
	double smoothFilter[][3] = {
		{0.0625, 0.125, 0.0625},
		{0.125, 0.25, 0.125},
		{0.0625, 0.125, 0.0625}
	};

	for (int h = 1; h < nHeight + 1; h++) {
		for (int w = 1; w < nWidth + 1; w++) {
			filterStart_h = h - 1;
			filterStart_w = w - 1;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++) 
					sumOfPixelInFilter += (int)(In_Pad[filterStart_h + i][filterStart_w + j] * smoothFilter[i][j]);
				
			Out[h - 1][w - 1] = sumOfPixelInFilter;
			sumOfPixelInFilter = 0;
		}
	}
}

void SharpenFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth) {
	int filterStart_h, filterStart_w;//각 픽셀 당 필터의 좌상단 기준점
	int sumOfPixelInFilter = 0;//filter내의 픽셀값 총합
	int sharpenFilter[3][3] = {
		{0, -1, 0},
		{-1, 5, -1},
		{0, -1, 0}
	};

	for (int h = 1; h < nHeight + 1; h++) {
		for (int w = 1; w < nWidth + 1; w++) {
			filterStart_h = h - 1;
			filterStart_w = w - 1;
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					sumOfPixelInFilter += In_Pad[filterStart_h + i][filterStart_w + j] * sharpenFilter[i][j];
			if (sumOfPixelInFilter < 0)
				Out[h - 1][w - 1] = 0;
			else if (sumOfPixelInFilter > 255)
				Out[h - 1][w - 1] = 255;
			else {
				if (sumOfPixelInFilter < 0 || sumOfPixelInFilter > 255)
					cout << sumOfPixelInFilter << endl;
				Out[h - 1][w - 1] = sumOfPixelInFilter;
			}
			sumOfPixelInFilter = 0;
		}
	}
}

void MedianFilter(unsigned char**In_Pad, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	int originalStart_h = (int)nFilterSize / 2;
	int originalStart_w = (int)nFilterSize / 2;
	int filterStart_h, filterStart_w;
	int *pixelInMask = new int[nFilterSize*nFilterSize];//필터 크기만큼 배열 할당
	int k = 0;//Median filter mask 내부의 값을 순회하기 위한 변수

	for (int h = originalStart_h; h < (nHeight + (int)nFilterSize / 2); h++) {
		for (int w = originalStart_w; w < (nWidth + (int)nFilterSize / 2); w++) {
			filterStart_h = h - (int)nFilterSize / 2;
			filterStart_w = w - (int)nFilterSize / 2;
			for (int i = 0; i < nFilterSize; i++)
				for (int j = 0; j < nFilterSize; j++)
					pixelInMask[k++] = In_Pad[filterStart_h + i][filterStart_w + j];//필터 내부의 값을 배열에 저장
			sort(pixelInMask, pixelInMask + (k+1));

			Out[h - (int)nFilterSize / 2][w - (int)nFilterSize / 2] = pixelInMask[(int)k/2];
			k = 0;
		}
	}
	
}
