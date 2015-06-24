//
// Created by RV Administrator on 16.06.2015.
//

#include "bitboards.h"
#include "defs.h"

#define RAND_64 (	(uint64_t)rand() | \
					(uint64_t)rand() << 15 | \
					(uint64_t)rand() << 30 | \
					(uint64_t)rand() << 45 | \
					((uint64_t)rand() & 0xf) << 60    )

MiniBitboard singleSquaresMasks[SQUARE_NUMBER];
MiniBitboard singleSquaresMasksNegativ[SQUARE_NUMBER];
PosKey hashkeys[1<<8];

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

MiniBitboard chanceMasks[] {
        0b000000011,
        0b000000101,
        0b000000110,

        0b000011000,
        0b000101000,
        0b000110000,

        0b011000000,
        0b101000000,
        0b110000000,

        0b000100100,
        0b100000100,
        0b100100000,

        0b000010010,
        0b010000010,
        0b010010000,

        0b000001001,
        0b001000001,
        0b001001000,

        0b000010001,
        0b100000001,
        0b100010000,

        0b000010100,
        0b001000100,
        0b001010000
};

MiniBitboard full = 0b111111111;
MiniBitboard empty = 0b0;


MiniBitboard captureBoard[1<<9];
int popcountLookup[1<<9];


void initBitboards() {
    for(int square = 0; square < SQUARE_NUMBER; ++square) {
        singleSquaresMasks[square] = 1 << square;
        singleSquaresMasksNegativ[square] = 0b111111111 ^ (1 << square);
    }
    for (int square = 0; square < 1<<8; square++) {
        hashkeys[square] = RAND_64;
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
                temp = feld | square;
                for (MiniBitboard mask : winMasks) {
                    if ((temp & mask) == mask)
                        result |= square;
                }
            }
        }
        captureBoard[feld] = result;
    }
}