package com.example.jefferson.goodstracker.Communication;

import java.io.IOException;

/**
 * Created by Jefferson on 14/07/2017.
 */

public interface IDecoder {

    boolean frame_to_chat(DataFrame frame,ChatMessage chat);
    boolean frame_to_cmd(DataFrame frame,Cmd cmd);
    boolean ans_to_frame(AnsCmd ans, DataFrame frame);
    boolean frame_to_ans(DataFrame frame, AnsCmd ans);
    boolean cmd_to_frame(Cmd cmd, DataFrame frame);
    boolean str_to_ans(String data, AnsCmd ans);
    
    String header_to_str(Header header);
    Header str_to_header(String data);

    boolean chat_to_frame(ChatMessage chatMessage, DataFrame frame);
}
