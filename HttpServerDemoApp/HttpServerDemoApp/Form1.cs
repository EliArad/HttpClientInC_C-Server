using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HttpServerDemoApp
{
    public partial class Form1 : Form
    {
        HttpServerListener m_server;
        AutoResetEvent m_event = new AutoResetEvent(false);
        public Form1()
        {
            InitializeComponent();
            Control.CheckForIllegalCrossThreadCalls = false;
            string serverIp = Properties.Settings.Default.ServerIp;
            int port = Properties.Settings.Default.port;
            
            textBox2.Text = serverIp.ToString();
            textBox3.Text = port.ToString();

            try
            {
                m_server = new HttpServerListener();

                HttpServerListener.ServerCallback p = new HttpServerListener.ServerCallback(ServerCalls);
                m_server.SetServerCallback(p);
                m_server.CreateListener(m_event, serverIp, port);
                m_server.Start();
            }
            catch (Exception err)
            {
                MessageBox.Show(err.Message);
            } 
        }
        void ServerCalls(string uri , string jsonBody)
        {
            txtIncomingData.AppendText(uri + ": " + jsonBody + Environment.NewLine);

        }
        void LoadSettings()
        {

        }
        void SaveSettings()
        {

        }
    }
}
