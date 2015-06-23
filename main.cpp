#include "Board.h"
#include "inits.h"
#include "IO.h"

using namespace std;

int main() {
    initAll();
    Board board;
    /*board.move(SQUARE_0, SQUARE_3);
    board.move(SQUARE_1, SQUARE_7);
    board.move(SQUARE_0, SQUARE_5);
    board.move(SQUARE_1, SQUARE_8);
    board.move(SQUARE_2, SQUARE_0);
    board.move(SQUARE_4, SQUARE_2);
    board.move(SQUARE_2, SQUARE_8);
    board.move(SQUARE_6, SQUARE_4);
    board.move(SQUARE_3, SQUARE_4);
    board.move(SQUARE_7, SQUARE_3);
    board.move(SQUARE_4, SQUARE_7);
    board.move(SQUARE_7, SQUARE_5);
    board.move(SQUARE_6, SQUARE_2);
    board.move(SQUARE_8, SQUARE_0);
    board.move(SQUARE_7, SQUARE_4);
    board.move(SQUARE_4, SQUARE_4);
    board.move(SQUARE_4, SQUARE_8);
    board.move(SQUARE_5, SQUARE_7);
    board.move(SQUARE_8, SQUARE_5);
    board.move(SQUARE_8, SQUARE_8);

    board.move(SQUARE_4, SQUARE_5);
    board.move(SQUARE_5, SQUARE_6);
    board.move(SQUARE_6, SQUARE_1);
    board.move(SQUARE_1, SQUARE_1);
    board.move(SQUARE_1, SQUARE_3);
    board.move(SQUARE_3, SQUARE_2);
    board.move(SQUARE_2, SQUARE_7);
    board.move(SQUARE_7, SQUARE_8);
    board.move(SQUARE_8, SQUARE_3);
    board.move(SQUARE_3, SQUARE_1);
    board.move(SQUARE_1, SQUARE_2);

    // Erste Variante der Engine, fehlerhafte Scoreberechnung
    board.move(SQUARE_2, SQUARE_5);
    board.move(SQUARE_5, SQUARE_1);
    board.move(SQUARE_1, SQUARE_0);
    board.move(SQUARE_0, SQUARE_0);
    board.move(SQUARE_0, SQUARE_6);
    board.move(SQUARE_6, SQUARE_8);
    board.move(SQUARE_8, SQUARE_4);
    board.move(SQUARE_4, SQUARE_6);

    // Variante mit gefixtem Score
    board.move(SQUARE_6, SQUARE_5);
    board.move(SQUARE_5, SQUARE_8);
    board.move(SQUARE_8, SQUARE_7);
    board.move(SQUARE_7, SQUARE_1);
    board.move(SQUARE_1, SQUARE_4);
    board.move(SQUARE_4, SQUARE_0);
    // board.move(SQUARE_0, SQUARE_8);*/

    // board.move(SQUARE_0, SQUARE_0);
    SEARCHINFO info;
    info.quit = false;
    IO io;

    io.consoleLoop(&board, &info);

    return 0;
}