package com.example.jefferson.goodstracker.Communication;



/**
 * Created by Jefferson on 14/07/2017.
 */

/**
 * http://fasterxml.github.io/jackson-dataformats-binary/javadoc/cbor/2.9.pr1/
 */


public class DecoderFrameCBOR extends DecoderFrame{

    @Override
    public boolean frame_to_cmd(DataFrame frame, Cmd cmd){

        return false;
    }

    @Override
    public boolean ans_to_frame(AnsCmd ans, DataFrame frame) {
        return false;
    }

    @Override
    public boolean frame_to_ans(DataFrame frame, AnsCmd ans) {
        return false;
    }

    @Override
    public boolean cmd_to_frame(Cmd cmd, DataFrame frame) {
        return false;
    }

    @Override
    public boolean str_to_ans(String data, AnsCmd ans) {
        return false;
    }

    @Override
    public String header_to_str(Header header) {
        return null;
    }

    @Override
    public Header str_to_header(String data) {
        return null;
    }
}
