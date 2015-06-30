//
// Created by RV Administrator on 16.06.2015.
//

#ifndef TIDESTICTACTICS_BITBOARDS_H
#define TIDESTICTACTICS_BITBOARDS_H

#include "defs.h"

extern MiniBitboard singleSquaresMasks[SQUARE_NUMBER];
extern MiniBitboard singleSquaresMasksNegativ[SQUARE_NUMBER];
extern MiniBitboard winMasks[8];
extern MiniBitboard captureBoard[1<<9];
extern int popcountLookup[1<<9];
extern bool wonBoard[1<<9];

extern PosKey hashkeys[1<<8]; // Key = position


extern MiniBitboard full;

extern void initBitboards();

#endif //TIDESTICTACTICS_BITBOARDS_H
