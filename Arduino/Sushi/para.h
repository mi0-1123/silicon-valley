// ============================================
//  PAYLOAD
// ============================================
struct payloadStructure { // COM communication packet
    int8_t  CC; // (dice)
    int16_t PP; // (gravity)
    int16_t XX;
    int16_t YY;
    int16_t ZZ;
} __attribute__((packed));
payloadStructure payloadd;
struct payloadStructure2 { // COM communication packet
    int8_t  CC; // (dice)
    int16_t PP; // (gravity)
    int16_t XX;
    int16_t YY;
    int16_t ZZ;
} __attribute__((packed));
payloadStructure payloadd2;

struct paydvalue { // COM communication packet
    int  ID;
    bool shake;
    bool stick;
    int  dist;
    int16_t z,y;
} __attribute__((packed));
paydvalue payd1={-1, false, false, 0};

struct paydvalue2 { // COM communication packet
    int  ID;
    bool shake;
    bool stick;
    int  dist;
    int16_t z,y;
} __attribute__((packed));
paydvalue2 payd2={-1, false, false, 0};

int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
int16_t bx, by, bz, hx, hy, hz;

struct  sendMsg {
    String message;
} __attribute__((packed));
sendMsg sendmsg;

//int ggp = 0;
//byte ast;        // dice
//int16_t gst = 0; // gravity
byte recvflg = 0;         // flag for receive data dump
// ============================================
// GESTURE
// ============================================
/* Color */   int ambD, redD, greenD, blueD; // ambient, Red, Green, Blue
/* Distance*/ int distance  = 0;             // prximity sensor data
/* Gesture*/  int Gesture   = 0;             // Gesture Pattern
int GestureGo = 0;             // Gesture Pattern
int stayC = 0;      // long stay timer
int k;              // Gesture
int Oshot = 0;      // One shot timer
int Gmask = 0;      // set:GestureGo reset distance < 50
