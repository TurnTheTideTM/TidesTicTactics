//
// Created by RV Administrator on 16.06.2015.
//

#include "Miniboard.h"
#include "RatingTable.h"

Miniboard::Miniboard() {
    won = COLOR_NONE;
    boardstate[COLOR_X] = 0b0000000000;
    boardstate[COLOR_O] = 0b0000000000;
    boardstate[COLOR_BOTH] = 0b000000000;
}

void Miniboard::setSquare(MiniBitboard toSet, Color color) {
    boardstate[color] |= toSet;
    boardstate[COLOR_BOTH] |= toSet;
}

void Miniboard::setSquare(Square square, Color color) {
    boardstate[color] = boardstate[color] | singleSquaresMasks[square];
    boardstate[COLOR_BOTH] |= singleSquaresMasks[square];
}

void Miniboard::setSquare(int square, Color color) {
    boardstate[color] |= singleSquaresMasks[square];
    boardstate[COLOR_BOTH] |= singleSquaresMasks[square];
}

void Miniboard::unsetSquare(MiniBitboard toSet, Color color) {
    boardstate[color] ^= toSet;
    boardstate[COLOR_BOTH] ^= toSet;
}

void Miniboard::unsetSquare(Square square, Color color) {
    boardstate[color] &= singleSquaresMasksNegativ[square];
    boardstate[COLOR_BOTH] &= singleSquaresMasksNegativ[square];
}

void Miniboard::unsetSquare(int square, Color color) {
    boardstate[color] &= singleSquaresMasksNegativ[square];
    boardstate[COLOR_BOTH] &= singleSquaresMasksNegativ[square];
}

double Miniboard::getScore() {
    switch (whoWon()) {
        case COLOR_X:
            return 5;
        case COLOR_O:
            return -5;
        case COLOR_BOTH:
            return 0;
        default:
            break;
    }
    return ratingTable[boardstate[COLOR_X] << 9 | boardstate[COLOR_O]];
}

bool Miniboard::isSet(Square square, Color color) {
    return (singleSquaresMasks[square] & boardstate[color]) != 0;
}

bool Miniboard::isSet(int square, Color color) {
    return (singleSquaresMasks[square] & boardstate[color]) != 0;
}

bool Miniboard::isWon() {
    for (MiniBitboard mask : winMasks) {
        // Sieht auch Wins mit unentschiedenen boards
        if (((boardstate[COLOR_X] ^ boardstate[COLOR_BOTH]) & mask) == mask)
            return true;
        else if (((boardstate[COLOR_O] ^ boardstate[COLOR_BOTH]) & mask) == mask)
            return true;
    }
    return false;
}

bool Miniboard::isFull() {
    return boardstate[COLOR_BOTH] == full;
}

Color Miniboard::whoWon() {
    for (MiniBitboard mask : winMasks) {
        bool win1 = ((boardstate[COLOR_X] ^ boardstate[COLOR_BOTH]) & mask) == mask;
        bool win2 = ((boardstate[COLOR_O] ^ boardstate[COLOR_BOTH]) & mask) == mask;
        bool winboth = (win1 && win2);
        if(winboth) {
            return COLOR_BOTH;
        } else if (win1) {
            return COLOR_O;
        } else if (win2) {
            return COLOR_X;
        }
    }
    return COLOR_NONE;
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
    result += " |\n \\ - - - /";
    return result;
}