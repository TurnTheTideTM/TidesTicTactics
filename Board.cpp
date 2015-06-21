//
// Created by RV Administrator on 16.06.2015.
//

#include "Board.h"
#include <algorithm>

Board::Board() {
    transpositiontable = new Transpositiontable(SIZE_TRANS_MB);
    key = 0;
    next = SQUARE_NONE;
    prev = SQUARE_NONE;
    winner = COLOR_NONE;
    toMove = COLOR_X;
    movecount = 0;
}

void Board::setzeFeld(Square squareBig, Square squareSmall, Color color) {
    smallBoards[squareBig].setSquare(squareSmall, color);
    key ^= hashkeys[squareBig<<4|squareSmall];
}

void Board::setzeFeld(int squareBig, int squareSmall, Color color) {
    smallBoards[squareBig].setSquare(squareSmall, color);
    key ^= hashkeys[squareBig<<4|squareSmall];
}

void Board::move(HistoryMove m) {
    move((Square) ((m & 0xF0) >> 4), (Square) (m & 0xF));
}

void Board::move(Square squareBig, Square squareSmall) {

    setzeFeld(squareBig, squareSmall, toMove);

    // generate the move to save and set squares in the big board if needed
    HistoryMove move = 0;
    if (! bigBoard.isSet(squareBig, COLOR_BOTH)) {
        if (smallBoards[squareBig].isWon()) {
            bigBoard.setSquare(squareBig, toMove);
            winner = bigBoard.whoWon();
            move |= 1 << 16;
        } else if (smallBoards[squareBig].isFull()) {
            bigBoard.setSquare(squareBig, COLOR_BOTH);
            move |= 1 << 16;
            winner = bigBoard.whoWon();
        }
    }

    // Change color to move
    if (toMove == COLOR_X)
        toMove = COLOR_O;
    else
        toMove = COLOR_X;

    move |= next << 12;
    move |= prev << 8;
    move |= squareSmall << 4;
    move |= squareBig;
    movelist[movecount] = move;
    // won, next, prev, small, big
    // 1     4     4      4     4

    prev = squareBig;
    next = squareSmall;
    movecount++;
}

void Board::undo() {
    // extract data from move
    int move = movelist[movecount - 1];
    int squareBig = (move & 0xF);
    int squareSmall = (move & 0xF0) >> 4;
    prev = static_cast<Square> ((move & 0xF00) >> 8);
    next = static_cast<Square> ((move & 0xF000) >> 12);

    // Change color to move
    if (toMove == COLOR_X)
        toMove = COLOR_O;
    else
        toMove = COLOR_X;

    // undo move
    smallBoards[squareBig].unsetSquare(squareSmall, toMove);

    // reset big board if necessary
    if (move & 0x10000) {
        bigBoard.unsetSquare(squareBig, toMove);
    }
    winner = COLOR_NONE;

    key ^= hashkeys[squareBig<<4|squareSmall];

    movecount--;
}

bool Board::isSet(Square bigSquare, Square smallSquare, Color color) {
    return smallBoards[bigSquare].isSet(smallSquare, color);
}

double Board::getScore() {
    switch (winner) {
        case COLOR_X:
            return ISMATE;
        case COLOR_O:
            return -ISMATE;
        case COLOR_BOTH:
            return 0;
        default:
            break;
    }
    double sum = 0;
    for(int i = 0; i < SQUARE_NUMBER; i++) {
        sum += smallBoards[i].getScore();
    }
    sum += bigBoard.getScore() * 3;
    return sum;
}

