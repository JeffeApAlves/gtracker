using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace GoodsTracker
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
            try
            {
                Debug.Listeners.Add(new TextWriterTraceListener("debug_trace.txt"));
                Debug.AutoFlush = true;

                Application.Run(new MainForm());
            }
            catch (Exception e)
            {
                Debug.WriteLine("Erro na aplicao");
                Debug.WriteLine(e.ToString());
            }            
        }
    }
}
