//
// Created by RV Administrator on 17.06.2015.
//

#include <math.h>
#include "Transpositiontable.h"

/*
 * HashEntry *positionTable;
    int count;
    int hits;
    int cuts;
 */
Transpositiontable::Transpositiontable(int sizeMB) {
    int hashSize = 0x100000 * sizeMB;
    count = (long) (hashSize / sizeof(HashEntry));
    count -= 2;
    count = 1 << ((int) ceil(log2(count)) - 1);
    powerKey = PosKey(count - 1);
    // if (positionTable != NULL) free(positionTable);
    positionTable = (HashEntry *) malloc(count * sizeof(HashEntry));
    clearTable();
}

int Transpositiontable::getPvLine(int depth, Board *board) {
    Coordinate move = probePvMove(board);
    int counter = 0;

    while (move != NOMOVE && counter < depth) {
        if (board->moveExists(move)) {
            board->move(move);
            board->pvArray[counter++] = move;
        } else {
            break;
        }
        move = probePvMove(board);
    }

    for (int i = counter; i > 0; i--) {
        board->undo();
    }

    return counter;
}

void Transpositiontable::clearTable() {
    HashEntry *hashEntry;
    for (hashEntry = positionTable; hashEntry < positionTable + count; hashEntry++) {
        hashEntry->poskey = 0;
        hashEntry->move = NOMOVE;
        hashEntry->depth = 0;
        hashEntry->score = 0;
        hashEntry->flags = HF_NONE;
    }
}

bool Transpositiontable::probeEntry(Board *board, Coordinate *move, int *score, int alpha, int beta, int depth) {
    long index = (board->key & powerKey).to_ulong();
    if (positionTable[index].poskey == board->key) {
        *move = positionTable[index].move;
        if (positionTable[index].depth >= depth) {
            hits++;

            *score = positionTable[index].score;
            if (*score > ISMATE) *score -= board->movecount;
            else if (*score < -ISMATE) *score += board->movecount;

            switch (positionTable[index].flags) {
                case HF_ALPHA:
                    if (*score <= alpha) {
                        *score = alpha;
                        return true;
                    }
                    break;
                case HF_BETA:
                    if (*score >= beta) {
                        *score = beta;
                        return true;
                    }
                    break;
                case HF_EXACT:
                    return true;
                default:
                    break;
            }
        }
    }
    return false;
}

void Transpositiontable::storeEntry(Board *board, Coordinate move, int score, HashFlag flag, int depth) {
    long index = (board->key & powerKey).to_ulong();
    if (positionTable[index].poskey == 0) {
        newWrite++;
    } else {
        overWrite++;
    }
    if (score > ISMATE) score += board->movecount;
    else if (score < -ISMATE) score -= board->movecount;

    positionTable[index].move = move;
    positionTable[index].poskey = board->key;
    positionTable[index].score = score;
    positionTable[index].flags = flag;
    positionTable[index].depth = depth;
}

Coordinate Transpositiontable::probePvMove(Board *board) {
    long index = (board->key & powerKey).to_ulong();
    if (board->key == positionTable[index].poskey) {
        return positionTable[index].move;
    }
    return NOMOVE;
}
