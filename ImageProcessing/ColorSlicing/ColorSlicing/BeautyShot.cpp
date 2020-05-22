#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;
#define W 400
#define H 411

typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

#pragma pack(2)
typedef struct {
	WORD bfType;
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}BITMAPFILEHEADER;

typedef struct {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}BITMAPINFOHEADER;
#pragma pack()
//2차원 배열을 동적할당하는 함수
unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal) {
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++) {
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}
//filter를 적용하기 위해 가장자리를 padding하는 함수
unsigned char** Padding(unsigned char **In, int nHeight, int nWidth, int nFilterSize) {
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
//smooth filter
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

void ColorSlicing(unsigned char r[], unsigned char radius, unsigned char origin[H][W][3], unsigned char **map) {
	//r : 기준이 되는 r,g,b 픽셀값 (길이 3 배열), origin : r, g, b 픽셀 데이터로만 이루어진 원본 영상
	int squareSum=0;//기준 픽셀과 현재 픽셀의 오차제곱합을 저장하는 변수	

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			for (int i = 0; i < 3; i++) 
				squareSum += ((r[i] - origin[h][w][i]) * (r[i] - origin[h][w][i]));//기준과 현재의 r, g, b 픽셀 오차 제곱의 합.													
			if (squareSum > (radius*radius)) {						//비슷한 color가 아닌 펙셀				
				origin[h][w][0] = origin[h][w][1] = origin[h][w][2] = 50;
				map[h][w] = 0;
			}
			else {
				map[h][w] = 1;//얼굴이 추출되는 부분
			}
			squareSum = 0;
		}
	}
}

