//
// Created by RV Administrator on 18.06.2015.
//

#include "defs.h"

#ifdef WIN32
#include "windows.h"
#else
#include "sys/time.h"
#endif

Coordinate NOMOVE = 170;
int ISMATE = 10081;
int SIZE_TRANS_MB = 250;
int MAXMOVES = 81;
int INFINITY = 25565;
int BOARDSCORE = 100;

int GetTimeMs() {
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec*1000 + t.tv_usec/1000;
#endif
}

std::string PRMOVE(Coordinate m) {
    int boardBig = (m & 0xF0) >> 4;
    int boardSmall = m & 0xF;
    std::string returnMove= {'B', (char) ('1' + boardBig), 'F', (char) ('1' + boardSmall), ' '};
    return returnMove;
};