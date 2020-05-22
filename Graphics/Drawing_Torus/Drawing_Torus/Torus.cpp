#include<Windows.h>
#include<iostream>
#include<gl\GL.h>//렌더링 기능 제공
#include<gl\GLU.h>//gl라이브러리 도우미
#include<gl\glut.h>//사용자의 입력을 받고 화면 윈도우를 제어하기 위한 lib 
#include<math.h>
#define PI 3.141592
#define HEIGHT 512
#define WIDTH 512
#define CHANNEL 3

using namespace std;

typedef struct {
	float x;
	float y;
	float z;
}Position;

static Position p[36][18];//18개의 점과 36번 회전된 점이 찍힐 위치
static double rad = 0.3;//원의 반지름 설정 
static int c = 0, k = 0, w = 0, q = 0, qw = 0, sh = 0, polyNormVec = 0, pointNormVec = 0;//각 번호 키 설정 

static Position center[36][18];
static GLbyte check[HEIGHT][WIDTH][CHANNEL];
static GLbyte marble[HEIGHT][WIDTH][CHANNEL];
static GLbyte wood[HEIGHT][WIDTH][CHANNEL];
static GLuint texID[3];

void ReadMaterial() {	
	FILE *fp;
	//check 텍스쳐 읽어오기
	fopen_s(&fp, "check.raw", "rb");
	fread(check, HEIGHT*WIDTH*CHANNEL, 1, fp);
	fclose(fp);

	//marble 텍스쳐 읽어오기	
	fopen_s(&fp, "marble.raw", "rb");
	fread(marble, HEIGHT*WIDTH*CHANNEL, 1, fp);
	fclose(fp);


	//wood 텍스쳐 읽어오기
	fopen_s(&fp, "wood.raw", "rb");
	fread(wood, HEIGHT*WIDTH*CHANNEL, 1, fp);
	fclose(fp);

	glGenTextures(3, &texID[0]);//텍스쳐 id 등록
	
	glBindTexture(GL_TEXTURE_2D, texID[0]);	
	glTexImage2D(GL_TEXTURE_2D, 0, 3	, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, check);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texID[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, marble);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, texID[2]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, wood);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void InitTorus() {
	float x, z;
	for (int j = 0; j < 36; j++) {
		for (int i = 0; i < 18; i++) {
			p[j][i].x = rad * cos(i * PI / 9);
			p[j][i].y = rad * sin(i * PI / 9);
			p[j][i].z = 0;
			//Translation
			p[j][i].x = p[j][i].x + 0.6;
			p[j][i].y = p[j][i].y + 0.6;
			//Rotation	
			x = p[j][i].x * cos((10.0*j) * (PI / 180)) +
				p[j][i].z*sin((10.0*j) * (PI / 180));
			z = p[j][i].z * cos((10.0*j) *(PI / 180)) -
				p[j][i].x * sin((10.0*j) * (PI / 180));
			p[j][i].x = x;
			p[j][i].z = z;
		}
	}
}
void ChangeSize(int w, int h) {
	if (h == 0)//devided by zero 방지
		h = 1;

	glViewport(0, 0, w, h);//Set Viewport to window dimension

	//Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0*(float)h / (float)w, 2.0*(float)h / (float)w, -10.0, 10.0);
	else
		glOrtho(-2.0*(float)w / (float)h, 2.0*(float)w / (float)h, -2.0, 2.0, -10.0, 10.0);
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		k = (k == 0) ? 1 : 0; c = 1; break;
	case '2':
		if (!c)break;
		w = (w == 0) ? 1 : 0; break;
	case '3':
		if (!c)break;
		q = (q == 0) ? 1 : 0; break;
		/*
	case '4':
		if (!c)break;
		qw = (qw == 0) ? 1 : 0; break;
	case '5':
		if (!c)break;
		sh = (sh == 0) ? 1 : 0; break;
	case '6':
		if (!c)break;
		polyNormVec = (polyNormVec == 0) ? 1 : 0; break;
		break;
	case '7':
		if (!c)break;
		pointNormVec = (pointNormVec == 0) ? 1 : 0; break;
	case 'a':
		if (yAxisSweepAngle < 36)
			yAxisSweepAngle++;
		break;
	case 's':
		if (yAxisSweepAngle > 0)
			yAxisSweepAngle--;
		break;
	case 'j':
		if (zAxisSweepAngle < 18)
			zAxisSweepAngle++;
		break;
	case 'k':
		if (zAxisSweepAngle > 0)
			zAxisSweepAngle--;
		break;
		*/
	}

	glutPostRedisplay();
}
void SetupRC() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Rendering Buffer를 지울 색 설정 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	ReadMaterial();//텍스쳐 매핑을 위한 material을 읽어들임.
}
void DrawingAxis() {
	//x축	
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(2.5, 0, 0);
	glVertex3f(0, 0, 0);
	glEnd();
	//y축

	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 2.5, 0);
	glEnd();
	//z축

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 2.5);
	glEnd();
}
float Normalize(Position &n) {
	float f = n.x*n.x + n.y*n.y + n.z*n.z;
	float vectorSize;
	if (f > 0) {
		vectorSize = (float)(1.0 / sqrt(f));
		n.x *= vectorSize;
		n.y *= vectorSize;
		n.z *= vectorSize;
	}
	return vectorSize;
}
void CrossProduct(Position p1, Position p2, Position p3, Position &n) {
	//p3-p2 x p1-p2 바깥 방향 normal vector 
	//p3.x-p2.x   p1.x-p2.x
	//p3.y-p2.y   p1.y-p2.y
	//p3.z-p2.z   p1.z-p2.z  
	n.x = (p3.y - p2.y)*(p1.z - p2.z) - (p3.z - p2.z)*(p1.y - p2.y);
	n.y = (p3.z - p2.z)*(p1.x - p2.x) - (p3.x - p2.x)*(p1.z - p2.z);
	n.z = (p3.x - p2.x)*(p1.y - p2.y) - (p3.y - p2.y)*(p1.x - p2.x);

	Normalize(n);
}
void PolygonColor(int i, int j, Position &normalVec) {
	Position COP;
	float dotPro;

	if (j == 35) {
		if (i == 17) {
			CrossProduct(p[j][0], p[j][i], p[0][i], normalVec);
			center[j][i].x = (p[j][i].x + p[0][0].x) / 2;
			center[j][i].y = (p[j][i].y + p[0][0].y) / 2;
			center[j][i].z = (p[j][i].z + p[0][0].z) / 2;
			COP.x = 10 - ((p[j][i].x + p[0][0].x) / 2);
			COP.y = 10 - ((p[j][i].y + p[0][0].y) / 2);
			COP.z = 10 - ((p[j][i].z + p[0][0].z) / 2);
		}
		else {

			CrossProduct(p[j][i + 1], p[j][i], p[0][i], normalVec);
			center[j][i].x = (p[j][i].x + p[0][i + 1].x) / 2;
			center[j][i].y = (p[j][i].y + p[0][i + 1].y) / 2;
			center[j][i].z = (p[j][i].z + p[0][i + 1].z) / 2;
			COP.x = 10 - ((p[j][i].x + p[0][i + 1].x) / 2);
			COP.y = 10 - ((p[j][i].y + p[0][i + 1].y) / 2);
			COP.z = 10 - ((p[j][i].z + p[0][i + 1].z) / 2);
		}
	}
	else {
		if (i == 17) {
			CrossProduct(p[j][0], p[j][i], p[j + 1][i], normalVec);
			center[j][i].x = (p[j][i].x + p[j + 1][0].x) / 2;
			center[j][i].y = (p[j][i].y + p[j + 1][0].y) / 2;
			center[j][i].z = (p[j][i].z + p[j + 1][0].z) / 2;
			COP.x = 10 - ((p[j][i].x + p[j + 1][0].x) / 2);
			COP.y = 10 - ((p[j][i].y + p[j + 1][0].y) / 2);
			COP.z = 10 - ((p[j][i].z + p[j + 1][0].z) / 2);
		}
		else {
			CrossProduct(p[j][i + 1], p[j][i], p[j + 1][i], normalVec);
			center[j][i].x = (p[j][i].x + p[j + 1][i + 1].x) / 2;
			center[j][i].y = (p[j][i].y + p[j + 1][i + 1].y) / 2;
			center[j][i].z = (p[j][i].z + p[j + 1][i + 1].z) / 2;
			COP.x = 10 - ((p[j][i].x + p[j + 1][i + 1].x) / 2);
			COP.y = 10 - ((p[j][i].y + p[j + 1][i + 1].y) / 2);
			COP.z = 10 - ((p[j][i].z + p[j + 1][i + 1].z) / 2);
		}
	}
	dotPro = normalVec.x * COP.x + normalVec.y * COP.y + normalVec.z * COP.z;
	if (dotPro > 0) {
		glColor3f(0.0f, 0.0f, 1.0f);
	}
	else {
		glColor3f(1.0f, 0.0f, 0.0f);
	}
}
void OuterOffSetting(int j, int i, Position &aveNormalVec) {
	//현재 j,i 위치를 중심으로 하는 1,2,3,4분면	
	Position plane1[3];
	Position plane2[3];
	Position plane3[3];
	Position plane4[3];
	Position normalVec[4];//각 사분면의 법선벡터 
	//4면으로 둘러쌓인 꼭짓점의 법선벡터
	float vectorSize;
	if (j == 0) {
		if (i == 0) {
			plane1[0] = p[j][i + 1];
			plane1[1] = p[j][i];
			plane1[2] = p[j + 1][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j + 1][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][17];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][17];
			plane3[1] = p[j][i];
			plane3[2] = p[35][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[35][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][i + 1];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else if (i == 17) {
			plane1[0] = p[j][0];
			plane1[1] = p[j][i];
			plane1[2] = p[j + 1][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j + 1][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][i - 1];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][i - 1];
			plane3[1] = p[j][i];
			plane3[2] = p[35][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[35][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][0];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else {
			plane1[0] = p[j][i + 1];
			plane1[1] = p[j][i];
			plane1[2] = p[j + 1][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j + 1][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][i - 1];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][i - 1];
			plane3[1] = p[j][i];
			plane3[2] = p[35][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[35][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][i + 1];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
	}
	else if (j == 35) {
		if (i == 0) {
			plane1[0] = p[j][i + 1];
			plane1[1] = p[j][i];
			plane1[2] = p[0][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[0][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][17];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][17];
			plane3[1] = p[j][i];
			plane3[2] = p[j - 1][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j - 1][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][i + 1];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else if (i == 17) {
			plane1[0] = p[j][0];
			plane1[1] = p[j][i];
			plane1[2] = p[0][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[0][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][i - 1];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][i - 1];
			plane3[1] = p[j][i];
			plane3[2] = p[j - 1][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j - 1][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][0];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else {
			plane1[0] = p[j][i + 1];
			plane1[1] = p[j][i];
			plane1[2] = p[0][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[0][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][i - 1];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][i - 1];
			plane3[1] = p[j][i];
			plane3[2] = p[j - 1][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j - 1][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][i + 1];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
	}
	else {
		if (i == 0) {
			plane1[0] = p[j][i + 1];
			plane1[1] = p[j][i];
			plane1[2] = p[j + 1][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j + 1][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][17];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][17];
			plane3[1] = p[j][i];
			plane3[2] = p[j - 1][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j - 1][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][i + 1];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else if (i == 17) {//j 상관없음
			plane1[0] = p[j][0];
			plane1[1] = p[j][i];
			plane1[2] = p[j + 1][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j + 1][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][i - 1];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][i - 1];
			plane3[1] = p[j][i];
			plane3[2] = p[j - 1][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j - 1][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][0];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else {
			plane1[0] = p[j][i + 1];
			plane1[1] = p[j][i];
			plane1[2] = p[j + 1][i];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j + 1][i];
			plane2[1] = p[j][i];
			plane2[2] = p[j][i - 1];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j][i - 1];
			plane3[1] = p[j][i];
			plane3[2] = p[j - 1][i];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j - 1][i];
			plane4[1] = p[j][i];
			plane4[2] = p[j][i + 1];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
	}

	vectorSize = Normalize(normalVec[0]) + Normalize(normalVec[1]) + Normalize(normalVec[2]) + Normalize(normalVec[3]);
	aveNormalVec.x = (normalVec[0].x + normalVec[1].x + normalVec[2].x + normalVec[3].x) / vectorSize;
	aveNormalVec.y = (normalVec[0].y + normalVec[1].y + normalVec[2].y + normalVec[3].y) / vectorSize;
	aveNormalVec.z = (normalVec[0].z + normalVec[1].z + normalVec[2].z + normalVec[3].z) / vectorSize;

}
void InnerOffSetting(int j, int i, Position &aveNormalVec) {
	//현재 j,i 위치를 중심으로 하는 1,2,3,4분면	
	Position plane1[3];
	Position plane2[3];
	Position plane3[3];
	Position plane4[3];
	Position normalVec[4];//각 사분면의 법선벡터 
	//4면으로 둘러쌓인 꼭짓점의 법선벡터
	float vectorSize;
	if (j == 0) {
		if (i == 0) {
			plane1[0] = p[j + 1][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][i + 1];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][17];
			plane2[1] = p[j][i];
			plane2[2] = p[j + 1][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[35][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][17];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][i + 1];
			plane4[1] = p[j][i];
			plane4[2] = p[35][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else if (i == 17) {
			plane1[0] = p[j + 1][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][0];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][i - 1];
			plane2[1] = p[j][i];
			plane2[2] = p[j + 1][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[35][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][i - 1];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][0];
			plane4[1] = p[j][i];
			plane4[2] = p[35][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else {
			plane1[0] = p[j + 1][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][i + 1];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][i - 1];
			plane2[1] = p[j][i];
			plane2[2] = p[j + 1][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[35][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][i - 1];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][i + 1];
			plane4[1] = p[j][i];
			plane4[2] = p[35][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
	}
	else if (j == 35) {
		if (i == 0) {
			plane1[0] = p[0][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][i + 1];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][17];
			plane2[1] = p[j][i];
			plane2[2] = p[0][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j - 1][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][17];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][i + 1];
			plane4[1] = p[j][i];
			plane4[2] = p[j - 1][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else if (i == 17) {
			plane1[0] = p[0][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][0];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][i - 1];
			plane2[1] = p[j][i];
			plane2[2] = p[0][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j - 1][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][i - 1];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][0];
			plane4[1] = p[j][i];
			plane4[2] = p[j - 1][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else {
			plane1[0] = p[0][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][i + 1];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][i - 1];
			plane2[1] = p[j][i];
			plane2[2] = p[0][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j - 1][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][i - 1];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][i + 1];
			plane4[1] = p[j][i];
			plane4[2] = p[j - 1][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
	}
	else {
		if (i == 0) {
			plane1[0] = p[j + 1][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][i + 1];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][17];
			plane2[1] = p[j][i];
			plane2[2] = p[j + 1][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j - 1][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][17];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][i + 1];
			plane4[1] = p[j][i];
			plane4[2] = p[j - 1][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else if (i == 17) {//j 상관없음
			plane1[0] = p[j + 1][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][0];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][i - 1];
			plane2[1] = p[j][i];
			plane2[2] = p[j + 1][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j - 1][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][i - 1];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][0];
			plane4[1] = p[j][i];
			plane4[2] = p[j - 1][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
		else {
			plane1[0] = p[j + 1][i];
			plane1[1] = p[j][i];
			plane1[2] = p[j][i + 1];
			CrossProduct(plane1[0], plane1[1], plane1[2], normalVec[0]);
			plane2[0] = p[j][i - 1];
			plane2[1] = p[j][i];
			plane2[2] = p[j + 1][i];
			CrossProduct(plane2[0], plane2[1], plane2[2], normalVec[1]);
			plane3[0] = p[j - 1][i];
			plane3[1] = p[j][i];
			plane3[2] = p[j][i - 1];
			CrossProduct(plane3[0], plane3[1], plane3[2], normalVec[2]);
			plane4[0] = p[j][i + 1];
			plane4[1] = p[j][i];
			plane4[2] = p[j - 1][i];
			CrossProduct(plane4[0], plane4[1], plane4[2], normalVec[3]);
		}
	}

	vectorSize = Normalize(normalVec[0]) + Normalize(normalVec[1]) + Normalize(normalVec[2]) + Normalize(normalVec[3]);
	aveNormalVec.x = (normalVec[0].x + normalVec[1].x + normalVec[2].x + normalVec[3].x) / vectorSize;
	aveNormalVec.y = (normalVec[0].y + normalVec[1].y + normalVec[2].y + normalVec[3].y) / vectorSize;
	aveNormalVec.z = (normalVec[0].z + normalVec[1].z + normalVec[2].z + normalVec[3].z) / vectorSize;

}

void DrawingTorus() {
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	gluLookAt(1, 1, 1, 0, 0, 0, 0, 1, 0);

	Position aveNormalVec[36][18];//꼭지점의 법선 벡터
	Position polyNormalVec[36][18];//폴리곤의 법선 벡터
	Position anv;
	DrawingAxis();//drawing x,y,z axises
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat pointAmbientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat pointDiffuseLight[] = { 0.5f, 0.5f,0.5f, 1.0f };
	GLfloat pointSpecularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat pointLightPos[] = { 0, 1, 0, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, pointAmbientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, pointLightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, pointDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, pointSpecularLight);

	//토러스 생성
	/*
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//quads를 생성
	for (int j = 0; j < 36; j++) {
		for (int i = 0; i < 18; i++) {

			glBegin(GL_QUADS);
			glNormal3f(polyNormalVec[j][i].x, polyNormalVec[j][i].y, polyNormalVec[j][i].z);

			if (j == 35) {
				if (i == 17) {
					PolygonColor(i, j, polyNormalVec[j][i]);
					glVertex3f(p[j][i].x, p[j][i].y, p[j][i].z);
					glVertex3f(p[0][i].x, p[0][i].y, p[0][i].z);
					glVertex3f(p[0][0].x, p[0][0].y, p[0][0].z);
					glVertex3f(p[j][0].x, p[j][0].y, p[j][0].z);
				}
				else {
					PolygonColor(i, j, polyNormalVec[j][i]);
					glVertex3f(p[j][i].x, p[j][i].y, p[j][i].z);
					glVertex3f(p[0][i].x, p[0][i].y, p[0][i].z);
					glVertex3f(p[0][i + 1].x, p[0][i + 1].y, p[0][i + 1].z);
					glVertex3f(p[j][i + 1].x, p[j][i + 1].y, p[j][i + 1].z);
				}
			}
			else {
				if (i == 17) {
					PolygonColor(i, j, polyNormalVec[j][i]);
					glVertex3f(p[j][i].x, p[j][i].y, p[j][i].z);
					glVertex3f(p[j + 1][i].x, p[j + 1][i].y, p[j + 1][i].z);
					glVertex3f(p[j + 1][0].x, p[j + 1][0].y, p[j + 1][0].z);
					glVertex3f(p[j][0].x, p[j][0].y, p[j][0].z);
				}
				else {
					PolygonColor(i, j, polyNormalVec[j][i]);
					glVertex3f(p[j][i].x, p[j][i].y, p[j][i].z);
					glVertex3f(p[j + 1][i].x, p[j + 1][i].y, p[j + 1][i].z);
					glVertex3f(p[j + 1][i + 1].x, p[j + 1][i + 1].y, p[j + 1][i + 1].z);
					glVertex3f(p[j][i + 1].x, p[j][i + 1].y, p[j][i + 1].z);
				}
			}
			glEnd();
		}
	}
	*/

	
	if (k == 1) {//ckeck 패턴 텍스쳐 매핑	
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		for (int j = 0; j < 36; j++) {
			for (int i = 0; i < 18; i++) {
				glBindTexture(GL_TEXTURE_2D, texID[0]);//첫번째 텍스쳐(check)를 bind
				glBegin(GL_QUADS);				

				//와이어 프레임을 그리지 않으므로 오프세팅 0
				OuterOffSetting(j, i, aveNormalVec[j][i]);
				anv.x = aveNormalVec[j][i].x * 0;
				anv.y = aveNormalVec[j][i].y * 0;
				anv.z = aveNormalVec[j][i].z * 0;

				//꼭지점의 법선벡터를 부여.
				glNormal3f(aveNormalVec[j][i].x, aveNormalVec[j][i].y, aveNormalVec[j][i].z);
				if (j == 35) {
					if (i == 17) {
						glTexCoord2f(1, 0);//p[35][17]
						glVertex3f(p[j][i].x + anv.x, p[j][i].y + anv.y, p[j][i].z + anv.z);
						glVertex3f(p[0][i].x + anv.x, p[0][i].y + anv.y, p[0][i].z + anv.z);
						glVertex3f(p[0][0].x + anv.x, p[0][0].y + anv.y, p[0][0].z + anv.z);
						glVertex3f(p[j][0].x + anv.x, p[j][0].y + anv.y, p[j][0].z + anv.z);
					}
					else {
						if(i==0)
							glTexCoord2f(1, 1);//p[35][0]
						glVertex3f(p[j][i].x + anv.x, p[j][i].y + anv.y, p[j][i].z + anv.z);
						glVertex3f(p[0][i].x + anv.x, p[0][i].y + anv.y, p[0][i].z + anv.z);
						glVertex3f(p[0][i + 1].x + anv.x, p[0][i + 1].y + anv.y, p[0][i + 1].z + anv.z);
						glVertex3f(p[j][i + 1].x + anv.x, p[j][i + 1].y + anv.y, p[j][i + 1].z + anv.z);
					}
				}
				else {
					if (i == 17) {
						if(j==0)
							glTexCoord2f(0, 0);
						glVertex3f(p[j][i].x + anv.x, p[j][i].y + anv.y, p[j][i].z + anv.z);
						glVertex3f(p[j + 1][i].x + anv.x, p[j + 1][i].y + anv.y, p[j + 1][i].z + anv.z);
						glVertex3f(p[j + 1][0].x + anv.x, p[j + 1][0].y + anv.y, p[j + 1][0].z + anv.z);
						glVertex3f(p[j][0].x + anv.x, p[j][0].y + anv.y, p[j][0].z + anv.z);
					}
					else {//가장 처음 그려지는 좌표
						if (j == 0 && i == 0) 
							glTexCoord2f(0, 1);						
						glVertex3f(p[j][i].x + anv.x, p[j][i].y + anv.y, p[j][i].z + anv.z);
						glVertex3f(p[j + 1][i].x + anv.x, p[j + 1][i].y + anv.y, p[j + 1][i].z + anv.z);
						glVertex3f(p[j + 1][i + 1].x + anv.x, p[j + 1][i + 1].y + anv.y, p[j + 1][i + 1].z + anv.z);
						glVertex3f(p[j][i + 1].x + anv.x, p[j][i + 1].y + anv.y, p[j][i + 1].z + anv.z);
					}
				}
				glEnd();
			}
		}
		glDisable(GL_LIGHTING);
	}
	glutSwapBuffers();
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);//glut라이브러리 초기화, 윈도우 시스템과 세션을 연다 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//rgb, 더블 버퍼링 
	glutInitWindowSize(600, 600);
	glutCreateWindow("Drawing Torus");
	InitTorus();
	SetupRC();
	glutDisplayFunc(DrawingTorus);
	glutReshapeFunc(ChangeSize);//크기가 변경되면 콜백 함수 호출
	glutKeyboardFunc(keyboard);
	
	
	glutMainLoop();
}