void Board::getMoves(Movelist* movelist) {
    movelist->count = 0;
    // wenn das brett gewonnen ist kann nicht weiter gezogen werden.
    if (winner != COLOR_NONE) {
        return;
    }
    int x;
    int y;
    // next = -1 beim ersten zug --> alle zuege moeglich
    if (next < 0 || next > 8) {
        for(x = 0; x < 9; x++) {
            for(y = 0; y < 9; y++) {
                if((x == 4) && (y == 4)) {
                    continue;
                }
                movelist->moves[movelist->count].move = x<<4 | y;
                movelist->count++;
            }
        }
    } else {
        // addet alle squares vom miniboard, ausser den gesperrten
        for(x = 0; x < 9; x++) {
            if(! smallBoards[next].isSet(x, COLOR_BOTH)) {
                if(x == prev) {
                    continue;
                }
                movelist->moves[movelist->count].move = next<<4 | x;
                movelist->count++;
            }

        }
        if(movelist->count == 0) {
            // wenn der gesperrte square der einzige ist, der uebrig ist, muss dieser gespielt werden
            if(! smallBoards[next].isSet(prev, COLOR_BOTH)) {
                movelist->moves[movelist->count].move = next<<4 | prev;
                movelist->count++;
            }
            // andernfalls werden alle freien squares hinzugefuegt
            else {
                for(x = 0; x < 9; x++) {
                    for(y = 0; y < 9; y++) {
                        if (!smallBoards[x].isSet(y, COLOR_BOTH)) {
                            movelist->moves[movelist->count].move = x << 4 | y;
                            movelist->count++;
                        }
                    }
                }
            }
        }
    }
    /*for (int i = 0; i < movelist->count; i++) {
        printf("%s ", std::to_string(movelist->moves[i].move).c_str());
    }
    printf("\n");*/
}

bool Board::isInMoves(int x, int y, Movelist* moves) {
    for (int i = 0; i < moves->count; i++) {
        if((x<<4 | y) == moves->moves[i].move)
            return true;
    }
    return false;
}

bool Board::isInMoves(HistoryMove m, Movelist* moves) {
    for (int i = 0; i < moves->count; i++) {
        if(m == moves->moves[i].move)
            return true;
    }
    return false;
}

bool Board::moveExists(HistoryMove m) {
    Movelist moves;
    getMoves(&moves);
    return isInMoves(m, &moves);
}

std::string Board::printBoard(){
    std::string result = " To move: ";
    if (toMove == COLOR_X)
        result += "X";
    else
        result += "O";
    result += "\n";
    result += " Plys: " + std::to_string(movecount) + "\n";
    result += " PosKey: " + std::to_string(key) + "\n";
    result += " / - - - + - - - + - - - \\\n";

    Square feld_g_alt = SQUARE_NONE;
    Movelist moves;
    getMoves(&moves);

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            Square feld_g = static_cast<Square> (y / 3 + (x / 3) * 3);
            Square feld_k = static_cast<Square> (y % 3 + (x % 3) * 3);
            if (feld_g != feld_g_alt) {
                feld_g_alt = feld_g;
                result += " |";
            }
            if (isSet(feld_g, feld_k, COLOR_X))
                result += " X";
            else if (isSet(feld_g, feld_k, COLOR_O))
                result += " O";
            else if (isInMoves(feld_g, feld_k, &moves)) {
                result += " ~";
            }
            else
                result += "  ";
        }
        result += " |\n";
        if (x % 3 == 2 and x < 8)
            result += " | - - - + - - - + - - - |\n";
    }
    result += " \\ - - - + - - - + - - - /\n";
    result += " Metaboard\n";
    result += bigBoard.printMiniboard();
    return result;
}

std::string Board::printBoard(Color color){
    std::string result = " To move: ";
    if (toMove == COLOR_X)
        result += "X";
    else
        result += "O";
    result += "\n";
    result += " Plys: " + std::to_string(movecount) + "\n";
    result += " PosKey: " + std::to_string(key) + "\n";
    result += " / - - - + - - - + - - - \\\n";

    Square feld_g_alt = SQUARE_NONE;
    Movelist moves;
    getMoves(&moves);

    for (int x = 0; x < 9; x++) {
        for (int y = 0; y < 9; y++) {
            Square feld_g = static_cast<Square> (y / 3 + (x / 3) * 3);
            Square feld_k = static_cast<Square> (y % 3 + (x % 3) * 3);
            if (feld_g != feld_g_alt) {
                feld_g_alt = feld_g;
                result += " |";
            }
            if (color != COLOR_NONE && isSet(feld_g, feld_k, color))
                result += " " + (std::to_string(color));
            else if (isInMoves(feld_g, feld_k, &moves)) {
                result += " ~";
            }
            else
                result += "  ";
        }
        result += " |\n";
        if (x % 3 == 2 and x < 8)
            result += " | - - - + - - - + - - - |\n";
    }
    result += " \\ - - - + - - - + - - - /\n";
    result += " Metaboard\n";
    result += bigBoard.printMiniboard();
    return result;
}
