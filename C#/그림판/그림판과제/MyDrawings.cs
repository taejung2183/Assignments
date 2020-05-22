using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace 그림판과제 {
    class MyDrawings {
        private Rectangle rect;
        private Rectangle rectC;
        private Point[] point = new Point[2];
        private int thick;
        private Color color;
        private bool fill;
        private int drawType;
        //0:freeLine 1:line 2:rectangle 3:oval

        public MyDrawings() {
            point[0] = new Point();
            point[1] = new Point();
            rect = new Rectangle();
            rectC = new Rectangle();
            thick = 1;
            fill = false;
            color = Color.Black;
            drawType=0;
        }

        public void setRect(Point start, Point finish, int thick, Color color, bool fill,int type) {
            rect.X = Math.Min(start.X, finish.X);
            rect.Y = Math.Min(start.Y, finish.Y);
            rect.Width = Math.Abs(start.X - finish.X);
            rect.Height = Math.Abs(start.Y - finish.Y);
            this.thick = thick;
            this.color = color;
            this.fill = fill;
            drawType=type;
        }

        public void setRectC(Point start, Point finish, int thick, Color color, bool fill,int type) {
            rectC.X = Math.Min(start.X, finish.X);
            rectC.Y = Math.Min(start.Y, finish.Y);
            rectC.Width = Math.Abs(start.X - finish.X);
            rectC.Height = Math.Abs(start.Y - finish.Y);
            this.thick = thick;
            this.color = color;
            this.fill = fill;
            drawType=type;
        }
        
        public void setPoint(Point start, Point finish, int thick, Color color,int type) {
            point[0] = start;
            point[1] = finish;
            this.thick = thick;
            this.color = color;
            drawType=type;
        }

        public int getType() {
            return drawType;
        }

        public Point getPoint1() {
            return point[0];
        }

        public Point getPoint2() {
            return point[1];
        }

        public Rectangle getRect() {
            return rect;
        }

        public Rectangle getRectC() {
            return rectC;
        }

        public int getThick() {
            return thick;
        }

        public Color getColor() {
            return color;
        }

        public bool getFill() {
            return fill;
        }

    }
}
