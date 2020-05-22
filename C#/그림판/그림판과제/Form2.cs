using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;

struct Constant {
    public const int MAX_NUM_OF_DRAWINGS = 320000;
}


namespace 그림판과제 {    
      
    public partial class Form2 : Form {
        int i;
        private MyDrawings[] myDrawings;//그림을 저장
        public int nDrawing;//저장된 그림 개수
        //직선
        public bool line;        
        //자유 곡선
        public bool freeLine;
        //사각형
        public bool rectangle;
        //타원
        public bool oval;
        //선 속성
        public int thick;
        public Color color;
        public bool fill;
        public Pen pen;
        public SolidBrush rectangleBrush,ovalBrush;
        private Point start,end;

        public Image image;
        public Form1 parent;

        public Form2() {
            InitializeComponent();
            
            SetupVar();
        }
        //필드값 초기화
        private void SetupVar() {
            image = null;
            i = 0;
            thick = 1;
            fill = false;
            freeLine = false;
            line = false;
            rectangle = false;
            oval = false;
            start = new Point(-1, -1);
            end = new Point(-1, -1);
            myDrawings = new MyDrawings[Constant.MAX_NUM_OF_DRAWINGS];
            nDrawing=0;

            SetupMine();
        }
        //그림 데이터 저장 공간 초기화
        public void SetupMine() {
            for (i = 0; i < Constant.MAX_NUM_OF_DRAWINGS; i++) {
                myDrawings[i] = new MyDrawings();
            }
        }

        private void Form2_Load(object sender, EventArgs e) {

        }

        public void setStart_p(int x, int y) {
            start.X = x;
            start.Y = y;
        }

        public void DrawImage(Image img) {
            image = img;
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e) {
            parent.setChildPageNum(0);        
            DialogResult dr = MessageBox.Show("변경 내용을 저장하시겠습니까?", "알림",MessageBoxButtons.YesNoCancel, MessageBoxIcon.Information);
            if (dr == DialogResult.Yes) parent.저장SToolStripMenuItem_Click(sender,e);           
            else if(dr== DialogResult.No) { }                            
            else {
                parent.setChildPageNum(1);
                e.Cancel=true;
            }
        }

        private void doubleBufferPanel1_Paint(object sender, PaintEventArgs e) {
            if (image != null) {
                e.Graphics.DrawImage(image, 10, 10);
                this.Size = new System.Drawing.Size(image.Width + 16, image.Height + 39);
            }

            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            for (i = 0; i <= nDrawing; i++) {
                switch (myDrawings[i].getType()) {
                    case 0://freeLine
                        pen.Width = myDrawings[i].getThick();
                        pen.Color = myDrawings[i].getColor();
                        e.Graphics.DrawLine(pen, myDrawings[i].getPoint1(), myDrawings[i].getPoint2());
                        break;
                    case 1://line
                        pen.Width = myDrawings[i].getThick();
                        pen.Color = myDrawings[i].getColor();
                        e.Graphics.DrawLine(pen, myDrawings[i].getPoint1(), myDrawings[i].getPoint2());
                        break;
                    case 2://rectangle
                        pen.Width = myDrawings[i].getThick();
                        pen.Color = rectangleBrush.Color = myDrawings[i].getColor();

                        bool fillFlag = myDrawings[i].getFill();
                        if (fillFlag == true)
                            e.Graphics.FillRectangle(rectangleBrush, myDrawings[i].getRect());
                        else
                            e.Graphics.DrawRectangle(pen, myDrawings[i].getRect());
                        break;
                    case 3://oval
                        pen.Width = myDrawings[i].getThick();
                        pen.Color = ovalBrush.Color = myDrawings[i].getColor();
                        bool fillFlag1 = myDrawings[i].getFill();
                        if (fillFlag1 == true)
                            e.Graphics.FillEllipse(ovalBrush, myDrawings[i].getRectC());
                        else
                            e.Graphics.DrawEllipse(pen, myDrawings[i].getRectC());
                        break;
                }
            }
        }

        private void doubleBufferPanel1_MouseDown(object sender, MouseEventArgs e) {
            start.X = e.X;
            start.Y = e.Y;
        }

        private void doubleBufferPanel1_MouseMove(object sender, MouseEventArgs e) {
            if (start.X == -1 && start.Y == -1)
                return;

            end.X = e.X;
            end.Y = e.Y;

            if (freeLine == true) {
                myDrawings[nDrawing++].setPoint(start, end, thick, color, 0);
                start.X = e.X;
                start.Y = e.Y;
            }
            if (line == true) {
                myDrawings[nDrawing].setPoint(start, end, thick, color, 1);
            }
            if (rectangle == true) {
                myDrawings[nDrawing].setRect(start, end, thick, color, fill, 2);
            }
            if (oval == true) {
                myDrawings[nDrawing].setRectC(start, end, thick, color, fill, 3);
            }

            doubleBufferPanel1.Invalidate(true);
            doubleBufferPanel1.Update();
        }

        private void doubleBufferPanel1_MouseUp(object sender, MouseEventArgs e) {
            nDrawing++;

            start.X = -1;
            start.Y = -1;
            end.X = -1;
            end.Y = -1;
        }
    }
}
