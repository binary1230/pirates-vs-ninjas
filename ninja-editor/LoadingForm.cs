using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.Xml.Linq;

namespace MapEditor
{
    public partial class LoadingForm : Form
    {
        Editor editorFrm = null;

        public LoadingForm()
        {
            InitializeComponent();


        }

        public void LoadLevel(string mapname)
        {
            editorFrm = new Editor();

            Hide();

            editorFrm.SetMapName(mapname);
            editorFrm.ShowDialog();

            Show();

            editorFrm = null;
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            LoadLevel(txtFilename.Text);
        }

        private void lstGameFiles_SelectedIndexChanged(object sender, EventArgs e)
        {
            txtFilename.Text = lstGameFiles.SelectedItem.ToString();
        }

        bool PopulateMapData(string path)
        {
            string default_xml = path + "default.xml";
            if (!File.Exists(default_xml))
                return false;

            XDocument doc = XDocument.Load(default_xml);

            string[] excluded = new string[] { "menu.xml", "credits.xml" };
            
            foreach (XElement el in doc.Root.Elements("mode_file"))
            {
                string xmlfile = el.Value.ToString();

                if (Array.IndexOf(excluded, xmlfile) == -1) {
                    lstGameFiles.Items.Add(xmlfile);
                }
            }

            
            foreach (string file in Directory.GetFiles(path)) {
                if (file.Contains("level_"))
                {
                    string name_only = Path.GetFileName(file);

                    if (!lstGameFiles.Items.Contains(name_only))
                        lstGameFiles.Items.Add(name_only);
                }
            }

            return true;
        }

        void PopulateAllMapData()
        {
            bool loaded = false;

            string[] paths = new string[] {
                "data/",
                "../../data/",
            };

            foreach (string path in paths)
            {
                loaded = PopulateMapData(path);
            }

            if (!loaded)
            {
                MessageBox.Show("Couldn't load default.xml (is this exe next to the 'data/' dir?)");
            }
        }

        private void LoadingForm_Load(object sender, EventArgs e)
        {
            PopulateAllMapData();
        }
    }
}
