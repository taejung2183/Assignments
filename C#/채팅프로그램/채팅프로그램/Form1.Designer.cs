namespace 채팅프로그램 {
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
            this.txt_All = new System.Windows.Forms.TextBox();
            this.txt_Send = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btn_Server = new System.Windows.Forms.Button();
            this.btn_Connect = new System.Windows.Forms.Button();
            this.btn_Exit = new System.Windows.Forms.Button();
            this.btn_Send = new System.Windows.Forms.Button();
            this.txt_ServerIp = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // txt_All
            // 
            this.txt_All.Location = new System.Drawing.Point(36, 39);
            this.txt_All.Multiline = true;
            this.txt_All.Name = "txt_All";
            this.txt_All.Size = new System.Drawing.Size(227, 277);
            this.txt_All.TabIndex = 0;
            // 
            // txt_Send
            // 
            this.txt_Send.Location = new System.Drawing.Point(38, 332);
            this.txt_Send.Name = "txt_Send";
            this.txt_Send.Size = new System.Drawing.Size(224, 21);
            this.txt_Send.TabIndex = 1;
            this.txt_Send.TextChanged += new System.EventHandler(this.txt_Send_TextChanged);
            this.txt_Send.KeyDown += new System.Windows.Forms.KeyEventHandler(this.txt_Send_KeyDown);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.txt_ServerIp);
            this.groupBox1.Controls.Add(this.btn_Connect);
            this.groupBox1.Controls.Add(this.btn_Server);
            this.groupBox1.Location = new System.Drawing.Point(297, 39);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(138, 160);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "옵션";
            // 
            // btn_Server
            // 
            this.btn_Server.Location = new System.Drawing.Point(20, 26);
            this.btn_Server.Name = "btn_Server";
            this.btn_Server.Size = new System.Drawing.Size(106, 19);
            this.btn_Server.TabIndex = 0;
            this.btn_Server.Text = "서버 켜기";
            this.btn_Server.UseVisualStyleBackColor = true;
            this.btn_Server.Click += new System.EventHandler(this.btn_Server_Click);
            // 
            // btn_Connect
            // 
            this.btn_Connect.ForeColor = System.Drawing.Color.Red;
            this.btn_Connect.Location = new System.Drawing.Point(20, 56);
            this.btn_Connect.Name = "btn_Connect";
            this.btn_Connect.Size = new System.Drawing.Size(105, 22);
            this.btn_Connect.TabIndex = 1;
            this.btn_Connect.Text = "서버 연결";
            this.btn_Connect.UseVisualStyleBackColor = true;
            this.btn_Connect.Click += new System.EventHandler(this.btn_Connect_Click);
            // 
            // btn_Exit
            // 
            this.btn_Exit.Location = new System.Drawing.Point(298, 291);
            this.btn_Exit.Name = "btn_Exit";
            this.btn_Exit.Size = new System.Drawing.Size(124, 24);
            this.btn_Exit.TabIndex = 3;
            this.btn_Exit.Text = "프로그램 종료";
            this.btn_Exit.UseVisualStyleBackColor = true;
            this.btn_Exit.Click += new System.EventHandler(this.btn_Exit_Click);
            // 
            // btn_Send
            // 
            this.btn_Send.Location = new System.Drawing.Point(298, 327);
            this.btn_Send.Name = "btn_Send";
            this.btn_Send.Size = new System.Drawing.Size(123, 25);
            this.btn_Send.TabIndex = 4;
            this.btn_Send.Text = "보내기";
            this.btn_Send.UseVisualStyleBackColor = true;
            this.btn_Send.Click += new System.EventHandler(this.btn_Send_Click);
            this.btn_Send.KeyDown += new System.Windows.Forms.KeyEventHandler(this.btn_Send_KeyDown);
            // 
            // txt_ServerIp
            // 
            this.txt_ServerIp.Location = new System.Drawing.Point(27, 113);
            this.txt_ServerIp.Name = "txt_ServerIp";
            this.txt_ServerIp.Size = new System.Drawing.Size(98, 21);
            this.txt_ServerIp.TabIndex = 2;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.btn_Send);
            this.Controls.Add(this.btn_Exit);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.txt_Send);
            this.Controls.Add(this.txt_All);
            this.Name = "Form1";
            this.Text = "Chatting";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox txt_All;
        private System.Windows.Forms.TextBox txt_Send;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox txt_ServerIp;
        private System.Windows.Forms.Button btn_Connect;
        private System.Windows.Forms.Button btn_Server;
        private System.Windows.Forms.Button btn_Exit;
        private System.Windows.Forms.Button btn_Send;
    }
}

