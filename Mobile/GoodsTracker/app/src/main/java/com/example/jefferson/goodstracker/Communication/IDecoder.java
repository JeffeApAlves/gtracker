package com.example.jefferson.goodstracker.Communication;

import java.io.IOException;

/**
 * Created by Jefferson on 14/07/2017.
 */

public interface IDecoder {

    public boolean frame_to_cmd(DataFrame frame,Cmd cmd);
    public boolean ans_to_frame(AnsCmd ans, DataFrame frame);
    public boolean frame_to_ans(DataFrame frame, AnsCmd ans);
    public boolean cmd_to_frame(Cmd cmd, DataFrame frame);
    public boolean str_to_ans(String data, AnsCmd ans);
    
    public String header_to_str(Header header);
    public Header str_to_header(String data);
}
