package ru.servers.game.network.protocol.fromserver;

import lombok.EqualsAndHashCode;
import lombok.ToString;
import ru.servers.game.common.Primitives;
import ru.servers.game.network.protocol.NetworkPacket;

@ToString(callSuper = true)
@EqualsAndHashCode(callSuper = true)
public final class InitializePositionAnswerPacket extends NetworkPacket implements PacketFromServer{

    // Byte protocol:
    // [0] - protocol type
    // [1...4] - packet number
    // [5] - result initialization of position

    private final static byte POSITION_RESULT_INITIALIZATION = 5; // index
    private final static byte SIZE_RESULT_INITIALIZATION = Primitives.BYTE_SIZE; // bytes

    public final static byte SIZE_PACKET = SIZE_NETWORK_PACKET + SIZE_RESULT_INITIALIZATION ;

    public InitializePositionAnswerPacket(byte[] bufferFromRequest) {
        super(bufferFromRequest);
        if (bufferFromRequest.length < SIZE_PACKET){
            throw new IllegalArgumentException("Incorrect size of the initialize position answer packet.");
        }
    }

    public void setResultInitialization(boolean resultInitialization){
        buffer[POSITION_RESULT_INITIALIZATION] = (byte) (resultInitialization ? 1 : 0);
    }

    public boolean getResultInitialization(){
        return buffer[POSITION_RESULT_INITIALIZATION] != 0;
    }

    @Override
    public byte[] getBuffer() {
        return buffer;
    }

}