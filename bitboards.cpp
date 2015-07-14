//
// Created by RV Administrator on 16.06.2015.
//

#include "bitboards.h"

MiniBitboard singleSquaresMasks[SQUARE_NUMBER];
MiniBitboard singleSquaresMasksNegativ[SQUARE_NUMBER];
PosKey hashkeys[2][1<<8];
PosKey hashkeysMeta[2][9];

MiniBitboard winMasks[] = {
        0b000000111,
        0b000111000,
        0b111000000,
        0b100100100,
        0b010010010,
        0b001001001,
        0b100010001,
        0b001010100
};

MiniBitboard full = 0b111111111;


MiniBitboard captureBoard[1<<9];
int popcountLookup[1<<9];
bool wonBoard[1<<9];


void initBitboards() {
    for(int square = 0; square < SQUARE_NUMBER; ++square) {
        singleSquaresMasks[square] = (MiniBitboard) (1 << square);
        singleSquaresMasksNegativ[square] = (MiniBitboard) (0b111111111 ^ (1 << square));
    }
    for (int square = 0; square < 1<<8; square++) {
        int square_big = (square & 0b11110000) >> 4;
        int square_small = square & 0b00001111;
        if (square_big < 9 && square_small < 9) {
            hashkeys[0][square] = 0;
            hashkeys[0][square] |= PosKey(1) << (square_big * 9 + square_small);
            hashkeys[1][square] = 0;
            hashkeys[1][square] |= PosKey(1) << (square_big * 9 + square_small + 81);
        }
    }
    for (int square = 0; square < 9; square++) {
        hashkeysMeta[0][square] = 0;
        hashkeysMeta[0][square] |= PosKey(1) << (162 + square);
        hashkeysMeta[1][square] = 0;
        hashkeysMeta[1][square] |= PosKey(1) << (171 + square);
    }
    MiniBitboard result;
    MiniBitboard temp;
    int count;
    for(int feld = 0; feld < 1<<9; feld++) {
        // popcount
        count = 0;
        for (int i = 0; i < 9; i++) {
            if (((feld >> i) & 0x01) != 0) {
                count++;
            }
        }
        popcountLookup[feld] = count;

        // capture boards
        result = 0;

        if(popcountLookup[feld] == 8) {
            for (int square : singleSquaresMasks) {
                if((feld | square) != feld) {
                    result |= square;
                }
            }
        } else {
            for (int square : singleSquaresMasks) {
                temp = (MiniBitboard) (feld | square);
                for (MiniBitboard mask : winMasks) {
                    if ((temp & mask) == mask)
                        result |= square;
                }
            }
        }
        captureBoard[feld] = result;

        // wonBoard

        for (MiniBitboard mask : winMasks) {
            if ((feld & mask) == mask) {
                wonBoard[feld] = true;
                break;
            }
        }
    }

}