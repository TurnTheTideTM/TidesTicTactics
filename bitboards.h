//
// Created by RV Administrator on 16.06.2015.
//

#ifndef TIDESTICTACTICS_BITBOARDS_H
#define TIDESTICTACTICS_BITBOARDS_H

#include "defs.h"

extern MiniBitboard singleSquaresMasks[SQUARE_NUMBER];
extern MiniBitboard singleSquaresMasksNegativ[SQUARE_NUMBER];
extern MiniBitboard winMasks[8];
extern MiniBitboard chanceMasks[8*3];

extern PosKey hashkeys[1<<8]; // Key = position

extern MiniBitboard full;
extern MiniBitboard empty;

extern void initBitboards();

#endif //TIDESTICTACTICS_BITBOARDS_H
