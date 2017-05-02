using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;

[TypeConverter(typeof(ExpandableObjectConverter))]
public partial class b2Vec2 {}

public partial class Object {}

namespace MapEditor
{
    public delegate void OnObjectsChangedDelegate();
    public delegate void OnSelectionChangedDelegate();
    public delegate void OnSelectedObjectMovedDelegate();

    public class EditorUI : EditorBaseUI
    {
        public OnObjectsChangedDelegate _onObjectsChangedDelegate;
        public OnSelectionChangedDelegate _onSelectionChangedDelegate;
        public OnSelectedObjectMovedDelegate _onSelectedObjectMovedDelegate;

        public override void OnObjectsChanged()
        {
            _onObjectsChangedDelegate();
        }

        public override void OnSelectionChanged()
        {
            _onSelectionChangedDelegate();
        }

        public override void OnSelectedObjectMoved()
        {
            _onSelectedObjectMovedDelegate();
        }
    }

    class GameWrapper
    {
        private bool _should_exit = false;
        public bool ShouldExit {
            get { return _should_exit; }
        }

        public bool Paused {
            get { return GameState.GetInstance() != null && GameState.GetInstance().IsPaused(); }
            set { GameState.GetInstance().SetPaused(value); }
        }

        public bool Init(string mapname)
        {
            GameState.CreateInstance();
            GameState game = GameState.GetInstance();
            if (game == null)
                return false;

            GameOptions.CreateInstance();
            GameOptions options = GameOptions.GetInstance();

            options.SetPropMapEditorEnabled(true);
            options.SetPropFirstMode(mapname);

            if (!game.Init(0, null))
                return false;

            game.SetPaused(true);
            Paused = true;

            return true;
        }

        public void OnTick()
        {
            GameState game = GameState.GetInstance();

            _should_exit = game.ShouldExit();

            if (_should_exit) {
                return;
            }

            game.ProcessEvents();
            game.TickIfNeeded();
        }

        public void Shutdown()
        {
            GameState game = GameState.GetInstance();
            game.Shutdown();

            GameState.FreeInstance();
        }

        public List<String> GetObjectDefNames()
        {
            ObjectFactory objectFactory = ObjectFactory.GetInstance();
            List<String> objectDefNames = new List<String>();

            int count = objectFactory.GetObjectDefinitionCount();
            for (int i = 0; i < count; i++)
            {
                string objectDefName = objectFactory.GetObjectDefinition(i);
                objectDefNames.Add(objectDefName);
            }

            return objectDefNames;
        }

        public List<String> GetLayerNames()
        {
            GameWorld world = GameWorld.GetInstance();
            List<String> layerNames = new List<String>();

            foreach (ObjectLayer layer in world.GetLayers())
            {
                layerNames.Add(layer.GetName());
            }

            return layerNames;
        }
    }

    public class DownCast
    {
        public static Object From(Object obj)
        {
            Object downcast = null;

            // This is weird, but needed.  Our C++ -> C# bindings need to call a function which returns the correct
            // derived type.  We cast it explicitly to the derived type, but also once we've done that, we can keep it as an Object and
            // C# will have the info needed.  This is useful for a bunch of reasons including the property editor can edit Derived class
            // property.
            downcast = ObjectSpring.DynamicCastFrom(obj); if (downcast != null) return downcast;
            downcast = ObjectPlayer.DynamicCastFrom(obj); if (downcast != null) return downcast;

            // if can't figure out anything else, return the original
            return obj;
        }
    }
}
