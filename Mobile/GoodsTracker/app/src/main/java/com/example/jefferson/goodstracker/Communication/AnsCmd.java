package com.example.jefferson.goodstracker.Communication;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;

/**
 * Created by Jefferson on 08/07/2017.
 */

public class AnsCmd {

    Header          header;
    DataTelemetria telemetria;

    public AnsCmd() {

        header      = new Header();
        telemetria  = null;
    }

    public AnsCmd(String r,Operation o) {

        telemetria  = null;
        header      = new Header();
        header.setResource(r);
        header.setOperation(o);

    }

    public Header getHeader() {
        return header;
    }

    public void setHeader(Header header) {
        this.header = header;
    }

    public DataTelemetria getTelemetria() {
        return telemetria;
    }

    public void setTelemetria(DataTelemetria telemetria) {
        this.telemetria = telemetria;
    }
}
