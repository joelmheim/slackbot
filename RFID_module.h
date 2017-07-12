#ifndef _RFID_
#define _RFID_

#include <MFRC522.h>
////////////////////////////////////////////////////////////////////////

class Rfid_unit{
public:
    Rfid_unit(MFRC522* my_rfid);
    void initializeRFID();
    boolean RFID_scan();
    String uidString;
private:
    MFRC522* rfid_pntr;
    String constructUidString(byte *buffer, byte bufferSize);
};

#endif // _RFID_

