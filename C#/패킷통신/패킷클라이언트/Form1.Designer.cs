namespace 패킷클라이언트 {
    partial class Form1 {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent() {
            this.txt_ip = new System.Windows.Forms.TextBox();
            this.txt_init = new System.Windows.Forms.TextBox();
            this.txt_login = new System.Windows.Forms.TextBox();
            this.btn_connect = new System.Windows.Forms.Button();
            this.btn_init = new System.Windows.Forms.Button();
            this.btn_login = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // txt_ip
            // 
            this.txt_ip.Location = new System.Drawing.Point(68, 71);
            this.txt_ip.Name = "txt_ip";
            this.txt_ip.Size = new System.Drawing.Size(323, 21);
            this.txt_ip.TabIndex = 0;
            this.txt_ip.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // txt_init
            // 
            this.txt_init.Location = new System.Drawing.Point(68, 164);
            this.txt_init.Name = "txt_init";
            this.txt_init.Size = new System.Drawing.Size(323, 21);
            this.txt_init.TabIndex = 1;
            // 
            // txt_login
            // 
            this.txt_login.Location = new System.Drawing.Point(68, 261);
            this.txt_login.Name = "txt_login";
            this.txt_login.Size = new System.Drawing.Size(323, 21);
            this.txt_login.TabIndex = 2;
            // 
            // btn_connect
            // 
            this.btn_connect.Location = new System.Drawing.Point(448, 70);
            this.btn_connect.Name = "btn_connect";
            this.btn_connect.Size = new System.Drawing.Size(119, 21);
            this.btn_connect.TabIndex = 3;
            this.btn_connect.Text = "접속";
            this.btn_connect.UseVisualStyleBackColor = true;
            this.btn_connect.Click += new System.EventHandler(this.btn_connect_Click);
            // 
            // btn_init
            // 
            this.btn_init.Location = new System.Drawing.Point(448, 164);
            this.btn_init.Name = "btn_init";
            this.btn_init.Size = new System.Drawing.Size(119, 21);
            this.btn_init.TabIndex = 4;
            this.btn_init.Text = "Init";
            this.btn_init.UseVisualStyleBackColor = true;
            this.btn_init.Click += new System.EventHandler(this.btn_init_Click);
            // 
            // btn_login
            // 
            this.btn_login.Location = new System.Drawing.Point(448, 261);
            this.btn_login.Name = "btn_login";
            this.btn_login.Size = new System.Drawing.Size(119, 21);
            this.btn_login.TabIndex = 5;
            this.btn_login.Text = "Login";
            this.btn_login.UseVisualStyleBackColor = true;
            this.btn_login.Click += new System.EventHandler(this.btn_login_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.btn_login);
            this.Controls.Add(this.btn_init);
            this.Controls.Add(this.btn_connect);
            this.Controls.Add(this.txt_login);
            this.Controls.Add(this.txt_init);
            this.Controls.Add(this.txt_ip);
            this.Name = "Form1";
            this.Text = "Packet Client";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txt_ip;
        private System.Windows.Forms.TextBox txt_init;
        private System.Windows.Forms.TextBox txt_login;
        private System.Windows.Forms.Button btn_connect;
        private System.Windows.Forms.Button btn_init;
        private System.Windows.Forms.Button btn_login;
    }
}

