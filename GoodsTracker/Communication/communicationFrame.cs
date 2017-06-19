using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GoodsTracker
{
    /**
     * 
     * Frame Coomunication
     * [ End. de orig[5] , End dest[5] ,Operacao[2] , Recurso[2] , SizePayload[3] , payload[ 0 ~ 255] , '*'CheckSum[5] ] \r\n
     * 
     * End. de orig: 
     * Range: 00000~65535 (00000) Broadcast
     * 
     * End. de dest: 
     * Range: 00000~65535 (00000) Broadcast
     * 
     * Operacao:
     * Possiveis: RD ou WR
     * 
     * Recurso: 
     * Range: 01~99
     * 01: Lat , Long , AccelX , AccelY , AccelZ , Level, Speed
     * 
     * SizePayload:
     * Range: 0~255
     * 
     * Payload:
     * Conteudo App
     * Observacao: '[' ']' sao caracteres especiais entao usar \] e \[ 
     * 
     * CheckSum 
     * Somatoria
     */

    internal class CommunicationFrame
    {
        public const int LEN_MAX_PAYLOAD = 256;

        protected string dest;
        protected string orig;
        protected string operation;
        protected string resource;
        protected string sizePayload;
        protected string CheckSum;
        private string payLoad;

        public string PayLoad { get => payLoad; set => payLoad = value; }

        internal CommunicationFrame()
        {
            payLoad = "";

            clear();
        }

        internal byte getByte(int i)
        {
            return (byte)payLoad[i];
        }

        internal void setByte(int i, byte b)
        {
            char[] letters  = payLoad.ToCharArray();
            letters[i]      = (char)b;
            payLoad         = string.Join("", letters);
        }

        internal void addByte(byte b)
        {
            payLoad = string.Join(payLoad,(char)b);
        }

        internal void clear()
        {
            payLoad = "";
        }

        internal int getCount()
        {
            return payLoad.Length;
        }

        internal bool isFull()
        {
            return getCount() >= LEN_MAX_PAYLOAD;
        }

        internal bool isEmpty()
        {
            return getCount() <= 0;
        }
    }
}
