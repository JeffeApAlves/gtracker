﻿using System;
using System.Diagnostics;
using System.Globalization;

namespace GoodsTracker
{
    class DecoderFrame : FrameSerialization
    {
        struct INDEX
        {
            public const int ADDRESS = 0;
            public const int DEST = 1;
            public const int COUNT = 2;
            public const int OPERACAO = 3;
            public const int RESOURCE = 4;
            public const int SIZE_PAYLOAD = 5;

            public const int LAT = 6;
            public const int LNG = 7;

            public const int ACCEL_X = 8;
            public const int ACCEL_Y = 9;
            public const int ACCEL_Z = 10;

            public const int ACCEL_XG = 11;
            public const int ACCEL_YG = 12;
            public const int ACCEL_ZG = 13;

            public const int SPEED = 14;
            public const int LEVEL = 15;
            public const int TRAVA = 16;
            public const int TIME = 17;
            public const int DATE = 18;
        };

        static public bool setHeader(Header header)
        {
            bool ret    = false;

            try
            {
                // Header
                header.Append(header.Address.ToString("D5")+ CONST_CHAR.SEPARATOR);
                header.Append(header.Dest.ToString("D5")+ CONST_CHAR.SEPARATOR);
                header.Append(header.Count.ToString("D5") + CONST_CHAR.SEPARATOR);
                header.Append(header.Operation.ToString()+ CONST_CHAR.SEPARATOR);
                header.Append(header.Resource);

                ret = true;
            }
            catch (Exception e)
            {
                ret = false;

                Console.WriteLine("Erro na codificacao do Header");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return ret;
        }

        public bool encode(out PayLoad payload, Telemetria b)
        {
            bool ret    = false;
            payload     = new PayLoad();

            try
            {
                payload.Append(b.Latitude);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Longitude);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisX.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisY.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisZ.Acceleration.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisX.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisY.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.AxisZ.Rotation.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Speed.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.Level.Val);
                payload.Append(CONST_CHAR.SEPARATOR);
                payload.Append(b.StatusLock);
                payload.Append(CONST_CHAR.SEPARATOR);

                string str = b.Time.ToLongTimeString();
                str = str.Replace(":","");
                str = str.Replace(":","");

                payload.Append(str);
                payload.Append(CONST_CHAR.SEPARATOR);

                str = b.Date.ToShortDateString().Remove(5, 1);
                str = str.Replace("/", "");
                str = str.Replace("/", "");
                payload.Append(str);

                ret = true;
            }
            catch (Exception e)
            {
                ret = false;

                Console.WriteLine("Erro na codificacao do frame");
                Console.WriteLine(payload.Data);
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return ret;
        }

