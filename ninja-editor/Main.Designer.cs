namespace MapEditor
{
    partial class Main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.TreeNode treeNode15 = new System.Windows.Forms.TreeNode("npc2");
            System.Windows.Forms.TreeNode treeNode16 = new System.Windows.Forms.TreeNode("fg", new System.Windows.Forms.TreeNode[] {
            treeNode15});
            System.Windows.Forms.TreeNode treeNode17 = new System.Windows.Forms.TreeNode("wall");
            System.Windows.Forms.TreeNode treeNode18 = new System.Windows.Forms.TreeNode("bg", new System.Windows.Forms.TreeNode[] {
            treeNode17});
            System.Windows.Forms.TreeNode treeNode19 = new System.Windows.Forms.TreeNode("cloud1");
            System.Windows.Forms.TreeNode treeNode20 = new System.Windows.Forms.TreeNode("cloud6");
            System.Windows.Forms.TreeNode treeNode21 = new System.Windows.Forms.TreeNode("overlays", new System.Windows.Forms.TreeNode[] {
            treeNode19,
            treeNode20});
            this.FastTimer = new System.Windows.Forms.Timer(this.components);
            this.treeObjects = new System.Windows.Forms.TreeView();
            this.btn_GetObjects = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // FastTimer
            // 
            this.FastTimer.Interval = 5;
            this.FastTimer.Tick += new System.EventHandler(this.OnTick);
            // 
            // treeObjects
            // 
            this.treeObjects.Location = new System.Drawing.Point(12, 12);
            this.treeObjects.Name = "treeObjects";
            treeNode15.Name = "Node6";
            treeNode15.Text = "npc2";
            treeNode16.Name = "root1";
            treeNode16.Text = "fg";
            treeNode17.Name = "Node7";
            treeNode17.Text = "wall";
            treeNode18.Name = "root2";
            treeNode18.Text = "bg";
            treeNode19.Name = "Node3";
            treeNode19.Text = "cloud1";
            treeNode20.Name = "Node5";
            treeNode20.Text = "cloud6";
            treeNode21.Name = "root3";
            treeNode21.Text = "overlays";
            this.treeObjects.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode16,
            treeNode18,
            treeNode21});
            this.treeObjects.Size = new System.Drawing.Size(187, 271);
            this.treeObjects.TabIndex = 0;
            // 
            // btn_GetObjects
            // 
            this.btn_GetObjects.Location = new System.Drawing.Point(13, 290);
            this.btn_GetObjects.Name = "btn_GetObjects";
            this.btn_GetObjects.Size = new System.Drawing.Size(83, 23);
            this.btn_GetObjects.TabIndex = 1;
            this.btn_GetObjects.Text = "Get";
            this.btn_GetObjects.UseVisualStyleBackColor = true;
            this.btn_GetObjects.Click += new System.EventHandler(this.btn_GetObjects_Click);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(405, 313);
            this.Controls.Add(this.btn_GetObjects);
            this.Controls.Add(this.treeObjects);
            this.Name = "Main";
            this.Text = "Ninja-Editor";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            this.Load += new System.EventHandler(this.Main_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Timer FastTimer;
        private System.Windows.Forms.TreeView treeObjects;
        private System.Windows.Forms.Button btn_GetObjects;
    }
}

