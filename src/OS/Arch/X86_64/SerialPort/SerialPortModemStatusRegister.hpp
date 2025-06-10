#ifdef __x86_64__
#ifndef SerialPortModemStatusRegister_H
#define SerialPortModemStatusRegister_H

struct SerialPortModemStatusRegister {
    bool clearToSendChanged : 1;
    bool dataSetReadyChanged : 1;
    bool trailingEdgeOfRingIndicator : 1;
    bool dataCarrierDetectChanged : 1;
    bool clearToSend : 1;
    bool dataSetReady : 1;
    bool ringIndicator : 1;
    bool dataCarrierDetect : 1;
} __attribute__((packed));

#endif
#endif