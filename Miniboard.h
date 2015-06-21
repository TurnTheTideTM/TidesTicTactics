//
// Created by RV Administrator on 16.06.2015.
//

#ifndef TIDESTICTACTICS_MINIBOARD_H
#define TIDESTICTACTICS_MINIBOARD_H

#include "defs.h"
#include <string>
#include "bitboards.h"

class Miniboard {
    MiniBitboard boardstate [COLOR_NUMBER + 1];    // X O and BOTH
    Color won;
public:
    Miniboard();
    void setSquare(Square, Color);
    void setSquare(MiniBitboard, Color);
    void setSquare(int, Color);
    void unsetSquare(Square, Color);
    void unsetSquare(MiniBitboard, Color);
    void unsetSquare(int, Color);
    double getScore();
    bool isSet(Square, Color);
    bool isSet(int, Color);
    bool isWon();
    bool isFull();
    Color whoWon();
    std::string printMiniboard();
};


#endif //TIDESTICTACTICS_MINIBOARD_H
