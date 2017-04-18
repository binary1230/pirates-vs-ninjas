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
        public Main()
        {
            InitializeComponent();
        }

        private void OnTick(object sender, EventArgs e)
        {
            GameState game = GameState.GetInstance();

            if (!game.ShouldExit())
            {
                game.ProcessEvents();
                game.TickIfNeeded();
            }
        }

        private void Main_FormClosed(object sender, FormClosedEventArgs e)
        {
            FastTimer.Enabled = false;

            GameState game = GameState.GetInstance();

            game.Shutdown();

            GameState.FreeInstance();
        }

        private void Main_Load(object sender, EventArgs e)
        {
            Console.WriteLine(Directory.GetCurrentDirectory());

            GameState.CreateInstance();
            GameState game = GameState.GetInstance();

            if (!game.Init(0, null))
            {
                MessageBox.Show("Failed to init game, see log for details");
                Close();
                return;
            }

            FastTimer.Enabled = true;
        }
    }
}
