using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace HttpServerDemoApp
{
    public class HttpServerListener
    {

        HttpListener httpListener = null;
        bool m_running = false;
        Thread m_thread;
        AutoResetEvent m_event = null;
        string m_serverIp;
        int m_port;
        ConcurrentQueue<Tuple<string, int>> m_queue = new ConcurrentQueue<Tuple<string, int>>();
        public delegate void ServerCallback(string uri, string json);

        ServerCallback m_pServerCallback = null;


        public void CreateListener(AutoResetEvent ev, string ServerIp, int port)
        {
            if (httpListener != null)
                return;
            httpListener = new HttpListener();
            m_event = ev;

            m_serverIp = ServerIp;
            m_port = port;

            string uriPref = string.Format("http://{0}:{1}/", ServerIp, port);
            httpListener.Prefixes.Add(uriPref);
            try
            {
                httpListener.Start();               
            }
            catch (Exception err)
            {

                throw (new SystemException(err.Message));
            }
        }
        public void SetServerCallback(ServerCallback p)
        {
            m_pServerCallback = p;
        }
        
        public void CloseServer()
        {
            Stop();
        }
        public void NotifyIncomingFile()
        {
            m_event.Set();
        }
        public ConcurrentQueue<Tuple<string, int>> GetQueue()
        {
            return m_queue;
        }

        private void ListenerCallback(IAsyncResult result)
        {
            try
            {
                var context = httpListener.EndGetContext(result);

                HttpListenerRequest request = context.Request;
                HttpListenerResponse response = context.Response;

                string uris = string.Format("http://{0}:{1}/api/" , m_serverIp , m_port);

                Console.WriteLine("request.HttpMethod: " + request.HttpMethod);
                Console.WriteLine(request.Url.ToString());

                string text;
                using (var reader = new StreamReader(request.InputStream,
                                                     request.ContentEncoding))
                {
                    text = reader.ReadToEnd();
                    Console.WriteLine(text);
                    if (m_pServerCallback != null)    
                        m_pServerCallback(request.Url.ToString(), text);
                }

                context.Response.StatusCode = 200;
                context.Response.StatusDescription = "OK";
                context.Response.Close();
            }
            catch (Exception err)
            {
               
            }
        }
        public void Start()
        {
            if (m_thread == null || m_thread.IsAlive == false)
            {
                m_thread = new Thread(WaitInComming);
                m_thread.Start();
            }
        }
        void Stop()
        {

            try
            {
                m_running = false;
                httpListener.Close();
            }
            catch (Exception err)
            {

            }
            if (m_thread != null)
                m_thread.Join();

        }
        void WaitInComming()
        {
            m_running = true;

            while (m_running)
            {
                try
                {
                    var result = httpListener.BeginGetContext(ListenerCallback, httpListener);
                    result.AsyncWaitHandle.WaitOne();
                }
                catch (Exception err)
                {
                    Thread.Sleep(10);
                }
            }
        }
    }
}
