//
// Created by RV Administrator on 17.06.2015.
//

#ifndef TIDESTICTACTICS_TRANSPOSITIONTABLE_H
#define TIDESTICTACTICS_TRANSPOSITIONTABLE_H


#include "defs.h"
#include "Board.h"

class Board;

class Transpositiontable {
public:
    HashEntry *positionTable;
    long count;
    int hits;
    int cuts;
    int newWrite;   // saves how often a position got overwritten / newly written
    int overWrite;
    PosKey powerKey; // count = 2^power
    Transpositiontable(int);
    int getPvLine(int, Board*);
    void clearTable();
    bool probeEntry(Board*, Coordinate *, int*, int, int, int);
    void storeEntry(Board*, Coordinate , int, HashFlag, int);
    Coordinate probePvMove(Board*);
};


#endif //TIDESTICTACTICS_TRANSPOSITIONTABLE_H
