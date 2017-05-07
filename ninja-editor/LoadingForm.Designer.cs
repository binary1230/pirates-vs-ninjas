namespace MapEditor
{
    partial class LoadingForm
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
            this.btnLoad = new System.Windows.Forms.Button();
            this.txtFilename = new System.Windows.Forms.TextBox();
            this.lstGameFiles = new System.Windows.Forms.ListBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnResaveAll = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnLoad
            // 
            this.btnLoad.Location = new System.Drawing.Point(398, 226);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(75, 23);
            this.btnLoad.TabIndex = 0;
            this.btnLoad.Text = "Edit Map";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // txtFilename
            // 
            this.txtFilename.Location = new System.Drawing.Point(12, 226);
            this.txtFilename.Name = "txtFilename";
            this.txtFilename.Size = new System.Drawing.Size(380, 20);
            this.txtFilename.TabIndex = 1;
            this.txtFilename.Text = "level_0.xml";
            this.txtFilename.TextChanged += new System.EventHandler(this.txtFilename_TextChanged);
            // 
            // lstGameFiles
            // 
            this.lstGameFiles.FormattingEnabled = true;
            this.lstGameFiles.Location = new System.Drawing.Point(13, 39);
            this.lstGameFiles.Name = "lstGameFiles";
            this.lstGameFiles.Size = new System.Drawing.Size(460, 147);
            this.lstGameFiles.TabIndex = 2;
            this.lstGameFiles.SelectedIndexChanged += new System.EventHandler(this.lstGameFiles_SelectedIndexChanged);
            this.lstGameFiles.DoubleClick += new System.EventHandler(this.lstGameFiles_DoubleClick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Maps in game";
            // 
            // btnResaveAll
            // 
            this.btnResaveAll.Location = new System.Drawing.Point(15, 192);
            this.btnResaveAll.Name = "btnResaveAll";
            this.btnResaveAll.Size = new System.Drawing.Size(124, 23);
            this.btnResaveAll.TabIndex = 4;
            this.btnResaveAll.Text = "Resave All Maps";
            this.btnResaveAll.UseVisualStyleBackColor = true;
            this.btnResaveAll.Click += new System.EventHandler(this.btnResaveAll_Click);
            // 
            // LoadingForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(485, 261);
            this.Controls.Add(this.btnResaveAll);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lstGameFiles);
            this.Controls.Add(this.txtFilename);
            this.Controls.Add(this.btnLoad);
            this.MaximizeBox = false;
            this.Name = "LoadingForm";
            this.Text = "Pirates VS Ninjas - Map Editor";
            this.Load += new System.EventHandler(this.LoadingForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnLoad;
        private System.Windows.Forms.TextBox txtFilename;
        private System.Windows.Forms.ListBox lstGameFiles;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnResaveAll;
    }
}