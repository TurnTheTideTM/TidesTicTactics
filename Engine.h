//
// Created by RV Administrator on 17.06.2015.
//

#ifndef TIDESTICTACTICS_ENGINE_H
#define TIDESTICTACTICS_ENGINE_H


#include "defs.h"
#include "Board.h"

class Engine {
public:
    void checkUp(SEARCHINFO*);
    void pickNextMove(int, Movelist*);
    void clearForSearch(Board*, SEARCHINFO*);
    double alphaBeta(double, double, int, Board*, SEARCHINFO*);
    double quiescence(double, double, Board*, SEARCHINFO*);
    void searchPosition(Board*, SEARCHINFO*, bool);
};


#endif //TIDESTICTACTICS_ENGINE_H
