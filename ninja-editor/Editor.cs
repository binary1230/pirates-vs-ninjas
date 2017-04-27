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
    public partial class Editor : Form
    {
        GameWrapper gameWrapper = new GameWrapper();
        bool wasPaused = false;

        string lastLayerName = "foreground";
        string lastObjectDefName = "greenblock";

        public Editor()
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

            chkSnapToGrid.Checked = GameWorld.GetInstance().GetEditor().GetSnapToGrid();
            chkDrawPhysicsDebug.Checked = GameState.GetInstance().GetPhysicsDebugDraw();

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

        private string _map_to_load = "";

        public void SetMapName(string name)
        {
            _map_to_load = name;
        }

        private void Main_Load(object sender, EventArgs e)
        {
            Init(_map_to_load);
        }

        public void Init(string mapname) { 
            Console.WriteLine(Directory.GetCurrentDirectory());

            if (!gameWrapper.Init(mapname)) {
                MessageBox.Show("Failed to init game, see log for details");
                Close();
                return;
            }
            
            OnPauseStatusChanged();
            wasPaused = gameWrapper.Paused;

            FastTimer.Enabled = true;

            // hack: force editor window to the front
            this.WindowState = FormWindowState.Minimized;
            this.Show();
            this.WindowState = FormWindowState.Normal;
        }

        private void LoadGameLists()
        {
            lstObjectDefs.Items.Clear();
            foreach(string objectDefName in gameWrapper.GetObjectDefNames())
            {       
                lstObjectDefs.Items.Add(objectDefName);
            }

            lstLayers.Items.Clear();
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
            string currentLayer = lstLayers.SelectedItem.ToString();
            string currentObjDef = lstObjectDefs.SelectedItem.ToString();

            global::Editor editor = GameWorld.GetInstance().GetEditor();
            editor.CreateAndSelectObject(currentObjDef, currentLayer);
            editor.FlashText("creating objects");
        }

        private void btnPaused_Click(object sender, EventArgs e)
        {
            gameWrapper.Paused = !gameWrapper.Paused;
            OnPauseStatusChanged();
        }

        private void OnPauseStatusChanged()
        {
            bool paused = wasPaused = gameWrapper.Paused;

            btnPause.Text = paused ? "Play" : "Pause";
            btn_Create.Enabled = paused;
            btn_GetObjects.Enabled = paused;

            lstLayers.Enabled = paused;
            lstObjectDefs.Enabled = paused;
            treeObjects.Enabled = paused;

            if (paused)
            {
                LoadGameLists();

                lstLayers.SelectedIndex = lstLayers.FindString(lastLayerName);
                lstObjectDefs.SelectedIndex = lstObjectDefs.FindString(lastObjectDefName);
            }
            else
            {
                if (lstLayers.SelectedItem != null)
                    lastLayerName = lstLayers.SelectedItem.ToString();

                if (lstObjectDefs.SelectedItem != null)
                    lastObjectDefName = lstObjectDefs.SelectedItem.ToString();
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            GameState.GetInstance().SetPhysicsDebugDraw(chkDrawPhysicsDebug.Checked);
        }

        private void chkSnapToGrid_CheckedChanged(object sender, EventArgs e)
        {
            GameWorld.GetInstance().GetEditor().SetSnapToGrid(chkSnapToGrid.Checked);
        }

        private void treeObjects_AfterSelect(object sender, TreeViewEventArgs e)
        {
            
        }
    }
}
