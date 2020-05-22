#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//.rgb 파일을 읽어들여 grayscale 영상으로 변환한 후
//piecewise 적용하기. 
//1.   .rgb파일 읽기
//2.  grayscale 영상으로 변환하기
//3.  piecewise 함수 작성하기
#define W 512
#define H 512

void PieceWise(Mat lena) {
	//(r1, s1) = (65, 15)  (r2, s2) = (130, 180)

	Mat rgb[3];
	split(lena, rgb);//lena영상을 r,g,b 성분으로 각각 분리
	//각 성분에 piece wise linear constant stretching 적용
	for (int h = 0; h < H; h++) {
		for (int w = 0; w < W; w++) {
			//r성분 piecewise
			if (rgb[0].at<unsigned char>(h, w) < 65)
				rgb[0].at<unsigned char>(h, w) = (unsigned char)(0.23 * rgb[0].at<unsigned char>(h, w));
			else if (65 <= rgb[0].at<unsigned char>(h, w) && rgb[0].at<unsigned char>(h, w) <= 130)
				rgb[0].at<unsigned char>(h, w) = (unsigned char)(0.833 * rgb[0].at<unsigned char>(h, w)) + 80;
			else
				rgb[0].at<unsigned char>(h, w) = (unsigned char)(0.6 * rgb[0].at<unsigned char>(h, w));
			//g성분 piecewise
			if (rgb[1].at<unsigned char>(h, w) < 65)
				rgb[1].at<unsigned char>(h, w) = (unsigned char)(0.23 * rgb[1].at<unsigned char>(h, w));
			else if (65 <= rgb[0].at<unsigned char>(h, w) && rgb[0].at<unsigned char>(h, w) <= 130)
				rgb[1].at<unsigned char>(h, w) = (unsigned char)(0.833 * rgb[1].at<unsigned char>(h, w)) + 80;
			else
				rgb[0].at<unsigned char>(h, w) = (unsigned char)(0.6 * rgb[1].at<unsigned char>(h, w)) + 102;
			//b성분 piecewise
			if (rgb[2].at<unsigned char>(h, w) < 65)
				rgb[2].at<unsigned char>(h, w) = (unsigned char)(0.23 * rgb[2].at<unsigned char>(h, w));
			else if (65 <= rgb[0].at<unsigned char>(h, w) && rgb[2].at<unsigned char>(h, w) <= 130)
				rgb[2].at<unsigned char>(h, w) = (unsigned char)(0.833 * rgb[2].at<unsigned char>(h, w)) + 80;
			else
				rgb[2].at<unsigned char>(h, w) = (unsigned char)(0.6 * rgb[2].at<unsigned char>(h, w)) + 102;
		}
	}
	merge(rgb, 3, lena);
}

int main() {
	Mat lena(H, W, CV_8UC3);//3채널 Mat 데이터 생성 	
	unsigned char lenaBGR[512][512][3];
	FILE *fp = fopen("lena512.rgb", "rb");

	if (fp == NULL) {
		cout << "파일 열기 실패" << endl;
		system("pause");
		return 0;		
	}

	for (int i = 0; i < H; i++) 
		for (int j = 0; j < W; j++) 
			fread(lenaBGR[i][j], sizeof(unsigned char), 3, fp);
	//lenaBGR에는 BGR 순서의 3channel pixel data가 저장
	for (int h = 0; h < H; h++) {		
		for (int w = 0; w < W; w++) {			
			Vec3b rgb;
			for (int i = 0; i < 3; i++)
				rgb[i] = lenaBGR[h][w][2-i];//RGB순서로 rgb변수에 저장.
			lena.at<Vec3b>(h, w) = rgb;
		}
	}

	imshow("lena", lena);
	
	PieceWise(lena);

	imshow("PieceWise-lena", lena);

	waitKey(0);

	fclose(fp);
	for (int i = 0; i < W*H; i++)
		delete lenaBGR[i];
	delete lenaBGR;

	return 0;
}