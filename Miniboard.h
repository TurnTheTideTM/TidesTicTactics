//
// Created by RV Administrator on 16.06.2015.
//

#ifndef TIDESTICTACTICS_MINIBOARD_H
#define TIDESTICTACTICS_MINIBOARD_H

#include "defs.h"
#include <string>
#include "bitboards.h"

class Miniboard {
public:
    MiniBitboard boardstate [COLOR_NUMBER + 1];    // X O and BOTH
    Miniboard();
    void setSquare(Square, Color);
    void unsetSquare(int, Color);
    int getScore();
    bool isSet(Square, Color);
    bool isSet(int, Color);
    bool isWon();
    bool isFull();
    Color whoWon();
    std::string printMiniboard();
};


#endif //TIDESTICTACTICS_MINIBOARD_H
