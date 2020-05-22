using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 그림판과제 {
    public partial class doublebuffer : UserControl {
        public class DoubleBufferPanel : Panel {
            public DoubleBufferPanel() {
                this.SetStyle(ControlStyles.DoubleBuffer |
          ControlStyles.UserPaint |
          ControlStyles.AllPaintingInWmPaint,
          true);
                this.UpdateStyles();
            }
        }
        public doublebuffer() {
            InitializeComponent();
        }

        private void doublebuffer_Load(object sender, EventArgs e) {

        }
    }
}
