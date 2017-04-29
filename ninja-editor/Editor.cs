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
using System.Reflection;
using System.Diagnostics;

namespace MapEditor
{
    public partial class Editor : Form
    {
        GameWrapper gameWrapper = new GameWrapper();
        bool wasPaused = false;

        string lastLayerName = "foreground";
        string lastObjectDefName = "greenblock";

        // Dictionary<uint, string> objectList = new Dictionary<uint, string>();
        BindingList<KeyValuePair<uint, string>> objectList = new BindingList<KeyValuePair<uint, string>>();
        BindingSource objectListDataSource = null;

        public Editor()
        {
            InitializeComponent();

            lstObjects.DisplayMember = "Value";
            lstObjects.ValueMember = "Key";

            objectListDataSource = new BindingSource(objectList, null);

            RebindData();
        }

        public void RebindData()
        {
            lstObjects.DataSource = null;
            lstObjects.DataSource = objectListDataSource;
        }

        private void OnTick(object sender, EventArgs e)
        {
            gameWrapper.OnTick();

            if (wasPaused != gameWrapper.Paused)
            {
                OnPauseStatusChanged();
            }
            wasPaused = gameWrapper.Paused;

            chkSnapToGrid.Checked = GameWorld.GetInstance().GetEditor().GetPropSnapToGrid();
            chkDrawPhysicsDebug.Checked = GameState.GetInstance().GetPropPhysicsDebugDraw();

            if (gameWrapper.Paused)
            {
                UpdateIfPaused();
            }

            if (gameWrapper.ShouldExit)
            {
                FastTimer.Enabled = false;
                Close();
            }
        }

        private Object GetSelectedObject()
        {
            return GameWorld.GetInstance().GetEditor().GetPropSelection();
        }
        
        private void UpdateIfPaused()
        {
            if (GameWorld.GetInstance().GetEditor().GetPropObjectsChanged())
            {
                RebindData();
                SyncObjectListWithGame();
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
        }

        private void LoadStaticGameLists()
        {
            lstObjectDefs.Items.Clear();
            foreach (string objectDefName in gameWrapper.GetObjectDefNames())
            {
                lstObjectDefs.Items.Add(objectDefName);
            }

            lstLayers.Items.Clear();
            foreach (string layer in gameWrapper.GetLayerNames())
            {
                lstLayers.Items.Add(layer);
            }
        }

        private void SyncObjectListWithGame()
        {
            GameWorld world = GameWorld.GetInstance();
            ObjectVector objects = world.GetObjects();

            lstObjects.BeginUpdate();

            List<KeyValuePair<uint, string>> keysToRemove = new List<KeyValuePair<uint, string>>();

            // 1) remove anything that's no longer present
            foreach (KeyValuePair<uint, string> kvp in objectList)
            {
                bool found = false;

                foreach (Object obj in objects)
                {
                    if (kvp.Key == obj.GetID())
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                    continue;

                keysToRemove.Add(kvp);
            }

            foreach (KeyValuePair<uint, string> key in keysToRemove)
            {
                objectList.Remove(key);
            }

            // 2) add anything that needs to be added
            foreach (Object obj in objects)
            {
                bool found = false;
                foreach (KeyValuePair<uint,string> kvp in objectList)
                {
                    if (kvp.Key == obj.GetID())
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                    continue;

                objectList.Add(new KeyValuePair < uint, string > (obj.GetID(), obj.GetObjectDefName()));
            }

            lstObjects.EndUpdate();

            Object selection = GetSelectedObject();
            if (selection != null)
            {
                // 3) update object list with currently selected object
                lstObjects.SelectedValue = selection.GetID();

                // 4) update layer list with layer this object is currently on
                lstLayers.SelectedIndex = lstLayers.FindString(selection.GetLayer().GetName());
            }
        }

        private void btn_GetObjects_Click(object sender, EventArgs e)
        {
            SyncObjectListWithGame();
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
            lstObjects.Enabled = paused;

            if (paused)
            {
                LoadStaticGameLists();
                SyncObjectListWithGame();

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
            GameState.GetInstance().SetPropPhysicsDebugDraw(chkDrawPhysicsDebug.Checked);
        }

        private void chkSnapToGrid_CheckedChanged(object sender, EventArgs e)
        {
            GameWorld.GetInstance().GetEditor().SetPropSnapToGrid(chkSnapToGrid.Checked);
        }

        private void treeObjects_AfterSelect(object sender, TreeViewEventArgs e)
        {
            // TreeNode selected = treeObjects.SelectedNode;
            Object obj = null;
            try
            {
                KeyValuePair<uint,string> kvp = (KeyValuePair < uint, string > )lstObjects.SelectedValue;
                obj = GameWorld.GetInstance().FindObjectByID(kvp.Key);
            }
            catch (System.FormatException) { }

            UpdatePropertiesUIFromObject(obj);
        }

        private void UpdatePropertiesUIFromObject(Object obj)
        {
            if (obj == null)
            {
                return;
            }

            objectProperties.SelectedObject = obj;

            System.Type objectType = obj.GetType();
            foreach (MethodInfo methodInfo in objectType.GetMethods(BindingFlags.Public | BindingFlags.Instance))
            {
                if (methodInfo.Name.StartsWith("GetProp"))
                {
                    
                }
            }
        }
    }
}