        public bool decode(out AnsCmd ans, DataFrame frame)
        {
            bool ret    = false;
            ans         = new AnsCmd();

            try
            {
                string[] list = frame.Data.Split(CONST_CHAR.SEPARATOR);

                if (list != null && list.Length >= 8)
                {
                    // Exclui CheckSum
                    frame.Data = frame.Data.Substring(0, frame.Data.Length - 2);
                    byte cheksumRx = AsHex(list, list.Length-1);
                    ret = frame.checkSum() == cheksumRx;

                    if (ret)
                    {
                        ans.Header      = decoderHeader(list);

                        if (ans.Header.Resource.Equals(RESOURCE.TLM) && 
                            ans.Header.Operation.Equals(Operation.AN))
                        {
                            ans.Telemetria = decoderTelemetria(list);
                        }
                    }
                    else
                    {
                        Debug.WriteLine("Erro de CheckSum");
                    }
                }
                else
                {
                    Debug.WriteLine("Incorreto a quantidade de parametros recebidos");
                }
            }
            catch (Exception e)
            {
                ret = false;

                Console.WriteLine("Erro na decodificacao do frame");
                Console.WriteLine(frame.Data);
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return ret;
        }

        private Telemetria decoderTelemetria(string[] list)
        {
            Telemetria telemetria = new Telemetria();

            try
            {
                telemetria.setPosition((AsGrauDecimal(list, INDEX.LAT)),
                                        (AsGrauDecimal(list, INDEX.LNG)));

                telemetria.setAcceleration(AsDouble(list, INDEX.ACCEL_X),
                                            AsDouble(list, INDEX.ACCEL_Y),
                                            AsDouble(list, INDEX.ACCEL_Z));
  
                telemetria.setRotation(AsDouble(list, INDEX.ACCEL_XG),
                                        AsDouble(list, INDEX.ACCEL_YG),
                                        AsDouble(list, INDEX.ACCEL_ZG));

                telemetria.Speed.Val = AsDouble(list, INDEX.SPEED);
                telemetria.Level.Val = AsDouble(list, INDEX.LEVEL);
                telemetria.StatusLock = AsBool(list, INDEX.TRAVA);
                telemetria.Date      = AsDate(list, INDEX.DATE);
                telemetria.Time     = AsTime(list, INDEX.TIME);
            }
            catch (Exception e)
            {
                telemetria = new Telemetria();

                Console.WriteLine("Erro na decodificacao dos dados da Telemetria");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return telemetria;
        }
    
        private Header decoderHeader(string[] list)
        {
            Header header = new Header();

            try
            {
                header.Address = AsInteger(list, INDEX.ADDRESS);
                header.Dest = AsInteger(list, INDEX.DEST);
                header.Count = AsInteger(list, INDEX.COUNT);
                header.Operation = AsOperation(list, INDEX.OPERACAO);
                header.Resource = AsString(list, INDEX.RESOURCE);
                header.SizePayLoad = AsInteger(list, INDEX.SIZE_PAYLOAD);
            }
            catch (Exception e)
            {
                header = new Header();

                Console.WriteLine("Erro na codificacao do Header");
                Console.WriteLine(e.ToString());
                Debug.WriteLine(e.ToString());
            }

            return header;
        }

        static private Operation AsOperation(string[] list, int oPERACAO)
        {
            string str = AsString(list, oPERACAO);

            return (Operation)Enum.Parse(typeof(Operation), str);
        }

        static private DateTime AsTime(string[] list, int index)
        {
            CultureInfo culture = new CultureInfo("pt-BR");

            DateTime d = new DateTime();

            if ((int)index < list.Length)
            {
                string str = list[(int)index];

                if (!str.Equals("") && str.Length >= 6)
                {
                    str = str.Insert(2, ":");
                    str = str.Insert(5, ":");
                    str = str.Substring(0, str.Length - 4);

                    d = Convert.ToDateTime(str,culture);
                }
            }

            return d;
        }

        static private DateTime AsDate(string[] list, int index)
        {
            CultureInfo culture = new CultureInfo("pt-BR");

            DateTime d = new DateTime();

            if ((int)index < list.Length)
            {
                string str = list[(int)index];

                if (!str.Equals("") && str.Length>=6)
                {
                    str = str.Insert(2, "/");
                    str = str.Insert(5, "/");
                    d = Convert.ToDateTime(str, culture);
                }                
            }
            
            return d;
        }

        static private int AsInteger(string[] list, int index)
        {
            int dest = 0;

            if ((int)index < list.Length)
            {
                dest = Convert.ToInt16(list[(int)index]);
            }
              
            return dest;
        }

        static private double AsGrauDecimal(string[] list, int index)
        {
            double dest = AsDouble(list, index);

            double minutos = dest % 100;
            double graus = dest - minutos;
            double dec = (minutos / 60.0);


            return Math.Round((graus/100.0)+dec,4);

        }

        static private double AsDouble(string[] list, int index)
        {
            double dest = 0;

            if ((int)index < list.Length)
            {
                //dest = Convert.ToDouble(list[(int)index].Replace(".", ","));
                dest = Convert.ToDouble(list[(int)index]);
            }

            return dest;
        }

        static private string AsString(string[] list, int index)
        {
            string dest = "";

            if ((int)index < list.Length)
            {
                dest = list[(int)index];
            }

            return dest;
        }

        static private byte AsHex(string[] list, int index)
        {
            byte dest = 0;

            if ((int)index < list.Length)
            {
                dest = (byte)Convert.ToInt16(list[(int)index],16);
            }

            return dest;
        }

        static private bool AsBool(string[] list, int index)
        {
            int dest = 0;

            if ((int)index < list.Length)
            {
                dest = (int)Convert.ToInt16(list[(int)index]);
            }

            return dest>0 ? true:false;
        }
    }
}