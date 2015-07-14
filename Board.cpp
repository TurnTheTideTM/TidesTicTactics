//
// Created by RV Administrator on 16.06.2015.
//
#include "Board.h"
#include <algorithm>
#include <sstream>

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
    key ^= hashkeys[color][squareBig<<4|squareSmall];
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
            key ^= hashkeysMeta[toMove][squareBig];
        } else if (smallBoards[squareBig].isFull()) {
            bigBoard.setSquare(squareBig, COLOR_BOTH);
            move |= 1 << 16;
            winner = bigBoard.whoWon();
            if (winner == COLOR_NONE) {
                if (bigBoard.isFull()) {
                    winner = COLOR_BOTH;
                }
            }
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
        smallBoards[squareBig].unsetWon();
        bigBoard.unsetWon();
        key ^= hashkeys[toMove][squareBig];
    }
    winner = COLOR_NONE;

    key ^= hashkeys[toMove][squareBig<<4|squareSmall];

    movecount--;
}

bool Board::isSet(Square bigSquare, Square smallSquare, Color color) {
    return smallBoards[bigSquare].isSet(smallSquare, color);
}

int Board::getScore() {
    switch (winner) {
        case COLOR_X:
            if (toMove == COLOR_X)
                return ISMATE;
            else
                return -ISMATE;
        case COLOR_O:
            if (toMove == COLOR_X)
                return -ISMATE;
            else
                return ISMATE;
        case COLOR_BOTH:
            return 0;
        default:
            break;
    }
    int sum = 0;
    for(int i = 0; i < SQUARE_NUMBER; i++) {
        sum += smallBoards[i].getScore();
    }
    sum += bigBoard.getScore() * 10;
    if (toMove == COLOR_X)
        return sum;
    else
        return -sum;
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
                bool impossible = false;
                for (MiniBitboard bitboard : winMasks) {
                    if (((smallBoards[x].boardstate[toMove] | singleSquaresMasks[y]) & bitboard) == bitboard) {
                        impossible = true;
                    }
                }
                if (impossible)
                    continue;
                movelist->moves[movelist->count].move = (Coordinate) (x<<4 | y);
                movelist->count++;
            }
        }
    } else {
        //                                        0b110101001            & 0b001000000               ^ 0b111111111
        //                                        0b111101001 ^ 0b111111111
        //                                        0b000010110
        MiniBitboard targets = (smallBoards[next].boardstate[COLOR_BOTH] | singleSquaresMasks[prev]) ^ full;
        int feld = sizeof(unsigned int) * 8 - __builtin_clz(targets) - 1;
        while (targets != 0) {
            targets ^= singleSquaresMasks[feld];
            movelist->moves[movelist->count].move = next << 4 | feld;
            movelist->count++;
            feld = sizeof(unsigned int) * 8 - __builtin_clz(targets) - 1;
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
                    targets = (smallBoards[x].boardstate[COLOR_BOTH] | singleSquaresMasks[prev]) ^ full;
                    feld = sizeof(unsigned int) * 8 - __builtin_clz(targets) - 1;
                    while (targets != 0) {
                        targets ^= singleSquaresMasks[feld];
                        movelist->moves[movelist->count].move = (Coordinate) (x << 4 | feld);
                        movelist->count++;
                        feld = sizeof(unsigned int) * 8 - __builtin_clz(targets) - 1;
                    }
                }
                if(movelist->count == 0) {
                    for(x = 0; x < 9; x++) {
                        targets = (smallBoards[x].boardstate[COLOR_BOTH]) ^ full;
                        feld = sizeof(unsigned int) * 8 - __builtin_clz(targets) - 1;
                        while (targets != 0) {
                            targets ^= singleSquaresMasks[feld];
                            movelist->moves[movelist->count].move = (Coordinate) (x << 4 | feld);
                            movelist->count++;
                            feld = sizeof(unsigned int) * 8 - __builtin_clz(targets) - 1;
                        }
                    }
                }
            }
        }
    }
}

void Board::getCaptureMoves(Movelist* movelist) {
    movelist->count = 0;
    // wenn das brett gewonnen ist kann nicht weiter gezogen werden.
    if (winner != COLOR_NONE) {
        return;
    }
    const MiniBitboard targetBoard = smallBoards[next].boardstate[COLOR_BOTH];
    MiniBitboard moves;
    if(popcountLookup[targetBoard] == 9) {
        for (int i = 0; i < 9; i++) {
            if(bigBoard.isSet(i, COLOR_BOTH))
                continue;
            moves = captureBoard[smallBoards[i].boardstate[toMove]];
            int feld = sizeof(unsigned int) * 8 - __builtin_clz(moves) - 1;
            while (moves != 0) {
                moves ^= singleSquaresMasks[feld];
                if (feld != prev && (smallBoards[i].boardstate[COLOR_BOTH] & singleSquaresMasks[feld]) == 0) {
                    movelist->moves[movelist->count].move = (Coordinate) (i << 4 | feld);
                    movelist->count++;
                }
                feld = sizeof(unsigned int) * 8 - __builtin_clz(moves) - 1;
            }
        }
        return;
    } else if(popcountLookup[targetBoard] == 8) {
        moves = captureBoard[targetBoard];
        int feld = sizeof(unsigned int) * 8 - __builtin_clz(moves) - 1;
        movelist->moves[movelist->count].move = next << 4 | feld;
        movelist->count++;
        return;
    }
    else {
        if (bigBoard.isSet(next, COLOR_BOTH))
            return;
        moves = captureBoard[smallBoards[next].boardstate[toMove]];
        int feld = sizeof(unsigned int) * 8 - __builtin_clz(moves) - 1;
        while (moves != 0) {
            moves ^= singleSquaresMasks[feld];
            if (feld != prev && (smallBoards[next].boardstate[COLOR_BOTH] & singleSquaresMasks[feld]) == 0) {
                movelist->moves[movelist->count].move = next << 4 | feld;
                movelist->count++;
            }
            feld = sizeof(unsigned int) * 8 - __builtin_clz(moves) - 1;
        }
    }
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
    std::string result = "";
    if (winner == COLOR_NONE) {
        result +=  "To move: ";
        if (toMove == COLOR_X)
            result += "X";
        else
            result += "O";
    } else {
        result += "Winner: ";
        switch (winner) {
            case COLOR_X:
                result += "X";
                break;
            case COLOR_O:
                result += "O";
                break;
            case COLOR_BOTH:
                result += "BOTH";
                break;
            default:
                break;
        }
        result += "\n";
    }
    result += "\n";
    result += " Plys: " + std::to_string(movecount) + "\n";
    std::stringstream keystring;
    for(int i = (int) floor(key.size() / 4); i >= 0; i--) {
        keystring << std::hex << ((int) (( ( key & (PosKey(0xF) << (i*4)) ) ) >> (i*4)).to_ulong());
    }
    result += " PosKey: " + keystring.str() + "\n";
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

void Board::setupmove(Coordinate m) {
    Square sq_big = (Square) ((m & 0xF0) >> 4);
    Square sq_small = (Square) (m & 0xF);
    if (smallBoards[sq_big].isSet(sq_small, COLOR_BOTH)) {
        smallBoards[sq_big].unsetSquare(sq_small, COLOR_X);
        smallBoards[sq_big].unsetSquare(sq_small, COLOR_O);
        movecount--;
        key ^= hashkeys[toMove][m];
    } else {
        move(m);
    }
}

Board::~Board() {
    std::cout << "Deleting Transpositiontable..." << std::endl;
    delete transpositiontable;
}