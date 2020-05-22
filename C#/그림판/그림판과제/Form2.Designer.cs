namespace 그림판과제 {
    partial class Form2 {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.doubleBufferPanel1 = new 그림판과제.doublebuffer.DoubleBufferPanel();
            this.SuspendLayout();
            // 
            // doubleBufferPanel1
            // 
            this.doubleBufferPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.doubleBufferPanel1.Location = new System.Drawing.Point(0, 0);
            this.doubleBufferPanel1.Name = "doubleBufferPanel1";
            this.doubleBufferPanel1.Size = new System.Drawing.Size(411, 349);
            this.doubleBufferPanel1.TabIndex = 0;
            this.doubleBufferPanel1.Paint += new System.Windows.Forms.PaintEventHandler(this.doubleBufferPanel1_Paint);
            this.doubleBufferPanel1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.doubleBufferPanel1_MouseDown);
            this.doubleBufferPanel1.MouseMove += new System.Windows.Forms.MouseEventHandler(this.doubleBufferPanel1_MouseMove);
            this.doubleBufferPanel1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.doubleBufferPanel1_MouseUp);
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(411, 349);
            this.Controls.Add(this.doubleBufferPanel1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form2";
            this.Text = "제목 없음";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form2_FormClosing);
            this.Load += new System.EventHandler(this.Form2_Load);
            this.ResumeLayout(false);

        }

        #endregion

        public 그림판과제.doublebuffer.DoubleBufferPanel doubleBufferPanel1;
    }
}