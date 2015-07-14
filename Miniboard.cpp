//
// Created by RV Administrator on 16.06.2015.
//

#include "Miniboard.h"
#include "RatingTable.h"

Miniboard::Miniboard() {
    boardstate[COLOR_X] = 0b0000000000;
    boardstate[COLOR_O] = 0b0000000000;
    boardstate[COLOR_BOTH] = 0b000000000;
    whoHasWon = COLOR_NONE;
}

void Miniboard::setSquare(Square square, Color color) {
    boardstate[color] = boardstate[color] | singleSquaresMasks[square];
    boardstate[COLOR_BOTH] |= singleSquaresMasks[square];
    if (whoHasWon == COLOR_NONE) {
        bool win1 = wonBoard[boardstate[COLOR_X] ^ boardstate[COLOR_BOTH]];
        bool win2 = wonBoard[boardstate[COLOR_O] ^ boardstate[COLOR_BOTH]];
        if((win1 && win2)) {
            whoHasWon = COLOR_BOTH;
        } else if (win1) {
            whoHasWon = COLOR_O;
        } else if (win2) {
            whoHasWon = COLOR_X;
        }
    }
}

void Miniboard::unsetSquare(int square, Color color) {
    boardstate[color] &= singleSquaresMasksNegativ[square];
    boardstate[COLOR_BOTH] &= singleSquaresMasksNegativ[square];
}

void Miniboard::unsetWon() {
    whoHasWon = COLOR_NONE;
}

int Miniboard::getScore() {
    return ratingTable[boardstate[COLOR_X] << 9 | boardstate[COLOR_O]];
}

bool Miniboard::isSet(Square square, Color color) {
    return (singleSquaresMasks[square] & boardstate[color]) != 0;
}

bool Miniboard::isSet(int square, Color color) {
    return (singleSquaresMasks[square] & boardstate[color]) != 0;
}

bool Miniboard::isWon() {
    return whoHasWon != COLOR_NONE;
}

bool Miniboard::isFull() {
    return boardstate[COLOR_BOTH] == full;
}

Color Miniboard::whoWon() {
    return whoHasWon;
}

std::string Miniboard::printMiniboard() {
    std::string result = " / - - - \\\n |";
    for (int square = SQUARE_0; square < SQUARE_NUMBER; square++ ) {
        if (isSet((Square)square, COLOR_X)) {
            result += " X";
        } else if (isSet((Square)square, COLOR_O)) {
            result += " O";
        } else if (isSet((Square)square, COLOR_BOTH)) {
            result += " B";
        } else{
            result += "  ";
        }
        if (square % 3 == 2 && square != 8) {
            result += " |\n |";
        }
    }
    result += " |\n \\ - - - /\n";
    return result;
}