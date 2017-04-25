using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
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

        public bool Init()
        {
            GameState.CreateInstance();
            GameState game = GameState.GetInstance();
            if (game == null)
                return false;

            GameOptions.CreateInstance();
            GameOptions options = GameOptions.GetInstance();

            options.SetMapEditorEnabled(true);
            options.SetFirstMode("level_1.xml");

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
}
