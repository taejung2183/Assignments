namespace 탐색기 {
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.ListViewGroup listViewGroup1 = new System.Windows.Forms.ListViewGroup("ListViewGroup", System.Windows.Forms.HorizontalAlignment.Left);
            System.Windows.Forms.ListViewGroup listViewGroup2 = new System.Windows.Forms.ListViewGroup("ListViewGroup", System.Windows.Forms.HorizontalAlignment.Left);
            System.Windows.Forms.ListViewGroup listViewGroup3 = new System.Windows.Forms.ListViewGroup("ListViewGroup", System.Windows.Forms.HorizontalAlignment.Left);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.panel1 = new System.Windows.Forms.Panel();
            this.txtPath = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.splitter1 = new System.Windows.Forms.Splitter();
            this.lvwFiles = new System.Windows.Forms.ListView();
            this.cmuListView = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.mnuOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuDetail = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuList = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSmall = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuLarge = new System.Windows.Forms.ToolStripMenuItem();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.trvDir = new System.Windows.Forms.TreeView();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.cmuListView.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.txtPath);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(473, 30);
            this.panel1.TabIndex = 0;
            // 
            // txtPath
            // 
            this.txtPath.Location = new System.Drawing.Point(73, 2);
            this.txtPath.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.txtPath.Name = "txtPath";
            this.txtPath.Size = new System.Drawing.Size(400, 21);
            this.txtPath.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(7, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 12);
            this.label1.TabIndex = 0;
            this.label1.Text = "전체경로";
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.splitter1);
            this.panel2.Controls.Add(this.lvwFiles);
            this.panel2.Controls.Add(this.trvDir);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 30);
            this.panel2.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(473, 396);
            this.panel2.TabIndex = 1;
            // 
            // splitter1
            // 
            this.splitter1.Location = new System.Drawing.Point(96, 0);
            this.splitter1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.splitter1.Name = "splitter1";
            this.splitter1.Size = new System.Drawing.Size(7, 396);
            this.splitter1.TabIndex = 2;
            this.splitter1.TabStop = false;
            // 
            // lvwFiles
            // 
            this.lvwFiles.ContextMenuStrip = this.cmuListView;
            this.lvwFiles.Dock = System.Windows.Forms.DockStyle.Fill;
            listViewGroup1.Header = "ListViewGroup";
            listViewGroup1.Name = "colFileName";
            listViewGroup2.Header = "ListViewGroup";
            listViewGroup2.Name = "colFileSize";
            listViewGroup3.Header = "ListViewGroup";
            listViewGroup3.Name = "colFileDate";
            this.lvwFiles.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup1,
            listViewGroup2,
            listViewGroup3});
            this.lvwFiles.LargeImageList = this.imageList1;
            this.lvwFiles.Location = new System.Drawing.Point(96, 0);
            this.lvwFiles.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.lvwFiles.Name = "lvwFiles";
            this.lvwFiles.Size = new System.Drawing.Size(377, 396);
            this.lvwFiles.SmallImageList = this.imageList1;
            this.lvwFiles.TabIndex = 1;
            this.lvwFiles.UseCompatibleStateImageBehavior = false;
            this.lvwFiles.DoubleClick += new System.EventHandler(this.lvwFiles_DoubleClick);
            // 
            // cmuListView
            // 
            this.cmuListView.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.cmuListView.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuOpen,
            this.mnuDetail,
            this.mnuList,
            this.mnuSmall,
            this.mnuLarge});
            this.cmuListView.Name = "contextMenuStrip1";
            this.cmuListView.Size = new System.Drawing.Size(135, 114);
            // 
            // mnuOpen
            // 
            this.mnuOpen.Name = "mnuOpen";
            this.mnuOpen.Size = new System.Drawing.Size(134, 22);
            this.mnuOpen.Text = "열기";
            this.mnuOpen.Click += new System.EventHandler(this.mnuOpen_Click);
            // 
            // mnuDetail
            // 
            this.mnuDetail.Name = "mnuDetail";
            this.mnuDetail.Size = new System.Drawing.Size(134, 22);
            this.mnuDetail.Text = "자세히";
            this.mnuDetail.Click += new System.EventHandler(this.mnuView_Click);
            // 
            // mnuList
            // 
            this.mnuList.Name = "mnuList";
            this.mnuList.Size = new System.Drawing.Size(134, 22);
            this.mnuList.Text = "간단히";
            // 
            // mnuSmall
            // 
            this.mnuSmall.Name = "mnuSmall";
            this.mnuSmall.Size = new System.Drawing.Size(134, 22);
            this.mnuSmall.Text = "작은아이콘";
            // 
            // mnuLarge
            // 
            this.mnuLarge.Name = "mnuLarge";
            this.mnuLarge.Size = new System.Drawing.Size(134, 22);
            this.mnuLarge.Text = "큰아이콘";
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "과제1결과화면.JPG");
            this.imageList1.Images.SetKeyName(1, "AnonymousMethod - 복사본.PNG");
            this.imageList1.Images.SetKeyName(2, "delegate - 복사본.PNG");
            this.imageList1.Images.SetKeyName(3, "Generic - 복사본.PNG");
            // 
            // trvDir
            // 
            this.trvDir.Dock = System.Windows.Forms.DockStyle.Left;
            this.trvDir.ImageIndex = 0;
            this.trvDir.ImageList = this.imageList1;
            this.trvDir.Location = new System.Drawing.Point(0, 0);
            this.trvDir.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.trvDir.Name = "trvDir";
            this.trvDir.SelectedImageIndex = 0;
            this.trvDir.Size = new System.Drawing.Size(96, 396);
            this.trvDir.TabIndex = 0;
            this.trvDir.BeforeExpand += new System.Windows.Forms.TreeViewCancelEventHandler(this.trvDir_BeforeExpand);
            this.trvDir.BeforeSelect += new System.Windows.Forms.TreeViewCancelEventHandler(this.trvDir_BeforeSelect);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(473, 426);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.cmuListView.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox txtPath;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Splitter splitter1;
        private System.Windows.Forms.ListView lvwFiles;
        private System.Windows.Forms.ContextMenuStrip cmuListView;
        private System.Windows.Forms.ToolStripMenuItem mnuOpen;
        private System.Windows.Forms.ToolStripMenuItem mnuDetail;
        private System.Windows.Forms.ToolStripMenuItem mnuList;
        private System.Windows.Forms.ToolStripMenuItem mnuSmall;
        private System.Windows.Forms.ToolStripMenuItem mnuLarge;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.TreeView trvDir;
    }
}

