using System;
using System.Diagnostics;
using System.Text;
using static GoodsTracker.SerialSerialization;

namespace GoodsTracker
{
    enum UNIT_FIELD{
        GRAU_DECIMAL,
        GRAU_SEXAGESIMAL,
    }
 
    internal class FieldFrame<T> : Object
    {
        UNIT_FIELD  unit;
        string[]    list;
        int         index;
        string      strField = "";

        internal FieldFrame(INDEX i, string[] l,UNIT_FIELD u)
        {
            unit    = u;
            setValueField(l, i);
        }

        internal FieldFrame(INDEX i, string[] l)
        {
            unit    = UNIT_FIELD.GRAU_DECIMAL;
            setValueField(l, i);
        }

        void setValueField(string[] l,INDEX i)
        {
            list    = l;
            index   = (int)i;

            if (list == null)
            {
                throw new ArgumentException("Parameter cannot be null", "list");
            }else if(index >= list.Length)
            {
                StringBuilder sb = new StringBuilder();

                sb.Append("Lista nâo contem o dado a ser convertido");
                sb.Append(" index = " + index);
                sb.Append(" length =  " + list.Length);

                throw new ArgumentException(sb.ToString(), "list");
            }
            else
            {
                strField = list[index];
            }
        }

        internal T getVal()
        {
            Object v = null;

            if (index < list.Length)
            {
                try
                {
                    //TypeCode tipo = Type.GetTypeCode(val.GetType());
                    Type tipo = typeof(T);

                    //Debug.WriteLine(" Conversão da string '{0}' para {1}", strField, tipo.ToString());

                    if (tipo == typeof(String))
                    {
                        v = AsString();

                    }
                    else if (tipo == typeof(RESOURCE))
                    {
                        v = AsOperation();

                    }
                    else if (tipo == typeof(int) || tipo == typeof(Int16) || tipo == typeof(Int32))
                    {
                        v = AsInteger();

                    }
                    else if (tipo == typeof(uint) || tipo == typeof(UInt16) || tipo == typeof(UInt32))
                    {
                        v = AsUInteger();

                    }
                    else if (tipo == typeof(Double))
                    {
                        if (UNIT_FIELD.GRAU_DECIMAL.Equals(unit))
                        {
                            v = AsDouble();
                        }
                        else
                        {
                            v = AsGrauDecimal();
                        }
                    }
                    else if (tipo == typeof(bool))
                    {
                        v = AsBool();

                    }
                    else if (tipo == typeof(DateTime))
                    {
                        v = AsDateTime();

                    }
                    else if (tipo == typeof(Operation))
                    {
                        v = AsOperation();
                    }
                    else if (tipo == typeof(byte))
                    {
                        v = AsHex();
                    }
                }
                catch (Exception e)
                {
                    v = null;

                    Console.WriteLine("Erro na decodificacao do frame");
                    Console.WriteLine(e.ToString());
                    Debug.WriteLine(e.ToString());
                }
            }
            return (T)v;
        }

        private Operation AsOperation()
        {
            return (Operation)Enum.Parse(typeof(Operation), AsString());
        }

        private DateTime AsDateTime()
        {
//            CultureInfo culture = new CultureInfo("pt-BR");

            DateTime dt = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            dt = dt.AddSeconds(AsDouble()).ToLocalTime();

            return dt;
        }

        private Int32 AsInteger()
        {
            return Convert.ToInt32(strField);
        }

        private UInt32 AsUInteger()
        {
            return Convert.ToUInt32(strField);
        }

        private double AsGrauDecimal()
        {
            double dest     = AsDouble();
            double minutos  = dest % 100;
            double graus    = dest - minutos;
            double dec      = (minutos / 60.0);

            return Math.Round((graus / 100.0) + dec, 4);
        }

        private double AsDouble()
        {
            return Convert.ToDouble(strField);
        }

        private string AsString()
        {
            return strField;
        }

        private byte AsHex()
        {
            return (byte)Convert.ToInt16(strField, 16);
        }

        private bool AsBool()
        {
             return AsInteger() != 0 ? true : false;
        }
    }
}
