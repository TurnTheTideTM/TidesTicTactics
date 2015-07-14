//
// Created by RV Administrator on 16.06.2015.
//

#include "bitboards.h"

#define RAND_64 (	(uint64_t)rand() | \
					(uint64_t)rand() << 15 | \
					(uint64_t)rand() << 30 | \
					(uint64_t)rand() << 45 | \
					((uint64_t)rand() & 0xf) << 60    )

MiniBitboard singleSquaresMasks[SQUARE_NUMBER];
MiniBitboard singleSquaresMasksNegativ[SQUARE_NUMBER];
PosKey hashkeys[2][1<<8];

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
        hashkeys[0][square] = RAND_64;
        hashkeys[1][square] = RAND_64;
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