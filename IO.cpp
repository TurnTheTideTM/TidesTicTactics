//
// Created by RV Administrator on 18.06.2015.
//

#include "IO.h"
#include "string.h"
#include "Engine.h"
#include <algorithm>

void IO::demo1(Board* board, int depth) {
    struct coordpair {
        Square moveBig;
        Square moveSmall;
    };
    coordpair game [81] = {
            coordpair {SQUARE_3, SQUARE_0},
            coordpair {SQUARE_0, SQUARE_1},
            coordpair {SQUARE_3, SQUARE_1},
            coordpair {SQUARE_0, SQUARE_7},
            coordpair {SQUARE_4, SQUARE_2},
            coordpair {SQUARE_1, SQUARE_4},
            coordpair {SQUARE_4, SQUARE_6},
            coordpair {SQUARE_2, SQUARE_2},
            coordpair {SQUARE_6, SQUARE_7},
            coordpair {SQUARE_2, SQUARE_6},
            coordpair {SQUARE_6, SQUARE_8},
            coordpair {SQUARE_4, SQUARE_0},
            coordpair {SQUARE_7, SQUARE_3},
            coordpair {SQUARE_4, SQUARE_8},
            coordpair {SQUARE_7, SQUARE_4},
            coordpair {SQUARE_6, SQUARE_3},
            coordpair {SQUARE_8, SQUARE_5},
            coordpair {SQUARE_6, SQUARE_5},
            coordpair {SQUARE_8, SQUARE_2},
            coordpair {SQUARE_2, SQUARE_3},
            coordpair {SQUARE_3, SQUARE_3},
            coordpair {SQUARE_3, SQUARE_5},
            coordpair {SQUARE_5, SQUARE_7},
            coordpair {SQUARE_7, SQUARE_1},
            coordpair {SQUARE_1, SQUARE_3},
            coordpair {SQUARE_3, SQUARE_2},
            coordpair {SQUARE_2, SQUARE_8},
            coordpair {SQUARE_8, SQUARE_1},
            coordpair {SQUARE_1, SQUARE_0},
            coordpair {SQUARE_0, SQUARE_2},
            coordpair {SQUARE_2, SQUARE_1},
            coordpair {SQUARE_1, SQUARE_7},
            coordpair {SQUARE_7, SQUARE_5},
            coordpair {SQUARE_5, SQUARE_0},
            coordpair {SQUARE_0, SQUARE_0},
            coordpair {SQUARE_0, SQUARE_5},
            coordpair {SQUARE_5, SQUARE_5},

            coordpair {SQUARE_NONE, SQUARE_NONE}
    };
    for(int i = 0; i < depth; i++) {
        if (game[i].moveBig != SQUARE_NONE)
            board->move(game[i].moveBig, game[i].moveSmall);
        else
            break;
    }
}

void IO::demo2(Board* board, int depth) {
    struct coordpair {
        Square moveBig;
        Square moveSmall;
    };
    // coordpair {SQUARE_0, SQUARE_0},
    coordpair game [81] = {
            coordpair {SQUARE_0, SQUARE_2},
            coordpair {SQUARE_0, SQUARE_5},
            coordpair {SQUARE_0, SQUARE_0},
            coordpair {SQUARE_1, SQUARE_3},
            coordpair {SQUARE_1, SQUARE_4},
            coordpair {SQUARE_2, SQUARE_0},
            coordpair {SQUARE_2, SQUARE_3},
            coordpair {SQUARE_3, SQUARE_1},
            coordpair {SQUARE_4, SQUARE_5},
            coordpair {SQUARE_4, SQUARE_2},
            coordpair {SQUARE_6, SQUARE_6},
            coordpair {SQUARE_5, SQUARE_6},
            coordpair {SQUARE_6, SQUARE_7},
            coordpair {SQUARE_6, SQUARE_4},
            coordpair {SQUARE_7, SQUARE_1},
            coordpair {SQUARE_7, SQUARE_7},
            coordpair {SQUARE_8, SQUARE_8},
            coordpair {SQUARE_8, SQUARE_8},
            coordpair {SQUARE_0, SQUARE_4},

            coordpair {SQUARE_NONE, SQUARE_NONE}
    };
    for(int i = 0; i < depth; i++) {
        if (game[i].moveBig != SQUARE_NONE)
            board->move(game[i].moveBig, game[i].moveSmall);
        else
            break;
    }
}

