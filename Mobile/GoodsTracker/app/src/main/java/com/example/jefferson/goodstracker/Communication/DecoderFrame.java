package com.example.jefferson.goodstracker.Communication;

import android.util.Log;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;

import java.util.Date;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class DecoderFrame {

    class INDEX {

        static public final int ADDRESS = 0;
        static public final int DEST = 1;
        static public final int COUNT = 2;
        static public final int OPERACAO = 3;
        static public final int RESOURCE = 4;
        static public final int SIZE_PAYLOAD = 5;

        static public final int LAT = 6;
        static public final int LNG = 7;

        static public final int ACCEL_X = 8;
        static public final int ACCEL_Y = 9;
        static public final int ACCEL_Z = 10;

        static public final int ROT_X = 11;
        static public final int ROT_Y = 12;
        static public final int ROT_Z = 13;

        static public final int SPEED = 14;
        static public final int LEVEL = 15;
        static public final int TRAVA = 16;
        static public final int TIME = 17;
        static public final int DATE = 18;
    };

    static public boolean setHeader(Header header) {

        boolean ret    = false;

        try {

            // Header
            header.append(String.format("%05d",header.getAddress()) + CONST_COM.CHAR.SEPARATOR);
            header.append(String.format("%05d",header.getDest())    + CONST_COM.CHAR.SEPARATOR);
            header.append(String.format("%05d",header.getCount())   + CONST_COM.CHAR.SEPARATOR);
            header.append(header.getOperation().name()              + CONST_COM.CHAR.SEPARATOR);
            header.append(header.getResource());

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

    public boolean setValues(PayLoad payload, DataTelemetria b) {

        boolean ret    = false;
        payload     = new PayLoad();

        try
        {
            payload.append(b.getLatitude());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getLongitude());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.AxisX.Acceleration.Val);
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.AxisY.Acceleration.Val);
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.AxisZ.Acceleration.Val);
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.AxisX.Rotation.Val);
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.AxisY.Rotation.Val);
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.AxisZ.Rotation.Val);
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getValSpeed());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.getValLevel());
            payload.append(CONST_COM.CHAR.SEPARATOR);
            payload.append(b.isStatusLock());
            payload.append(CONST_COM.CHAR.SEPARATOR);

            String str = b.Time.ToLongTimeString();
            str = str.Remove(2, 1);
            str = str.Remove(4, 1);

            payload.append(str);
            payload.append(CONST_COM.CHAR.SEPARATOR);

            str = b.Date.ToShortDateString().Remove(5, 1);
            str = str.Remove(2, 1);
            payload.append(str);

            ret = true;

        } catch (Exception e) {

            ret = false;

            System.out.println("Erro na codificacao do frame");
            System.out.println(payload.getData());
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret;
    }

    public boolean getValues(AnsCmd ans, DataFrame frame) {

        boolean ret     = false;
        ans             = new AnsCmd();

        try
        {
            String[] list = frame.getData().split(String.valueOf(CONST_COM.CHAR.SEPARATOR));

            if (list != null && list.length >= 8)
            {
                //TODO verificar substring
                // Exclui CheckSum
                frame.setData(frame.getData().substring(0, frame.getData().length() - 2));
                byte cheksumRx = AsHex(list, list.length-1);
                ret = frame.checkSum() == cheksumRx;

                if (ret) {

                    Header header = decoderHeader(list);

                    ans.setHeader(header);

                    if (header.getResource().equals(RESOURCE_TYPE.TLM) &&
                        header.getOperation().equals(Operation.AN))
                    {
                        ans.setTelemetria(decoderTelemetria(list));
                    }

                } else {

                    Log.d("","Erro de CheckSum");
                }
            }
            else {

                Log.d("","Incorreto a quantidade de parametros recebidos");
            }
        }
        catch (Exception e)
        {
            ret = false;

            System.out.println("Erro na decodificacao do frame");
            System.out.println(frame.getData());
            System.out.println(e.toString());
            Log.d("",e.toString());
        }

        return ret;
    }

    private DataTelemetria decoderTelemetria(String[] list) {

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

            telemetria.setSpeed(        AsDouble(list, INDEX.SPEED));
            telemetria.setLevel(        AsDouble(list, INDEX.LEVEL));
            telemetria.setStatusLock(   AsBool(list, INDEX.TRAVA));
            telemetria.setDate(         AsDate(list, INDEX.DATE));
            telemetria.setTime (        AsTime(list, INDEX.TIME));
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

    private Header decoderHeader(String[] list) {

        Header header = new Header();

        try
        {
            header.setAddress(AsInteger(list, INDEX.ADDRESS));
            header.setDest(AsInteger(list, INDEX.DEST));
            header.setCount(AsInteger(list, INDEX.COUNT));
            header.setOperation(AsOperation(list, INDEX.OPERACAO));
            header.setResource(AsString(list, INDEX.RESOURCE));
            header.setSizePayLoad(AsInteger(list, INDEX.SIZE_PAYLOAD))s;
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

    static private Operation AsOperation(String[] list, int oPERACAO) {

        String str = AsString(list, oPERACAO);

        return Operation.valueOf(str );
    }

    static private Date AsTime(String[] list, int index) {

        Date d = Convert.ToDate("00:00:00");

        if (index < list.length)
        {
            String str = list[index];

            if (!str.equals("")) {

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

        if (index < list.length) {

            String str = list[index];

            if (!str.equals("")) {

                str = str.Insert(2, "/");
                str = str.Insert(5, "/");
                d = Convert.ToDate(str);
            }
        }
        return d;
    }

    static private int AsInteger(String[] list, int index) {

        int dest = 0;

        if (index < list.length) {

            dest = Convert.ToInt16(list[index]);
        }

        return dest;
    }

    static private double AsDouble(String[] list, int index) {

        double dest = 0;

        if (index < list.length) {

            dest = Convert.ToDouble(list[index].Replace(".", ","));
        }

        return dest;
    }

    static private String AsString(String[] list, int index) {

        String dest = "";

        if (index < list.length) {

            dest = list[index];
        }

        return dest;
    }

    static private byte AsHex(String[] list, int index) {

        byte dest = 0;

        if (index < list.length) {

            dest = (byte)Convert.ToInt16(list[index],16);
        }

        return dest;
    }

    static private boolean AsBool(String[] list, int index) {

        int dest = 0;

        if (index < list.length) {
            
            dest = Convert.ToInt16(list[index]);
        }

        return dest>0 ? true:false;
    }
}
