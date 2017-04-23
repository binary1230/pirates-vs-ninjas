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
            this.FastTimer = new System.Windows.Forms.Timer(this.components);
            this.treeObjects = new System.Windows.Forms.TreeView();
            this.btn_GetObjects = new System.Windows.Forms.Button();
            this.btn_Create = new System.Windows.Forms.Button();
            this.lstObjectDefs = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.lblLayers = new System.Windows.Forms.Label();
            this.btnPause = new System.Windows.Forms.Button();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.chkDrawPhysicsDebug = new System.Windows.Forms.CheckBox();
            this.label3 = new System.Windows.Forms.Label();
            this.lstLayers = new System.Windows.Forms.ListBox();
            this.chkSnapToGrid = new System.Windows.Forms.CheckBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // FastTimer
            // 
            this.FastTimer.Interval = 5;
            this.FastTimer.Tick += new System.EventHandler(this.OnTick);
            // 
            // treeObjects
            // 
            this.treeObjects.Location = new System.Drawing.Point(12, 25);
            this.treeObjects.Name = "treeObjects";
            this.treeObjects.Size = new System.Drawing.Size(187, 264);
            this.treeObjects.TabIndex = 0;
            // 
            // btn_GetObjects
            // 
            this.btn_GetObjects.Location = new System.Drawing.Point(12, 295);
            this.btn_GetObjects.Name = "btn_GetObjects";
            this.btn_GetObjects.Size = new System.Drawing.Size(83, 23);
            this.btn_GetObjects.TabIndex = 1;
            this.btn_GetObjects.Text = "Refresh";
            this.btn_GetObjects.UseVisualStyleBackColor = true;
            this.btn_GetObjects.Click += new System.EventHandler(this.btn_GetObjects_Click);
            // 
            // btn_Create
            // 
            this.btn_Create.Location = new System.Drawing.Point(369, 293);
            this.btn_Create.Name = "btn_Create";
            this.btn_Create.Size = new System.Drawing.Size(147, 23);
            this.btn_Create.TabIndex = 2;
            this.btn_Create.Text = "Create New Object";
            this.btn_Create.UseVisualStyleBackColor = true;
            this.btn_Create.Click += new System.EventHandler(this.btn_Create_Click);
            // 
            // lstObjectDefs
            // 
            this.lstObjectDefs.Location = new System.Drawing.Point(369, 25);
            this.lstObjectDefs.Name = "lstObjectDefs";
            this.lstObjectDefs.Size = new System.Drawing.Size(147, 264);
            this.lstObjectDefs.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(86, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Objects In Game";
            // 
            // lblLayers
            // 
            this.lblLayers.AutoSize = true;
            this.lblLayers.Location = new System.Drawing.Point(211, 9);
            this.lblLayers.Name = "lblLayers";
            this.lblLayers.Size = new System.Drawing.Size(38, 13);
            this.lblLayers.TabIndex = 5;
            this.lblLayers.Text = "Layers";
            // 
            // btnPause
            // 
            this.btnPause.Location = new System.Drawing.Point(116, 295);
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(83, 23);
            this.btnPause.TabIndex = 6;
            this.btnPause.Text = "Pause";
            this.btnPause.UseVisualStyleBackColor = true;
            this.btnPause.Click += new System.EventHandler(this.btnPaused_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.chkSnapToGrid);
            this.groupBox1.Controls.Add(this.chkDrawPhysicsDebug);
            this.groupBox1.Location = new System.Drawing.Point(522, 9);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(169, 100);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Options";
            // 
            // chkDrawPhysicsDebug
            // 
            this.chkDrawPhysicsDebug.AutoSize = true;
            this.chkDrawPhysicsDebug.Location = new System.Drawing.Point(7, 20);
            this.chkDrawPhysicsDebug.Name = "chkDrawPhysicsDebug";
            this.chkDrawPhysicsDebug.Size = new System.Drawing.Size(125, 17);
            this.chkDrawPhysicsDebug.TabIndex = 0;
            this.chkDrawPhysicsDebug.Text = "Draw Physics Debug";
            this.chkDrawPhysicsDebug.UseVisualStyleBackColor = true;
            this.chkDrawPhysicsDebug.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(366, 9);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(90, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Object Definitions";
            // 
            // lstLayers
            // 
            this.lstLayers.Location = new System.Drawing.Point(214, 25);
            this.lstLayers.Name = "lstLayers";
            this.lstLayers.Size = new System.Drawing.Size(147, 264);
            this.lstLayers.TabIndex = 8;
            // 
            // chkSnapToGrid
            // 
            this.chkSnapToGrid.AutoSize = true;
            this.chkSnapToGrid.Location = new System.Drawing.Point(7, 42);
            this.chkSnapToGrid.Name = "chkSnapToGrid";
            this.chkSnapToGrid.Size = new System.Drawing.Size(89, 17);
            this.chkSnapToGrid.TabIndex = 1;
            this.chkSnapToGrid.Text = "Snap To Grid";
            this.chkSnapToGrid.UseVisualStyleBackColor = true;
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(703, 328);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.lstLayers);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.btnPause);
            this.Controls.Add(this.lblLayers);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lstObjectDefs);
            this.Controls.Add(this.btn_Create);
            this.Controls.Add(this.btn_GetObjects);
            this.Controls.Add(this.treeObjects);
            this.Name = "Main";
            this.Text = "Ninja-Editor";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Main_FormClosed);
            this.Load += new System.EventHandler(this.Main_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer FastTimer;
        private System.Windows.Forms.TreeView treeObjects;
        private System.Windows.Forms.Button btn_GetObjects;
        private System.Windows.Forms.Button btn_Create;
        private System.Windows.Forms.ListBox lstObjectDefs;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label lblLayers;
        private System.Windows.Forms.Button btnPause;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox chkDrawPhysicsDebug;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ListBox lstLayers;
        private System.Windows.Forms.CheckBox chkSnapToGrid;
    }
}

