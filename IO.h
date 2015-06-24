//
// Created by RV Administrator on 18.06.2015.
//

#ifndef TIDESTICTACTICS_IO_H
#define TIDESTICTACTICS_IO_H


#include "Board.h"

class IO {
public:
    void consoleLoop(Board*, SEARCHINFO*);
    void demo1(Board*, int);
    Coordinate parseMove(char*);

    void demo2(Board *board, int depth);
};


#endif //TIDESTICTACTICS_IO_H
