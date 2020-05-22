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
//2���� �迭�� �����Ҵ��ϴ� �Լ�
unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal) {
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++) {
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}
	return rtn;
}
//filter�� �����ϱ� ���� �����ڸ��� padding�ϴ� �Լ�
unsigned char** Padding(unsigned char **In, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);

	for (int h = 0; h < nHeight; h++) {//���� �����͸� Pad�� �߾ӿ� ����
		for (int w = 0; w < nWidth; w++) {
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
		}
	}

	for (int h = 0; h < nPadSize; h++) {//�� �Ʒ� �����ڸ� �� ����
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++) {//��, �� �����ڸ� �� ����
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}

	for (int h = 0; h < nPadSize; h++) {//4���� ������ �� ����
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
	int filterStart_h, filterStart_w;//�� �ȼ� �� ������ �»�� ������
	int sumOfPixelInFilter = 0;//filter���� �ȼ��� ����
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
	//r : ������ �Ǵ� r,g,b �ȼ��� (���� 3 �迭), origin : r, g, b �ȼ� �����ͷθ� �̷���� ���� ����
	int squareSum=0;//���� �ȼ��� ���� �ȼ��� ������������ �����ϴ� ����	

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			for (int i = 0; i < 3; i++) 
				squareSum += ((r[i] - origin[h][w][i]) * (r[i] - origin[h][w][i]));//���ذ� ������ r, g, b �ȼ� ���� ������ ��.													
			if (squareSum > (radius*radius)) {						//����� color�� �ƴ� �弿				
				origin[h][w][0] = origin[h][w][1] = origin[h][w][2] = 50;
				map[h][w] = 0;
			}
			else {
				map[h][w] = 1;//���� ����Ǵ� �κ�
			}
			squareSum = 0;
		}
	}
}

int main() {
	Mat jws(H, W, CV_8UC3);//���� Ȯ�ο� ����, ���� ���� 
	unsigned char jwsRaw[H][W][3];//���� ������ r g b������ �и��Ǿ� ��� �迭 
	unsigned char jwsRawCopy[H][W][3];
	//slicing���� R,G,B ������ �����ϱ� ���� �迭
	unsigned char **colorSlicedRed = MemAlloc2D(H, W, 0);
	unsigned char **colorSlicedGreen = MemAlloc2D(H, W, 0);
	unsigned char **colorSlicedBlue = MemAlloc2D(H, W, 0);
	//R,G,B������ smoothing ����� ������ ����
	unsigned char **smoothRed = MemAlloc2D(H, W, 0);
	unsigned char **smoothGreen = MemAlloc2D(H, W, 0);
	unsigned char **smoothBlue = MemAlloc2D(H, W, 0);
	unsigned char **pad;//smoothing filter ���� �� padding�ϱ� ���� ����
	unsigned char **map = MemAlloc2D(H, W, 0);//color slicing ���θ� ��Ÿ�� ���� ���̺�

	FILE *fp = fopen("woosung.bmp", "rb");//���� ���� ���� ������
	FILE *fpRaw = fopen("woosung.raw", "wb");//�������� ����� ������ raw ������ ������ ����������
	///bmp������ ����� �о���̱� ���� ����ü ���� ����
	BITMAPFILEHEADER *file_header = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
	BITMAPINFOHEADER *info_header = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
	unsigned char faceColorStd[3] = {190, 171, 142};//������ �Ǵ� �� ��(r g b �ȼ� ��)
	unsigned char radius = 125;//color slicing�ϱ� ���� rgb sphereƯ�� ����(sphere��� ���� �ȼ����� �����Ͽ� 

	fread(file_header, sizeof(BITMAPFILEHEADER), 1, fp);//��� ������ ���� �� ������ �ȼ� �����͸� �ٸ� ���ۿ� ����.
	fread(info_header, sizeof(BITMAPINFOHEADER), 1, fp);//��� ������ ���� �� ������ �ȼ� �����͸� �ٸ� ���ۿ� ����.

	if (file_header->bfType == 0x4D42) {//bmp������ �ԷµǸ� ���� ũ���, �ȼ� �������� ��ġ�� ���
		cout << "file size : " << file_header->bfSize << endl;
		cout << "�ȼ� �������� ��ġ : " << file_header->bfOffBits << endl;
	}
	else {//�Էµ� ������ bmp������ �ƴϸ� ���α׷� ����
		cout << "bmp ������ �ƴմϴ�." << endl;
		system("pause");
		return 0;
	}

	fseek(fp, file_header->bfOffBits, SEEK_SET);//���� �ȼ� ������ ��ġ�� ���� ������ �̵�
	for (int h = H - 1; h >= 0; h--) {//������ �ȼ����� �����Ͽ� ���� ����.
		for (int w = W - 1; w >= 0; w--) {
			fread(jwsRaw[h][w], sizeof(unsigned char), 3, fp); //3����Ʈ�� �о�鿩 RGB������ �о����
		}
		reverse(begin(jwsRaw[h]), end(jwsRaw[h]));//������ reverse�Ͽ� �¿� ����.
	}

	//imshow�� ���� Mat���Ŀ� ����
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			Vec3b rgb;
			for (int i = 0; i < 3; i++)
				rgb[i] = jwsRaw[h][w][i];//Mat �����ʹ� rgb ������ ����			
			jws.at<Vec3b>(h, w) = rgb;
		}
	}
	imshow("original", jws);
	//�������� ����� ������ raw������ ���� ������� ���� 
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {//bgr ������ ���Ͽ� ����
			fwrite(&jwsRaw[h][w][2], sizeof(unsigned char), 1, fpRaw);
			fwrite(&jwsRaw[h][w][1], sizeof(unsigned char), 1, fpRaw);
			fwrite(&jwsRaw[h][w][0], sizeof(unsigned char), 1, fpRaw);
		}
	}

	//Raw���� ���� ����
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			for (int i = 0; i < 3; i++) 
				jwsRawCopy[h][w][i] = jwsRaw[h][w][i];			
		}
	}
	//������ �Ǵ� �� �ȼ� ������ slicing.
	ColorSlicing(faceColorStd, radius, jwsRaw, map);
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			Vec3b rgb;
			for (int i = 0; i < 3; i++)
				rgb[i] = jwsRaw[h][w][i];//Mat �����ʹ� rgb ������ ����			
			jws.at<Vec3b>(h, w) = rgb;
		}
	}
	imshow("colorsliced", jws);
	//Slicing ����� R,G,B�� ���� �и�
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
			if (map[h][w] == 1) {//slicing �� �κ� (��)
				//������ smoothing�� ��ģ rgb�� �����Ѵ�. 
				jwsRawCopy[h][w][0] = smoothRed[h][w];
				jwsRawCopy[h][w][1] = smoothGreen[h][w];
				jwsRawCopy[h][w][2] = smoothBlue[h][w];
			}
			else {//slicing ���� �κ�
				;
			}			
		}
	}

	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			Vec3b rgb;
			for (int i = 0; i < 3; i++) 
				rgb[i] = jwsRawCopy[h][w][i];//Mat �����ʹ� rgb ������ ����			
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