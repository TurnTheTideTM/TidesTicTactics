//
// Created by RV Administrator on 16.06.2015.
//

#ifndef TIDESTICTACTICS_BOARD_H
#define TIDESTICTACTICS_BOARD_H


#include "Miniboard.h"
#include "defs.h"
#include "Transpositiontable.h"
#include <string>

class Transpositiontable;

class Board {
public:
    Transpositiontable* transpositiontable;
    Coordinate pvArray[81];
    Miniboard smallBoards[SQUARE_NUMBER];
    Miniboard bigBoard;
    PosKey key;
    Color winner;
    Color toMove;
    Square prev;
    Square next;
    int movecount;
    HistoryMove movelist[81];
    HistoryMove searchKillers[81];
public:
    Board();
    void setzeFeld(Square, Square, Color);
    void setzeFeld(int, int, Color);
    void move(Square, Square);
    void move(HistoryMove);
    void undo();
    bool isSet(Square, Square, Color);
    bool isInMoves(int, int, Movelist*);
    bool isInMoves(HistoryMove, Movelist*);
    bool moveExists(HistoryMove);
    double getScore();
    void getMoves(Movelist*);
    void getCaptureMoves(Movelist*);
    std::string printBoard();
    std::string printBoard(Color);
};


#endif //TIDESTICTACTICS_BOARD_H
