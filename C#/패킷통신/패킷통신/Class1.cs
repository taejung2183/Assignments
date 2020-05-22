﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace 패킷통신
{
    public enum PacketType {
        초기화=0,
        로그인
    }
    public enum PacketSendERROR {
        정상=0,
        에러
    }
    [Serializable]
    public class Packet{//패킷 정의
        public int Length;
        public int Type;

        public Packet() {
            this.Length=0;
            this.Type=0;
        }
        public static byte[] Serialize(Object o) {
            MemoryStream ms=new MemoryStream(1024*4);
            BinaryFormatter bf=new BinaryFormatter();
            bf.Serialize(ms,0);
            return ms.ToArray();
        }

        public static Object Desserialize(byte[] bt) {
            MemoryStream ms=new MemoryStream(1024*4);
            foreach (byte b in bt) {
                ms.WriteByte(b);
            }
            ms.Position=0;
            BinaryFormatter bf=new BinaryFormatter();
            object obj=bf.Deserialize(ms);
            ms.Close();
            return obj;
        }
    }
    [Serializable]
    public class Initialize : Packet {
        public int Data=0;
    }

    [Serializable]
    public class Login : Packet {
        public string m_strID;
        public Login() {
            this.m_strID=null;
        }
    }

}
