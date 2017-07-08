package com.example.jefferson.goodstracker.Communication;

import android.util.Log;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class DecoderFrame {

    class INDEX
    {
        public final int ADDRESS = 0;
        public final int DEST = 1;
        public final int COUNT = 2;
        public final int OPERACAO = 3;
        public final int RESOURCE = 4;
        public final int SIZE_PAYLOAD = 5;

        public final int LAT = 6;
        public final int LNG = 7;

        public final int ACCEL_X = 8;
        public final int ACCEL_Y = 9;
        public final int ACCEL_Z = 10;

        public final int ROT_X = 11;
        public final int ROT_Y = 12;
        public final int ROT_Z = 13;

        public final int SPEED = 14;
        public final int LEVEL = 15;
        public final int TRAVA = 16;
        public final int TIME = 17;
        public final int DATE = 18;
    };

    static public boolean setHeader(Header header)
    {
        boolean ret    = false;

        try
        {
            // Header
            header.append(header.Address.toString("D5")+ final_CHAR.SEPARATOR);
            header.append(header.Dest.toString("D5")+ final_CHAR.SEPARATOR);
            header.append(header.Count.toString("D5") + final_CHAR.SEPARATOR);
            header.append(header.Operation.toString()+ final_CHAR.SEPARATOR);
            header.append(header.Resource);

            ret = true;
        }
        catch (Exception e)
        {
            ret = false;

            System.out.println("Erro na codificacao do Header");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret;
    }

    public boolean setValues(PayLoad payload, DataTelemetria b)
    {
        boolean ret    = false;
        payload     = new PayLoad();

        try
        {
            payload.append(b.Latitude);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.Longitude);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.AxisX.Acceleration.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.AxisY.Acceleration.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.AxisZ.Acceleration.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.AxisX.Rotation.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.AxisY.Rotation.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.AxisZ.Rotation.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.Speed.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.Level.Val);
            payload.append(final_CHAR.SEPARATOR);
            payload.append(b.StatusLock);
            payload.append(final_CHAR.SEPARATOR);

            String str = b.Time.ToLongTimeString();
            str = str.Remove(2, 1);
            str = str.Remove(4, 1);

            payload.append(str);
            payload.append(final_CHAR.SEPARATOR);

            str = b.Date.ToShortDateString().Remove(5, 1);
            str = str.Remove(2, 1);
            payload.append(str);

            ret = true;
        }
        catch (Exception e)
        {
            ret = false;

            System.out.println("Erro na codificacao do frame");
            System.out.println(payload.getData());
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret;
    }

    public boolean getValues(out AnsCmd ans, DataFrame frame)
    {
        boolean ret    = false;
        ans         = new AnsCmd();

        try
        {
            String[] list = frame.Data.Split(CONST_COM.CHAR.SEPARATOR);

            if (list != null && list.length() >= 8)
            {
                // Exclui CheckSum
                frame.Data = frame.Data.SubString(0, frame.Data.length() - 2);
                byte cheksumRx = AsHex(list, list.length()-1);
                ret = frame.checkSum() == cheksumRx;

                if (ret)
                {
                    ans.Header      = decoderHeader(list);

                    if (ans.Header.Resource.equals(RESOURCE.TLM) &&
                            ans.Header.Operation.equals(Operation.AN))
                    {
                        ans.Telemetria = decoderTelemetria(list);
                    }
                }
                else
                {
                    Log.d("","Erro de CheckSum");
                }
            }
            else
            {
                Log.d("","Incorreto a quantidade de parametros recebidos");
            }
        }
        catch (Exception e)
        {
            ret = false;

            System.out.println("Erro na decodificacao do frame");
            System.out.println(frame.Data);
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret;
    }

    private DataTelemetria decoderTelemetria(String[] list)
    {
        DataTelemetria telemetria = new DataTelemetria();

        try
        {
            telemetria.setPosition(AsDouble(list, INDEX.LAT)/100.0,
                    AsDouble(list, INDEX.LNG) / 100.0);

            telemetria.setAcceleration(AsDouble(list, INDEX.ACCEL_X),
                    AsDouble(list, INDEX.ACCEL_Y),
                    AsDouble(list, INDEX.ACCEL_Z));

            telemetria.setRotation(AsDouble(list, INDEX.ROT_X),
                    AsDouble(list, INDEX.ROT_Y),
                    AsDouble(list, INDEX.ROT_Z));

            telemetria.Speed.Val = AsDouble(list, INDEX.SPEED);
            telemetria.Level.Val = AsDouble(list, INDEX.LEVEL);
            telemetria.StatusLock = Asboolean(list, INDEX.TRAVA);
            telemetria.Date      = AsDate(list, INDEX.DATE);
            telemetria.Time     = AsTime(list, INDEX.TIME);
        }
        catch (Exception e)
        {
            telemetria = new DataTelemetria();

            System.out.println("Erro na decodificacao dos dados da Telemetria");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return telemetria;
    }

    private Header decoderHeader(String[] list)
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

            System.out.println("Erro na codificacao do Header");
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return header;
    }

    static private Operation AsOperation(String[] list, int oPERACAO)
    {
        String str = AsString(list, oPERACAO);

        return (Operation)Enum.Parse(typeof(Operation), str);
    }

    static private Date AsTime(String[] list, int index)
    {
        Date d = Convert.ToDate("00:00:00");

        if ((int)index < list.length())
        {
            String str = list[(int)index];

            if (!str.equals(""))
            {
                str = str.Insert(2, ":");
                str = str.Insert(5, ":");
                str = str.SubString(0, str.length() - 4);

                d = Convert.ToDate(str);
            }
        }

        return d;
    }

    static private Date AsDate(String[] list, int index) {

        Date d = Convert.ToDate("01/01/1900");

        if ((int)index < list.length()) {

            String str = list[(int)index];

            if (!str.equals(""))
            {
                str = str.Insert(2, "/");
                str = str.Insert(5, "/");
                d = Convert.ToDate(str);
            }
        }
        return d;
    }

    static private int AsInteger(String[] list, int index) {

        int dest = 0;

        if ((int)index < list.length()) {

            dest = Convert.ToInt16(list[(int)index]);
        }

        return dest;
    }

    static private double AsDouble(String[] list, int index) {

        double dest = 0;

        if ((int)index < list.length()) {

            dest = Convert.ToDouble(list[(int)index].Replace(".", ","));
        }

        return dest;
    }

    static private String AsString(String[] list, int index) {

        String dest = "";

        if ((int)index < list.length()) {

            dest = list[(int)index];
        }

        return dest;
    }
}
