using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace 그림판과제 {
    public partial class Form1 : Form {
        Form2 newPage=null;
        Image image;
        public int nNewPage=0;

        public Form1() {
            InitializeComponent();
            InitializeFileDialog();
            image=null;
        }

        public void setChildPageNum(int num) {
            nNewPage= (num==1)? nNewPage+1 : nNewPage-1;    
            if(nNewPage==0) 저장SToolStripMenuItem.Enabled = false;
            else 저장SToolStripMenuItem.Enabled = true;
        }

        private void InitializeFileDialog() {
            openFileDialog1.InitialDirectory=Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            openFileDialog1.Filter= "JPG (*.jpg)|*.jpg| JPEG (*.jpeg)|*jpeg| BMP (*.bmp)|*.bmp| GIF (*.gif)|*.gif| PNG (*.png)|*.png| 모든 파일 (*.*)|*.*";
            openFileDialog1.FileName="";

            saveFileDialog1.InitialDirectory=openFileDialog1.InitialDirectory;
            saveFileDialog1.Filter=openFileDialog1.Filter;
            saveFileDialog1.FileName="*.jpg";
        }   

        private void Form1_Load(object sender, EventArgs e) {
            if(nNewPage==0)
                저장SToolStripMenuItem.Enabled=false;
            panel1.Visible = false;
            panel2.Visible=false;
            button1.Enabled = false;
            button6.Enabled=false;
        }

        private void 새로만들기NToolStripMenuItem_Click(object sender, EventArgs e) {
            newPage=new Form2();
            newPage.MdiParent=this;
            newPage.Text="제목 없음";
            newPage.WindowState=System.Windows.Forms.FormWindowState.Maximized;
            newPage.Show();
            setChildPageNum(1);

            newPage.parent = this;//자식이 부모의 필드에 접근하게 하기 위한 초기화
            newPage.pen=new Pen(toolStripButton1.ForeColor);
            newPage.rectangleBrush = new SolidBrush(toolStripButton1.ForeColor);
            newPage.ovalBrush = new SolidBrush(toolStripButton1.ForeColor);

            //새로 만들면 기본 동작은 선 그리기
            newPage.freeLine = true;
            newPage.color = toolStripButton1.ForeColor;
        }

        private void 열기OToolStripMenuItem_Click(object sender, EventArgs e) {
            //현재 열려있는 창이 있는 경우
            if (newPage != null) {                
                newPage.Close();
                newPage = null;
            }
            if (openFileDialog1.ShowDialog() == DialogResult.OK) {
                //자식 폼 생성
                newPage=new Form2();
                newPage.MdiParent = this;
                newPage.Text=Path.GetFileName(openFileDialog1.SafeFileName);
                newPage.Show();

                image=Image.FromFile(openFileDialog1.FileName);
                newPage.DrawImage(image);
                setChildPageNum(1);

                newPage.parent = this;//자식이 부모의 필드에 접근하게 하기 위한 초기화
                newPage.pen = new Pen(toolStripButton1.ForeColor);
                newPage.rectangleBrush = new SolidBrush(toolStripButton1.ForeColor);
                newPage.ovalBrush = new SolidBrush(toolStripButton1.ForeColor);

                newPage.freeLine = true;
                newPage.color = toolStripButton1.ForeColor;
            }
        }

        public void 저장SToolStripMenuItem_Click(object sender, EventArgs e) {
            string path = null;
            Bitmap bmp = new Bitmap(newPage.doubleBufferPanel1.Width, newPage.doubleBufferPanel1.Height);
            newPage.doubleBufferPanel1.DrawToBitmap(bmp, newPage.doubleBufferPanel1.ClientRectangle);
            System.Drawing.Imaging.ImageFormat fmt=null;
            int index;

            if (newPage.Text.Equals("제목 없음")) {
                if (saveFileDialog1.ShowDialog() == DialogResult.OK) {
                    //제목 없음인 경우 대화상자 열기                    
                    path = saveFileDialog1.FileName;                                        
                    if (image != null) image.Dispose();

                    try {
                        if (File.Exists(path)) {
                            File.Delete(path);
                        }
                        
                        index=saveFileDialog1.FilterIndex;

                        switch (index) {
                            case 1:
                                fmt=System.Drawing.Imaging.ImageFormat.Jpeg;
                                break;
                            case 2:
                                fmt = System.Drawing.Imaging.ImageFormat.Jpeg;
                                break;
                            case 3:
                                fmt = System.Drawing.Imaging.ImageFormat.Bmp;
                                break;
                            case 4:
                                fmt = System.Drawing.Imaging.ImageFormat.Gif;
                                break;
                            case 5:
                                fmt = System.Drawing.Imaging.ImageFormat.Png;
                                break;
                        }
                        
                        bmp.Save(path, fmt);
                    } finally {
                        bmp.Dispose();
                    }
                    newPage.Text= Path.GetFileName(path);
                }
                else return;
            }
            else {//제목이 있는 경우
                path=openFileDialog1.FileName;
                if (image != null) image.Dispose();

                try {//해당 경로에 파일이 있으면 삭제한 뒤 다시 저장
                    if (File.Exists(path)) {
                        File.Delete(path);
                    }
                    index=openFileDialog1.FilterIndex;
                    switch (index) {
                        case 1:
                            fmt = System.Drawing.Imaging.ImageFormat.Jpeg;
                            break;
                        case 2:
                            fmt = System.Drawing.Imaging.ImageFormat.Jpeg;
                            break;
                        case 3:
                            fmt = System.Drawing.Imaging.ImageFormat.Bmp;
                            break;
                        case 4:
                            fmt = System.Drawing.Imaging.ImageFormat.Gif;
                            break;
                        case 5:
                            fmt = System.Drawing.Imaging.ImageFormat.Png;
                            break;
                    }
                    bmp.Save(path, fmt);
                } finally {
                    bmp.Dispose();
                }
            }
        }
        //현재 선택된 선 두께에 맞게 pen 두께를 적용
        private void penWidth() {
            if (!button1.Enabled) {
                newPage.thick=1;
            }
            else if (!button2.Enabled) {
                newPage.thick = 2;
            }
            else if (!button3.Enabled) {
                newPage.thick = 3;
            }
            else if (!button4.Enabled) {
                newPage.thick = 4;
            }
            else {
                newPage.thick = 5;
            }
        }
    
        private void toolStrip1_ItemClicked_1(object sender, ToolStripItemClickedEventArgs e) {
            //연필 클릭      
            if (e.ClickedItem == toolStripButton2) {
                if (newPage == null) return;
                newPage.freeLine = true;
                newPage.line=false;
                newPage.rectangle=false;
                newPage.oval=false;
                penWidth();
            }

            //브러쉬 클릭
            if (e.ClickedItem == toolStripButton3) {
                if (newPage == null) return;
                newPage.freeLine = true;
                newPage.line = false;
                newPage.rectangle = false;
                newPage.oval = false;
                newPage.thick = 5;
            }

            //직선
            if (e.ClickedItem == toolStripButton4) {
                if (newPage == null) return;
                newPage.freeLine = false;
                newPage.line = true;
                newPage.rectangle = false;
                newPage.oval = false;
                penWidth();

            }

            //사각형
            if (e.ClickedItem == toolStripButton5) {
                if (newPage == null) return;
                newPage.freeLine = false;
                newPage.line = false;
                newPage.rectangle = true;
                newPage.oval = false;
                //단색 채우기가 선택되어 있는 경우
                if (newPage!=null && button6.Enabled == true) 
                    newPage.fill=true;
                penWidth();
            }

            //타원
            if (e.ClickedItem == toolStripButton6) {
                if (newPage == null) return;
                newPage.freeLine = false;
                newPage.line = false;
                newPage.rectangle = false;
                newPage.oval = true;
                if (newPage!=null && button6.Enabled == true)
                    newPage.fill = true;
                penWidth();
            }
            //선 두께
            if (e.ClickedItem == toolStripButton7) {
                panel1.Visible=true;
                panel2.Visible=false;
            }
            //단색 채우기
            if (e.ClickedItem == toolStripButton8) {
                panel2.Visible=true;
                panel1.Visible=false;
            }
        }
        //두께1
        private void button1_Click(object sender, EventArgs e) {
            button1.Enabled=false;
            button2.Enabled = true;
            button3.Enabled = true;
            button4.Enabled = true;
            button5.Enabled = true;
            panel1.Visible=false;
            newPage.thick = 1;

        }
        //두께2
        private void button2_Click(object sender, EventArgs e) {
            button1.Enabled = true;
            button2.Enabled = false;
            button3.Enabled = true;
            button4.Enabled = true;
            button5.Enabled = true;
            panel1.Visible = false;
            newPage.thick = 2;
        }
        //두께3
        private void button3_Click(object sender, EventArgs e) {
            button1.Enabled = true;
            button2.Enabled = true;
            button3.Enabled = false;
            button4.Enabled = true;
            button5.Enabled = true;
            panel1.Visible=false;
            newPage.thick = 3;
        }
        //두께4
        private void button4_Click(object sender, EventArgs e) {
            button1.Enabled = true;
            button2.Enabled = true;
            button3.Enabled = true;
            button4.Enabled = false;
            button5.Enabled = true;
            panel1.Visible = false;
            newPage.thick = 4;
        }
        //두께5
        private void button5_Click(object sender, EventArgs e) {
            button1.Enabled = true;
            button2.Enabled = true;
            button3.Enabled = true;
            button4.Enabled = true;
            button5.Enabled = false;
            panel1.Visible = false;
            newPage.thick = 5;
        }
        //채우기 없음
        private void button6_Click(object sender, EventArgs e) {
            button6.Enabled=false;
            button7.Enabled=true;
            panel2.Visible=false;
            if(newPage!=null)
                newPage.fill=false;
        }
        //단색 채우기
        private void button7_Click(object sender, EventArgs e) {
            button6.Enabled = true;
            button7.Enabled = false;
            panel2.Visible = false;
            if(newPage!=null)
                newPage.fill=true;
        }
        //색상 버튼
        private void toolStripButton1_Click(object sender, EventArgs e) {
            if (colorDialog1.ShowDialog() == DialogResult.OK) {
                //색상 변경 버튼의 색 변경
                toolStripButton1.ForeColor = toolStripButton1.BackColor = colorDialog1.Color;
                //바뀐 색으로 펜 색상 변경
                if (newPage != null) {
                    newPage.color = toolStripButton1.ForeColor;
                }
            }
        }
        //이미지 지우기
        private void 이미지지우기NToolStripMenuItem_Click(object sender, EventArgs e) {
            if(newPage==null)
                return;
            newPage.SetupMine();
            newPage.image=null;
            newPage.doubleBufferPanel1.Invalidate();
        }
        //끝내기
        private void 끝내기XToolStripMenuItem_Click(object sender, EventArgs e) {
            Application.Exit();
        }
    }
}
