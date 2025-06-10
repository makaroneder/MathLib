#ifdef __x86_64__
#ifndef SerialPortLineStatusRegister_H
#define SerialPortLineStatusRegister_H

struct SerialPortLineStatusRegister {
    bool dataReady : 1;
    bool overrunError : 1;
    bool parityError : 1;
    bool framingError : 1;
    bool breakIndicator : 1;
    bool transmitterEmpty : 1;
    bool transmitterIdle : 1;
    bool impendingError : 1;
} __attribute__((packed));

#endif
#endif