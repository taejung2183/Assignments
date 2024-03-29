﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using 패킷통신;
using System.Net.Sockets;
using System.Threading;

namespace 패킷서버 {
    public partial class Form1 : Form {
        private NetworkStream m_networkstream;
        private TcpListener m_listener;

        private byte[] sendBuffer=new byte[1024*4];
        private byte[] readBuffer=new byte[1024*4];

        private bool m_bClientOn=false;

        private Thread m_thread;

        public Initialize m_initializeClass;
        public Login m_loginClass;


        public Form1() {
            InitializeComponent();
        }
        public void RUN() {
            this.m_listener=new TcpListener(7777);
            this.m_listener.Start();

            if (!this.m_bClientOn) {
                this.Invoke(new MethodInvoker(delegate () {
                    this.txt_server_state.AppendText("클라이언트 접속 대기중\n");
                }));
            }

            TcpClient client=this.m_listener.AcceptTcpClient();

            if (client.Connected) {
                this.m_bClientOn=true;
                this.Invoke(new MethodInvoker(delegate () {
                    this.txt_server_state.AppendText("클라이언트 접속\n");
                }));
                m_networkstream=client.GetStream();
            }
            int nRead=0;
            while (this.m_bClientOn) {
                try {
                    nRead=0;
                    nRead=this.m_networkstream.Read(readBuffer,0,1024*4);
                } catch {
                    this.m_bClientOn=false;
                    this.m_networkstream=null;
                }
                Packet packet=(Packet)Packet.Desserialize(this.readBuffer);

                switch ((int)packet.Type) {
                    case (int)PacketType.초기화: {
                            this.m_initializeClass=(Initialize)Packet.Desserialize(this.readBuffer);
                            this.Invoke(new MethodInvoker(delegate () {
                                this.txt_server_state.AppendText("패킷 전송 성공. "+
                                    "Initialize Class Data is"+this.m_initializeClass.Data+"\n");
                                  
                            }));
                            break;                             
                        }
                    case (int)PacketType.로그인: {
                            this.m_loginClass=(Login)Packet.Desserialize(this.readBuffer);
                            this.Invoke(new MethodInvoker(delegate () {
                                this.txt_server_state.AppendText("패킷 전송 성공. Login Class Data is"+
                                    this.m_loginClass.m_strID+"\n");
                            }));
                            break;
                        }

                }
            }
        }

        private void txt_server_state_TextChanged(object sender, EventArgs e) {

        }

        private void Form1_Load(object sender, EventArgs e) {
            this.m_thread=new Thread(new ThreadStart(RUN));
            this.m_thread.Start();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e) {
            this.m_listener.Stop();
            this.m_networkstream.Close();
            this.m_thread.Abort();
        }
    }
}
