package com.example.jefferson.goodstracker.Communication;

/**
 * Created by Jefferson on 14/07/2017.
 */

abstract public class DecoderFrame extends Object implements IDecoder {


    protected DecoderFrame() {

    }

    static IDecoder create(DataFrame frame) {

        return create(frame.getTypeFrame());
    }

    static IDecoder create(TypeFrame type){

        IDecoder decoder = null;

        switch (type){

            case OWNER:     decoder = new DecoderFrameOwner();  break;
            case CSV:       decoder = new DecoderFrameOwner();  break;
            case CBOR:      decoder = new DecoderFrameCBOR();   break;
            default:
        }

        return decoder;
    }
}
