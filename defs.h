//
// Created by RV Administrator on 16.06.2015.
//

#ifndef TIDESTICTACTICS_DEFS_H
#define TIDESTICTACTICS_DEFS_H

#include <stdint-gcc.h>
#include <stdio.h>
#include <iostream>


typedef uint16_t MiniBitboard;
typedef uint32_t HistoryMove;
typedef uint8_t Coordinate;
typedef uint64_t PosKey;

enum Color {
    COLOR_X,
    COLOR_O,
    COLOR_BOTH,
    COLOR_NONE,
    COLOR_NUMBER = 2
};

enum Square {
    SQUARE_0,
    SQUARE_1,
    SQUARE_2,
    SQUARE_3,
    SQUARE_4,
    SQUARE_5,
    SQUARE_6,
    SQUARE_7,
    SQUARE_8,
    SQUARE_NONE,
    SQUARE_NUMBER = 9
};

enum HashFlag {
    HF_NONE,
    HF_ALPHA,
    HF_BETA,
    HF_EXACT
};

struct S_MOVE {
    Coordinate move;
    double score;
};

struct Movelist {
    S_MOVE moves [80];
    int count = 0;
};

struct HashEntry {
    PosKey poskey;
    Coordinate move;
    double score;
    int depth;
    HashFlag flags;
};

typedef struct {				// the info about the latest search

    int starttime;				// for output on the GUI
    int stoptime;
    int depth;
    bool timeset;
    int movestogo;

    long nodes;

    bool quit;					// ends the search
    bool stopped;				// pauses the search

    float fh;					// fail high
    float fhf;					// fail high first

    bool POST_THINKING;			// post the search trees to the console

} SEARCHINFO;

extern int GetTimeMs();
extern std::string PRMOVE(Coordinate);

extern Coordinate NOMOVE;
extern double ISMATE;
extern int SIZE_TRANS_MB;
extern int MAXMOVES;
extern double INFINITY;
extern int rootDepth;

extern Coordinate movelist [];

#endif //TIDESTICTACTICS_DEFS_H
