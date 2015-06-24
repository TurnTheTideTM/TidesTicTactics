#include "Board.h"
#include "inits.h"
#include "IO.h"

using namespace std;

int main() {
    initAll();
    Board board;

    SEARCHINFO info;
    info.quit = false;
    IO io;

    io.consoleLoop(&board, &info);

    return 0;
}