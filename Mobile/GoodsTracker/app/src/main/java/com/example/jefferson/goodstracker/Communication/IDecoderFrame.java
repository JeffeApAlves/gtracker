package com.example.jefferson.goodstracker.Communication;

import com.example.jefferson.goodstracker.Domain.DataTelemetria;

/**
 * Created by Jefferson on 08/07/2017.
 */

public interface IDecoderFrame {

    boolean setValues(PayLoad payload, DataTelemetria b);
    boolean getValues(AnsCmd ans, DataFrame frame);
}
