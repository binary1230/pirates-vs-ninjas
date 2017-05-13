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
        string lastObjectDefName = "large-block";

        BindingList<KeyValuePair<uint, string>> objectList = new BindingList<KeyValuePair<uint, string>>();
        BindingSource objectListDataSource = null;

        EditorUI editorUI = new EditorUI();

        public Editor()
        {
            InitializeComponent();

            objectListDataSource = new BindingSource(objectList, null);

            RebindData();

            editorUI = new EditorUI();
            editorUI._onObjectsChangedDelegate = new OnObjectsChangedDelegate(OnObjectsChanged);
            editorUI._onSelectionChangedDelegate = new OnSelectionChangedDelegate(OnSelectionChanged);
            editorUI._onSelectedObjectMovedDelegate = new OnSelectedObjectMovedDelegate(OnSelectedObjectMoved);
        }

        public void RebindData()
        {
            lstObjects.DataSource = null;

            lstObjects.DisplayMember = "Value";
            lstObjects.ValueMember = "Key";
            lstObjects.DataSource = objectListDataSource;
        }

        private void OnTick(object sender, EventArgs e)
        {
            gameWrapper.OnTick();

            if (GameWorld.GetInstance() == null)
            {
                Close();
                return;
            }

            if (wasPaused != gameWrapper.Paused)
            {
                OnPauseStatusChanged();
            }
            wasPaused = gameWrapper.Paused;

            chkSnapToGrid.Checked = GameWorld.GetInstance().GetEditor().GetPropSnapToGrid();
            chkDrawPhysicsDebug.Checked = Game.GetInstance().GetPropPhysicsDebugDraw();

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

        public void OnObjectsChanged()
        {
            SyncObjectListWithGame();
            RebindData();
        }

        public void OnSelectionChanged()
        {
            UpdateSelectedObjectFromGame();
        }

        public void OnSelectedObjectMoved()
        {
            PostSelectionChanged(); // just enough to get our propertygrid to refresh
        }

        private void UpdateIfPaused()
        {
            // global::Editor editor = GameWorld.GetInstance().GetEditor();
        }

        private void UpdateSelectedObjectFromGame()
        {
            Object selection = GetSelectedObject();
            if (selection == null) {
                lstObjects.SelectedIndex = -1;
            } else { 
                // 3) update object list with currently selected object
                lstObjects.SelectedValue = selection.GetID();

                // 4) update layer list with layer this object is currently on
                lstLayers.SelectedIndex = lstLayers.FindString(selection.GetLayer().GetName());
            }
        }

        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            FastTimer.Enabled = false;

            gameWrapper.Shutdown();
        }

        private string _map_to_load = "";
        private bool _resave_and_exit = false;

        public void SetMapName(string name, bool resave_and_exit)
        {
            _map_to_load = name;
            _resave_and_exit = resave_and_exit;
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

            GameWorld.GetInstance().GetEditor().SetPropEditorUI(editorUI);

            OnPauseStatusChanged();
            wasPaused = gameWrapper.Paused;

            if (!_resave_and_exit) {
                FastTimer.Enabled = true;
            } else {
                SaveMap();
                Close();
            }   
        }

        public void SaveMap()
        {
            GameWorld.GetInstance().SaveWorldOverCurrentFile();
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
            Object selection = GetSelectedObject();

            lstObjects.BeginUpdate();

            var keysToRemove = new List<KeyValuePair<uint, string>>();

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

                var objectListItemToAdd = new KeyValuePair<uint, string>(
                    obj.GetID(), obj.GetPropObjectDefName()
                );
                objectList.Add(objectListItemToAdd);
            }

            // 3) object list should be ok, now ensure ordering is still ok
            uint selected_id = 0;
            if (selection != null)
                selected_id = selection.GetID();

            int i = 0;
            foreach (Object obj in objects)
            {
                if (obj.GetID() != objectList[i].Key)
                {
                    // ordering is wrong, search for the item in the right spot and swap it here
                    bool found = false;
                    for (int j = i + 1; j < objectList.Count; ++j)
                    {
                        if (objectList[j].Key == obj.GetID())
                        {
                            found = true;

                            var tmp = objectList[i];
                            objectList[i] = objectList[j];
                            objectList[j] = tmp;
                            break;
                        }
                    }
                    Debug.Assert(found);
                }

                i++;
            }

            if (selection != null)
                lstObjects.SelectedValue = selected_id;

            lstObjects.EndUpdate();
        }

        private void btn_GetObjects_Click(object sender, EventArgs e)
        {
            SyncObjectListWithGame();
        }

        private void btn_Create_Click(object sender, EventArgs e)
        {
            if (lstLayers.SelectedItem == null)
            {
                MessageBox.Show("Please select a layer first");
                return;
            }

            if (lstObjectDefs.SelectedItem == null)
            {
                MessageBox.Show("Please select an objectDef first");
                return;
            }

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
            Game.GetInstance().SetPropPhysicsDebugDraw(chkDrawPhysicsDebug.Checked);
        }

        private void chkSnapToGrid_CheckedChanged(object sender, EventArgs e)
        {
            GameWorld.GetInstance().GetEditor().SetPropSnapToGrid(chkSnapToGrid.Checked);
        }

        private void lstObjects_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lstObjects.SelectedValue != null)
            {
                uint object_id = (uint)lstObjects.SelectedValue;
                Object obj = GameWorld.GetInstance().FindObjectByID(object_id);

                if (obj != null)
                {
                    GameWorld.GetInstance().GetEditor().SelectObject(obj);
                }
            }

            PostSelectionChanged();
        }

        private void PostSelectionChanged()
        {
            // handled after anything (game or editor) has changed the selection

            Object selection = DownCast.From(GetSelectedObject());
            objectProperties.SelectedObject = selection;
            objectProperties.ExpandAllGridItems();
        }
    }
}
