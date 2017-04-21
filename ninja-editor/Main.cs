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
using System.Runtime.InteropServices;

namespace MapEditor
{
    public partial class Main : Form
    {
        GameWrapper gameWrapper = new GameWrapper();
        bool wasPaused = false;

        public Main()
        {
            InitializeComponent();
        }

        private void OnTick(object sender, EventArgs e)
        {
            gameWrapper.OnTick();

            if (wasPaused != gameWrapper.Paused)
            {
                OnPauseStatusChanged();
            }
            wasPaused = gameWrapper.Paused;

            if (gameWrapper.ShouldExit)
            {
                FastTimer.Enabled = false;
                Close();
            }
        }

        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            FastTimer.Enabled = false;

            gameWrapper.Shutdown();
        }

        private void Main_Load(object sender, EventArgs e)
        {
            Console.WriteLine(Directory.GetCurrentDirectory());

            if (!gameWrapper.Init()) {
                MessageBox.Show("Failed to init game, see log for details");
                Close();
                return;
            }
            
            OnPauseStatusChanged();
            wasPaused = gameWrapper.Paused;

            FastTimer.Enabled = true;
        }

        private void LoadGameLists()
        {
            lstObjectDefs.Items.Clear();
            foreach(string objectDefName in gameWrapper.GetObjectDefNames())
            {       
                lstObjectDefs.Items.Add(objectDefName);
            }

            foreach (string layer in gameWrapper.GetLayerNames())
            {
                lstLayers.Items.Add(layer);
            }

            LoadObjectsList();
        }

        private void LoadObjectsList()
        {
            treeObjects.Nodes.Clear();

            GameWorld world = GameWorld.GetInstance();
            ObjectVector objects = world.GetObjects();

            treeObjects.BeginUpdate();

            foreach (Object o in objects)
            {
                string def = o.GetObjectDefName();
                string layername = o.GetLayer().GetName();

                TreeNode layerNode = null;
                foreach (TreeNode n in treeObjects.Nodes)
                {
                    if (n.Text == layername)
                    {
                        layerNode = n;
                        break;
                    }
                }

                if (layerNode == null)
                    layerNode = treeObjects.Nodes.Add(layername);

                layerNode.Nodes.Add(def);
            }

            treeObjects.EndUpdate();
        }

        private void btn_GetObjects_Click(object sender, EventArgs e)
        {
            LoadObjectsList();
        }

        private void btn_Create_Click(object sender, EventArgs e)
        {

        }

        private void btnPaused_Click(object sender, EventArgs e)
        {
            gameWrapper.Paused = !gameWrapper.Paused;
            OnPauseStatusChanged();
        }

        private void OnPauseStatusChanged()
        {
            bool paused = gameWrapper.Paused;

            btnPause.Text = paused ? "Play" : "Pause";
            btn_Create.Enabled = !paused;
            btn_GetObjects.Enabled = !paused;

            lstLayers.Enabled = !paused;
            lstObjectDefs.Enabled = !paused;
            treeObjects.Enabled = !paused;

            if (paused)
            {
                LoadGameLists();
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            GameState.GetInstance().SetPhysicsDebugDraw(checkBox1.Checked);
        }
    }
}
