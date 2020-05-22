#include<Windows.h>
#include<iostream>
#include<fstream>
#include<gl\GL.h>//렌더링 기능 제공
#include<gl\GLU.h>//gl라이브러리 도우미
#include<gl\glut.h>//사용자의 입력을 받고 화면 윈도우를 제어하기 위한 lib 
#include<math.h>
#define PI 3.141592
using namespace std;

int pointNum = 0,triNum=0;
float *bunnyPoint;
int *triIndex;
float triNormalVector[948][3];//948개의 삼각형이 각각 x,y,z 법선벡터 좌표를 가짐
float pointLightTheta = 0.0;//점 광원을 회전시키는 변수
float pointLightSourceTheta = 0.0;
float directionalLightTheta = 0.0;//directional light source을 회전시키는 변수
float directionalLightSourceTheta = 0.0;
float spotLightTheta = 0.0;//spot light source를 회전시키는 변수
float spotLightSourceTheta = 0.0;
float spotCutOffTheta = 5.0;//spot light의 cut off를 조절하기 위한 변수
int cufOffFlag = 0;
int shineFlag = 0;
float spotLightSourceCutOffAngle = 0.01;
GLfloat shine = 10.0;
//key 적용을 위한 변수
int nVecOn = 0;//v key for normal vector toggle
int cutOffToggle = 0;//spot light cut off toggle
int pRot = 0;//ponit light 회전 toggle
int dRot = 0;//directional light 회전 toggle
int sRot = 0;//spot light 회전 toggle
int nShine = 0;
void timer(int value) {	
	if(pRot){
		pointLightTheta += 2.0;
		pointLightSourceTheta += 2.0;
	}
	else;
	if (dRot) {
		directionalLightTheta += 2.0;
		directionalLightSourceTheta += 2.0;
	}
	else;
	if (sRot) {
		spotLightTheta += 2.0;
		spotLightSourceTheta += 2.0;
	}
	else;
	if (!cutOffToggle) {
		if (cutOffToggle == 0 && cufOffFlag == 0) {//cuf off 각도에 따라 모형 변화 
			spotLightSourceCutOffAngle += 0.0005;
		}
		if (cutOffToggle == 0 && cufOffFlag == 1) {//cuf off 각도에 따라 모형 변화 
			spotLightSourceCutOffAngle -= 0.0005;
		}
		if (cufOffFlag == 0)//cut off 각도 증가 
			spotCutOffTheta += 0.1;
		else//각도 감소
			spotCutOffTheta -= 0.1;
		if (spotCutOffTheta > 25.0)
			cufOffFlag = 1;
		if (spotCutOffTheta < 5.0)
			cufOffFlag = 0;
	}
	else {//원뿔 모형 변화 없음, cut off 변화 없음 
		;
	}
	if (pointLightTheta >= 360.0)
		pointLightTheta -= 360.0;
	if (pointLightSourceTheta >= 360.0)
		pointLightSourceTheta -= 360.0;
	if (directionalLightTheta >= 360.0)
		directionalLightTheta -= 360.0;
	if (directionalLightSourceTheta >= 360.0)
		directionalLightSourceTheta -= 360.0;
	if (spotLightTheta >= 360.0)
		spotLightTheta -= 360.0;
	if (spotLightSourceTheta >= 360.0)
		spotLightSourceTheta -= 360.0;
	glutTimerFunc(20, timer, 1);
	glutPostRedisplay();
}
void ChangeSize(int w, int h) {
	if (h == 0)//devided by zero 방지
		h = 1;
	glViewport(-150, -150, w+300, h+300);//Set Viewport to window dimension
	//glViewport(0, 0, w, h);//Set Viewport to window dimension

	glMatrixMode(GL_PROJECTION);
	glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	/*
	if (w <= h) {
		//glOrtho(-5.0, 5.0, -5.0*(float)h / (float)w, 5.0*(float)h / (float)w, -5.0, 5.0);
		glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	}
	else;
		//glOrtho(-5.0*(float)w / (float)h,5.0*(float)w / (float)h, -5.0, 5.0, -5.0, 5.0);
		*/
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'v':
		nVecOn = (nVecOn == 0) ? 1 : 0;
		break;
	case 'c':
		cutOffToggle = (cutOffToggle == 0) ? 1 : 0;
		break;
	case 'p':
		pRot = (pRot == 0) ? 1 : 0;
		break;
	case 'd':
		dRot = (dRot == 0) ? 1 : 0;
		break;
	case 's':
		sRot = (sRot == 0) ? 1 : 0;
		break;
	case 'n':
		nShine = (nShine == 0) ? 1 : 0;
		break;
	}
	glutPostRedisplay();
}
void SetupRC() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);//Rendering Buffer를 지울 색 설정 
	glEnable(GL_DEPTH_TEST);
}
void DrawingAxis() {
	//x축	
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);	
	glVertex3f(0, 0, 0);
	glVertex3f(2.5, 0, 0);
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
float Normalize(float nVec[]) {
	float f = nVec[0]*nVec[0] + nVec[1]*nVec[1] + nVec[2]*nVec[2];
	float vectorSize;
	if (f > 0) {
		vectorSize = (float)(1.0 / sqrt(f));
		nVec[0] *= vectorSize;
		nVec[1] *= vectorSize;
		nVec[2] *= vectorSize;
	}
	return vectorSize;
}
void CrossProduct(float p1[], float p2[], float p3[], float normVec[]) {
	//p3-p2 x p1-p2 바깥 방향 normal vector 
	//p3.x-p2.x   p1.x-p2.x
	//p3.y-p2.y   p1.y-p2.y
	//p3.z-p2.z   p1.z-p2.z  
	normVec[0] = (p3[1] - p2[1])*(p1[2] - p2[2]) - (p3[2] - p2[2])*(p1[1] - p2[1]);
	normVec[1] = (p3[2] - p2[2])*(p1[0] - p2[0]) - (p3[0] - p2[0])*(p1[2] - p2[2]);
	normVec[2] = (p3[0] - p2[0])*(p1[1] - p2[1]) - (p3[1] - p2[1])*(p1[0] - p2[0]);

	Normalize(normVec);
}
void ReadBunny() {
	ifstream in("bunny_origin.txt");

	if (in.is_open()) {
		in >> pointNum >> triNum; //vertex개수와 삼각형 개수 입력
		bunnyPoint = (float*)malloc(sizeof(float)*pointNum*3);
		triIndex = (int*)malloc(sizeof(int)*triNum*3);

		for (int i = 0; i < pointNum*3; i++)//453x3개의 3D position 저장
			in >> bunnyPoint[i];
		for (int i = 0; i < triNum*3; i++)//948x3개의 point index
			in >> triIndex[i];
	}

	in.close();
}
void LightingBunny() {
	//Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(1, 1, 1, 0, 0, 0, 0, 1, 0);
	//gluLookAt(0, 0.0, -0.3, 0, 0, 0, 0, 1, 0);
	//gluLookAt(1, 0, 0, 0, 0, 0, 0, 1, 0);

	DrawingAxis();//Draw x y z axis
	ReadBunny();//Bunny 좌표 받아오기

	//토끼 모양 점 찍기
	//glBegin(GL_POINTS);
	//for (int i = 0; i < pointNum*3; i+=3){
	//	glVertex3f(bunnyPoint[i], bunnyPoint[i + 1], bunnyPoint[i + 2]);
	//}
	//glEnd();
	
	//삼각형의 세 꼭지점 좌표를 저장하기 위한 변수
	float triVertex1[3];
	float triVertex2[3];
	float triVertex3[3];
	//법선 벡터 좌표를 저장할 변수
	float normalVec[3];
	//도형을 생성하는 변수
	GLUquadric *sphere;
	GLUquadric *cylinder;
	GLUquadric *horn;
	sphere = gluNewQuadric();
	cylinder = gluNewQuadric();
	horn = gluNewQuadric();
	//물질의 속성 기술		
	GLfloat mat_diffuse[4] = { 0.2,0.5,0.8,1 };
	GLfloat mat_ambient[4] =  {0.1,0.1,0.1,1};
	GLfloat mat_specular[4] = { 1,1,1,1 };
	GLfloat mat_shininess = 400.0;
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	if (pRot == 1) {///////Point light (light0)
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glPushMatrix();
		glRotatef(pointLightTheta, 1, 1, 1);
		GLfloat pointAmbientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		GLfloat pointDiffuseLight[] = { 0.2f, 0.5f,0.8f, 1.0f };
		GLfloat pointSpecularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat pointLightPos[] = { 0.8, 0, 0, 1.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, pointAmbientLight);
		glLightfv(GL_LIGHT0, GL_POSITION, pointLightPos);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, pointDiffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, pointSpecularLight);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.3);
		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
		glPopMatrix();

		glColor3f(0.8, 0.8, 0.4);
		glDisable(GL_LIGHTING);
		//point light source
		glPushMatrix();
		glRotatef(pointLightSourceTheta, 1, 1, 1);
		glTranslatef(0.8, 0.0, 0.0);
		gluSphere(sphere, 0.05f, 10, 10);
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}
	else
		glDisable(GL_LIGHT0);	
	
	if (dRot == 1) {///////Directional light (light2)		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT2);
		glPushMatrix();
		glRotatef(directionalLightTheta, 1, 1, 1);
		GLfloat directionalAmbientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		GLfloat directionalSpecularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat directionalDiffuseLight[] = { 0.2f, 0.5f,0.8f, 0.0f };
		GLfloat directionalLightPos[] = { 0,0.8,0,0 };
		glLightfv(GL_LIGHT2, GL_POSITION, directionalLightPos);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, directionalDiffuseLight);
		glLightfv(GL_LIGHT0, GL_SPECULAR, directionalSpecularLight);
		glLightfv(GL_LIGHT0, GL_AMBIENT, directionalAmbientLight);
		glPopMatrix();

		//directional light source
		glDisable(GL_LIGHTING);
		glColor3f(0.8, 0.8, 0.4);
		glPushMatrix();
		glRotatef(directionalLightSourceTheta, 1, 1, 1);
		glTranslatef(0.0, 1.2, 0.0);
		glRotatef(55, 1, 1, 1);
		gluCylinder(cylinder, 0.05, 0.05, 0.2, 30, 10);
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}
	else
		glDisable(GL_LIGHT2);
	
	if (sRot == 1) {////////Spot light (light1)		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);		
		glPushMatrix();
		glRotatef(spotLightTheta, 1, 1, 1);
		GLfloat spotLightPos[] = { 0.42, 0.6, 0.7,1 };
		GLfloat	spotDiffuseLight[] = { 1, 1,0, 1 };
		GLfloat spotAmbientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
		GLfloat spotSpecularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT1, GL_SPECULAR, spotSpecularLight);
		glLightfv(GL_LIGHT1, GL_AMBIENT, spotAmbientLight);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, spotDiffuseLight);
		glLightfv(GL_LIGHT1, GL_POSITION, spotLightPos);
		GLfloat spot_direction[3] = { -0.1,-0.1,-0.1 };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotCutOffTheta);
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 50);
		glPopMatrix();

		//spot light source
		glDisable(GL_LIGHTING);
		glColor3f(0.8, 0.8, 0.4);
		glPushMatrix();
		glRotatef(spotLightSourceTheta, 1, 1, 1);
		glTranslatef(0.0, 0.0, 1.2);
		glRotatef(180, 1, 1, 1);
		gluCylinder(horn, 0, spotLightSourceCutOffAngle, 0.3, 30, 10);
		glPopMatrix();
		glEnable(GL_LIGHTING);
	}
	else
		glDisable(GL_LIGHT1);
	
	
	
	glEnable(GL_COLOR_MATERIAL);
	//Draw the bunny model	
	for (int i = 0; i < triNum * 3; i += 3) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		
		glColor3f(0.1f, 0.7f, 1.2f);
		glBegin(GL_TRIANGLES);		
		glVertex3f(bunnyPoint[(3 * (triIndex[i]-1))], bunnyPoint[(3 * (triIndex[i] - 1))+1], bunnyPoint[(3 * (triIndex[i] - 1)) + 2]);
		glVertex3f(bunnyPoint[(3 * (triIndex[i+1] - 1))], bunnyPoint[(3 * (triIndex[i+1] - 1)) + 1], bunnyPoint[(3 * (triIndex[i+1] - 1)) + 2]);
		glVertex3f(bunnyPoint[(3 * (triIndex[i+2] - 1))], bunnyPoint[(3 * (triIndex[i+2] - 1)) + 1], bunnyPoint[(3 * (triIndex[i+2] - 1)) + 2]);
		glEnd();
		//삼각형의 세 꼭지점을 CrossProduct함수에 전달하여 외적을 통해 법선 벡터를 구함.
		triVertex1[0] = bunnyPoint[(3 * (triIndex[i] - 1))];
		triVertex1[1] = bunnyPoint[(3 * (triIndex[i] - 1)) + 1];
		triVertex1[2] = bunnyPoint[(3 * (triIndex[i] - 1)) + 2];
		triVertex2[0] = bunnyPoint[(3 * (triIndex[i + 1] - 1))];
		triVertex2[1] = bunnyPoint[(3 * (triIndex[i + 1] - 1)) + 1];
		triVertex2[2] = bunnyPoint[(3 * (triIndex[i + 1] - 1)) + 2];
		triVertex3[0] = bunnyPoint[(3 * (triIndex[i + 2] - 1))];
		triVertex3[1] = bunnyPoint[(3 * (triIndex[i + 2] - 1)) + 1];
		triVertex3[2] = bunnyPoint[(3 * (triIndex[i + 2] - 1)) + 2];
		CrossProduct(triVertex1, triVertex2, triVertex3, normalVec);
		//법선 벡터를 따로 저장해 둠.
		triNormalVector[i/3][0] = normalVec[0]*0.3;
		triNormalVector[i/3][1] = normalVec[1]*0.3;
		triNormalVector[i/3][2] = normalVec[2]*0.3;
		//off setting을 위한 값 적용.
		normalVec[0] *= 0.0022;
		normalVec[1] *= 0.0022;
		normalVec[2] *= 0.0022;

		glDisable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(0.0f, 0.0f, 0.0f);
		//glDisable(GL_LIGHTING);
		glBegin(GL_TRIANGLES);
		glNormal3f(triNormalVector[i / 3][0], triNormalVector[i / 3][1], triNormalVector[i / 3][2]);
		glVertex3f(bunnyPoint[(3 * (triIndex[i] - 1))]+ normalVec[0], bunnyPoint[(3 * (triIndex[i] - 1)) + 1]+ normalVec[1], bunnyPoint[(3 * (triIndex[i] - 1)) + 2]+ normalVec[2]);
		glVertex3f(bunnyPoint[(3 * (triIndex[i + 1] - 1))]+ normalVec[0], bunnyPoint[(3 * (triIndex[i + 1] - 1)) + 1]+ normalVec[1], bunnyPoint[(3 * (triIndex[i + 1] - 1)) + 2]+ normalVec[2]);
		glVertex3f(bunnyPoint[(3 * (triIndex[i + 2] - 1))]+ normalVec[0], bunnyPoint[(3 * (triIndex[i + 2] - 1)) + 1]+ normalVec[1], bunnyPoint[(3 * (triIndex[i + 2] - 1)) + 2]+ normalVec[2]);
		glEnd();	
		glEnable(GL_LIGHTING);
	}
	
	//Draw the vertex normal vectors	
	if (nVecOn == 1) {
		glDisable(GL_LIGHTING);
		for (int i = 0; i < triNum*3; i+=3) {
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			
			glColor3f(0.0f, 0.0f, 0.0f);
			glBegin(GL_TRIANGLES);
			glVertex3f(bunnyPoint[(3 * (triIndex[i] - 1))] + normalVec[0], bunnyPoint[(3 * (triIndex[i] - 1)) + 1] + normalVec[1], bunnyPoint[(3 * (triIndex[i] - 1)) + 2] + normalVec[2]);
			glVertex3f(bunnyPoint[(3 * (triIndex[i + 1] - 1))] + normalVec[0], bunnyPoint[(3 * (triIndex[i + 1] - 1)) + 1] + normalVec[1], bunnyPoint[(3 * (triIndex[i + 1] - 1)) + 2] + normalVec[2]);
			glVertex3f(bunnyPoint[(3 * (triIndex[i + 2] - 1))] + normalVec[0], bunnyPoint[(3 * (triIndex[i + 2] - 1)) + 1] + normalVec[1], bunnyPoint[(3 * (triIndex[i + 2] - 1)) + 2] + normalVec[2]);
			glEnd();
			//삼각형의 한 vertex에서부터 법선 벡터를 그림 
			glBegin(GL_LINES);
			glVertex3f(bunnyPoint[(3 * (triIndex[i] - 1))] + normalVec[0], bunnyPoint[(3 * (triIndex[i] - 1)) + 1] + normalVec[1], bunnyPoint[(3 * (triIndex[i] - 1)) + 2] + normalVec[2]);
			glVertex3f(bunnyPoint[(3 * (triIndex[i] - 1))] + normalVec[0] + triNormalVector[i / 3][0], bunnyPoint[(3 * (triIndex[i] - 1)) + 1] + normalVec[1] + triNormalVector[i / 3][1], bunnyPoint[(3 * (triIndex[i] - 1)) + 2] + normalVec[2] + triNormalVector[i / 3][2]);
			glEnd();
			
		}		
		glEnable(GL_LIGHTING);
	}
	
	free(bunnyPoint);
	free(triIndex);
	
	glutSwapBuffers();
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);//glut라이브러리 초기화, 윈도우 시스템과 세션을 연다 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);//rgb, 더블 버퍼링 
	glutInitWindowSize(600, 600);
	glutCreateWindow("Lighting with bunny");
	glutDisplayFunc(LightingBunny);
	glutReshapeFunc(ChangeSize);//크기가 변경되면 콜백 함수 호출
	glutKeyboardFunc(keyboard);
	glutTimerFunc(20, timer, 1);
	SetupRC();
	glutMainLoop();
}