int main() {
	Mat jws(H, W, CV_8UC3);//영상 확인용 변수, 추후 제거 
	unsigned char jwsRaw[H][W][3];//원본 영상의 r g b성분이 분리되어 담긴 배열 
	unsigned char jwsRawCopy[H][W][3];
	//slicing후의 R,G,B 각각을 저장하기 위한 배열
	unsigned char **colorSlicedRed = MemAlloc2D(H, W, 0);
	unsigned char **colorSlicedGreen = MemAlloc2D(H, W, 0);
	unsigned char **colorSlicedBlue = MemAlloc2D(H, W, 0);
	//R,G,B각각의 smoothing 결과를 저장할 공간
	unsigned char **smoothRed = MemAlloc2D(H, W, 0);
	unsigned char **smoothGreen = MemAlloc2D(H, W, 0);
	unsigned char **smoothBlue = MemAlloc2D(H, W, 0);
	unsigned char **pad;//smoothing filter 적용 시 padding하기 위한 변수
	unsigned char **map = MemAlloc2D(H, W, 0);//color slicing 여부를 나타낼 매핑 테이블

	FILE *fp = fopen("woosung.bmp", "rb");//원본 영상 파일 포인터
	FILE *fpRaw = fopen("woosung.raw", "wb");//원본에서 헤더를 제거한 raw 영상을 저장할 파일포인터
	///bmp파일의 헤더를 읽어들이기 위한 구조체 변수 선언
	BITMAPFILEHEADER *file_header = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	BITMAPINFOHEADER *info_header = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
	unsigned char faceColorStd[3] = {190, 171, 142};//기준이 되는 얼굴 색(r g b 픽셀 값)
	unsigned char radius = 125;//color slicing하기 위한 rgb sphere특정 범위(sphere모양 내의 픽셀값을 추출하여 

	fread(file_header, sizeof(BITMAPFILEHEADER), 1, fp);//헤더 정보를 읽은 후 부터의 픽셀 데이터를 다른 버퍼에 저장.
	fread(info_header, sizeof(BITMAPINFOHEADER), 1, fp);//헤더 정보를 읽은 후 부터의 픽셀 데이터를 다른 버퍼에 저장.

	if (file_header->bfType == 0x4D42) {//bmp파일이 입력되면 파일 크기와, 픽셀 데이터의 위치를 출력
		cout << "file size : " << file_header->bfSize << endl;
		cout << "픽셀 데이터의 위치 : " << file_header->bfOffBits << endl;
	}
	else {//입력된 파일이 bmp형식이 아니면 프로그램 종료
		cout << "bmp 파일이 아닙니다." << endl;
		system("pause");
		return 0;
	}

	fseek(fp, file_header->bfOffBits, SEEK_SET);//실제 픽셀 데이터 위치로 파일 포인터 이동
	for (int h = H - 1; h >= 0; h--) {//마지막 픽셀부터 저장하여 상하 반전.
		for (int w = W - 1; w >= 0; w--) {
			fread(jwsRaw[h][w], sizeof(unsigned char), 3, fp); //3바이트씩 읽어들여 RGB순서로 읽어들임
		}
		reverse(begin(jwsRaw[h]), end(jwsRaw[h]));//열마다 reverse하여 좌우 반전.
	}

	//imshow를 위해 Mat형식에 저장
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			Vec3b rgb;
			for (int i = 0; i < 3; i++)
				rgb[i] = jwsRaw[h][w][i];//Mat 데이터는 rgb 순서로 저장			
			jws.at<Vec3b>(h, w) = rgb;
		}
	}
	imshow("original", jws);
	//원본에서 헤더를 제거한 raw파일을 파일 출력으로 저장 
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {//bgr 순서로 파일에 저장
			fwrite(&jwsRaw[h][w][2], sizeof(unsigned char), 1, fpRaw);
			fwrite(&jwsRaw[h][w][1], sizeof(unsigned char), 1, fpRaw);
			fwrite(&jwsRaw[h][w][0], sizeof(unsigned char), 1, fpRaw);
		}
	}

	//Raw파일 원본 복사
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			for (int i = 0; i < 3; i++) 
				jwsRawCopy[h][w][i] = jwsRaw[h][w][i];			
		}
	}
	//기준이 되는 얼굴 픽셀 값으로 slicing.
	ColorSlicing(faceColorStd, radius, jwsRaw, map);
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			Vec3b rgb;
			for (int i = 0; i < 3; i++)
				rgb[i] = jwsRaw[h][w][i];//Mat 데이터는 rgb 순서로 저장			
			jws.at<Vec3b>(h, w) = rgb;
		}
	}
	imshow("colorsliced", jws);
	//Slicing 결과를 R,G,B로 각각 분리
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			colorSlicedRed[h][w] = jwsRaw[h][w][0];
			colorSlicedGreen[h][w] = jwsRaw[h][w][1];
			colorSlicedBlue[h][w] = jwsRaw[h][w][2];
		}
	}

	pad = Padding(colorSlicedRed, H, W, 3);
	SmoothFilter(pad, smoothRed, H, W);
	pad = Padding(colorSlicedGreen, H, W, 3);
	SmoothFilter(pad, smoothGreen, H, W);
	pad = Padding(colorSlicedBlue, H, W, 3);
	SmoothFilter(pad, smoothBlue, H, W);

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			if (map[h][w] == 1) {//slicing 된 부분 (얼굴)
				//원본에 smoothing을 거친 rgb를 적용한다. 
				jwsRawCopy[h][w][0] = smoothRed[h][w];
				jwsRawCopy[h][w][1] = smoothGreen[h][w];
				jwsRawCopy[h][w][2] = smoothBlue[h][w];
			}
			else {//slicing 안한 부분
				;
			}			
		}
	}

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			Vec3b rgb;
			for (int i = 0; i < 3; i++) 
				rgb[i] = jwsRawCopy[h][w][i];//Mat 데이터는 rgb 순서로 저장			
			jws.at<Vec3b>(h, w) = rgb;
		}
	}	
	imshow("smoothed", jws);
	
	waitKey(0);
	fclose(fp);
	fclose(fpRaw);
	free(file_header);
	free(info_header);
	return 0;
}