void IO::consoleLoop(Board* board, SEARCHINFO* info) {
    printf("Welcome to TidesTicTactics in Console Mode!\n");
    info->POST_THINKING = true;
    setbuf(stdin, NULL);
    setbuf(stdin, NULL);
    int depth = 81;
    int movetime = 500; // 3 sec
    Coordinate move;
    char inBuf[80], command[80], modifier[80];
    Color engineSide = COLOR_NONE;
    Engine engine;
    Movelist movecheck;
    bool init = false;
    bool initX = true;
    Movelist movelist;

    while (true) {
        fflush(stdout);
        if ((board->toMove == engineSide || engineSide == COLOR_BOTH) && board->winner == COLOR_NONE) {
            info->starttime = GetTimeMs();
            info->depth = depth;
            if (movetime != 0) {
                info->timeset = true;
                info->stoptime = info->starttime + movetime;
            }
            engine.searchPosition(board, info, true);
            engineSide = COLOR_NONE;
        }
        // print prompt
        printf("");
        fflush(stdout);
        memset(&inBuf[0], 0, sizeof(inBuf));
        fflush(stdout);
        if(!fgets(inBuf, 80, stdin))
            continue;
        std::transform(inBuf, inBuf + 80 - 1, inBuf, [](unsigned char c) { return std::tolower(c); });
        sscanf(inBuf, "%s", command);

        if(!strcmp(command, "help")) {
            printf("\nCommands:\n");
            printf("quit - quit game\n");
            printf("force - will not move\n");
            printf("print - show board\n");
            printf("post - show thinking\n");
            printf("nopost - do not show thinking\n");
            printf("new - start new game\n");
            printf("go - set computer thinking\n");
            printf("depth x - set depth to x\n");
            printf("time x - set thinking time to x seconds (depth still applies if set)\n");
            printf("view - show current depth and movetime settings\n");
            printf("moves - show valid moves\n");
            printf("captures - show moves winning a board");
            printf("test x - load first x moves of demo");
            printf("test2 x - load first x moves of demo");
            printf("** note ** - to reset time and depth, set to 0\n");
            printf("enter moves using B1..9F1..9 notation\n\n\n");
            continue;
        }

        if(!strcmp(command, "moves")) {
            board->getMoves(&movecheck);
            if (movecheck.count == 0) {
                printf("No moves found");
            }
            for(int i = 0; i < movecheck.count; i++) {
                printf("%s ", PRMOVE(movecheck.moves[i].move).c_str());
            }
            printf("\n");
            continue;
        }

        if(!strcmp(command, "player")) {
            sscanf(inBuf, "player %s init", modifier);
            if(!strcmp(modifier, "one")) {
                init = true;
                initX = true;
            } else if(!strcmp(modifier, "two")) {
                init = true;
                initX = false;
            } else {
                printf("unknown player: %s\n", modifier);
            }
            continue;
        }

        if(!strcmp(command, "start")) {
            sscanf(inBuf, "start %s", modifier);
            if(!strcmp(modifier, "turns")) {
                init = false;
                board->toMove = COLOR_X;
                board->next = SQUARE_NONE;
            } else if(!strcmp(modifier, "game")) {
                init = true;
            } else {
                printf("unknown start modifier: %s\n", modifier);
            }
            continue;
        }

        if(!strcmp(command, "captures")) {
            board->getCaptureMoves(&movecheck);
            if (movecheck.count == 0) {
                printf("no captures found");
            }
            for(int i = 0; i < movecheck.count; i++) {
                printf("%s ", PRMOVE(movecheck.moves[i].move).c_str());
            }
            printf("\n");
            continue;
        }

        if(!strcmp(command, "eval")) {
            printf(board->printBoard().c_str());
            printf("\neval:%d",board->getScore());
            continue;
        }
        if(!strcmp(command, "quit")) {
            info->quit = true;
            break;
        }

        if(!strcmp(command, "post")) {
            info->POST_THINKING = true;
            continue;
        }

        if(!strcmp(command, "print")) {
            printf(board->printBoard().c_str());
            continue;
        }

        if(!strcmp(command, "nopost")) {
            info->POST_THINKING = false;
            continue;
        }

        if(!strcmp(command, "force")) {
            engineSide = COLOR_NONE;
            continue;
        }

        if(!strcmp(command, "demo")) {
            Color temp = COLOR_NONE;
            while (temp != board->toMove && board->winner == COLOR_NONE) {
                temp = board->toMove;
                info->starttime = GetTimeMs();
                info->depth = std::min(depth, MAXMOVES - board->movecount);
                if (movetime != 0) {
                    info->timeset = true;
                    info->stoptime = info->starttime + movetime;
                }
                engine.searchPosition(board, info, true);
                printf(board->printBoard().c_str());
                // printf("\nNONE\n");
                // printf(board->printBoard(COLOR_NONE).c_str());

            }
            continue;
        }

        if(!strcmp(command, "view")) {
            if(depth == 81) printf("depth not set ");
            else printf("depth %d",depth);

            if(movetime != 0) printf(" movetime %ds\n",movetime/1000);
            else printf(" movetime not set\n");

            continue;
        }

        if(!strcmp(command, "depth")) {
            sscanf(inBuf, "depth %d", &depth);
            if(depth==0) depth = 81;
            continue;
        }

        if(!strcmp(command, "test")) {
            engineSide = COLOR_NONE;
            while (board->movecount > 0)
                board->undo();
            int target;
            sscanf(inBuf, "test %d", &target);
            demo1(board, target);
            continue;
        }

        if(!strcmp(command, "test2")) {
            engineSide = COLOR_NONE;
            while (board->movecount > 0)
                board->undo();
            int target;
            sscanf(inBuf, "test2 %d", &target);
            demo2(board, target);
            continue;
        }

        if(!strcmp(command, "time")) {
            sscanf(inBuf, "time %d", &movetime);
            movetime *= 1000;
            continue;
        }

        if(!strcmp(command, "new")) {
            engineSide = COLOR_O;
            while (board->movecount > 0)
                board->undo();
            continue;
        }

        if(!strcmp(command, "go")) {
            engineSide = board->toMove;
            info->stopped = false;
            continue;
        }

        move = parseMove(inBuf);
        if(move == NOMOVE) {
            printf("command unknown:%s\n",inBuf);
            continue;
        }
        if(init) {
            if(initX) {
                board->toMove = COLOR_X;
                board->setupmove(move);
            } else {
                board->toMove = COLOR_O;
                board->setupmove(move);
            }
        } else {
            board->getMoves(&movelist);
            bool found = false;
            for (int i = 0; i < movelist.count; i++) {
                if (move == movelist.moves[i].move) {
                    found = true;
                    break;
                }
            }
            if (found)
                board->move(move);
            else {
                printf("INVALID MOVE: %s\n", PRMOVE(move).c_str());
                printf("%s", board->printBoard().c_str());
                break;
            }
        }
    }
}

Coordinate IO::parseMove(char* string) {
    if (!(string[0] == 'b' && string[2] == 'f')) {
        return NOMOVE;
    }
    int board_big = string[1] - '1';
    int board_small = string[3] - '1';
    return (Coordinate) (board_big << 4 | board_small);
}
