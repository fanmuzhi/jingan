using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace MT_TestGUI_Demo
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MT_Analysis_GUI());
        }
    }
}
