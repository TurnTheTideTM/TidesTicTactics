//
// Created by RV Administrator on 18.06.2015.
//

#include "IO.h"
#include "string.h"
#include "Engine.h"

void IO::consoleLoop(Board* board, SEARCHINFO* info) {
    printf("Welcome to TidesTicTactics in Console Mode!\n");
    info->POST_THINKING = true;
    setbuf(stdin, NULL);
    setbuf(stdin, NULL);
    int depth = 81;
    int movetime = 500; // 3 sec
    Coordinate move;
    char inBuf[80], command[80];
    Color engineSide = COLOR_NONE;
    Engine engine;
    Movelist movecheck;

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
        }
        printf("\nTidesTicTactics > ");
        fflush(stdout);
        memset(&inBuf[0], 0, sizeof(inBuf));
        fflush(stdout);
        if(!fgets(inBuf, 80, stdin))
            continue;
        sscanf(inBuf, "%s", command);

        if(!strcmp(command, "help")) {
            printf("\nCommands:\n");
            printf("quit - quit game\n");
            printf("force - will not move\n");
            printf("print - show board\n");
            printf("post - show thinking\n");
            printf("nopost - do not show thinking\n");
            // printf("new - start new game # TODO\n");
            printf("go - set computer thinking\n");
            printf("depth x - set depth to x\n");
            printf("time x - set thinking time to x seconds (depth still applies if set)\n");
            printf("view - show current depth and movetime settings\n");
            printf("moves - show valid moves\n");
            printf("captures - show moves winning a board");
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

        if(!strcmp(command, "captures")) {
            board->getCaptureMoves(&movecheck);
            if (movecheck.count == 0) {
                printf("No captures found");
            }
            for(int i = 0; i < movecheck.count; i++) {
                printf("%s ", PRMOVE(movecheck.moves[i].move).c_str());
            }
            printf("\n");
            continue;
        }

        if(!strcmp(command, "eval")) {
            printf(board->printBoard().c_str());
            printf("\nEval:%d",board->getScore());
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
            while (board->winner == COLOR_NONE) {
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

        if(!strcmp(command, "time")) {
            sscanf(inBuf, "time %d", &movetime);
            movetime *= 1000;
            continue;
        }

        if(!strcmp(command, "new")) {
            engineSide = COLOR_O;
            printf("#TODO new noch nicht implementiert");
            continue;
        }

        if(!strcmp(command, "go")) {
            engineSide = board->toMove;
            continue;
        }

        move = parseMove(inBuf);
        if(move == NOMOVE) {
            printf("Command unknown:%s\n",inBuf);
            continue;
        }
        board->move(move);
    }
}

Coordinate IO::parseMove(char* string) {
    if (!(string[0] == 'B' && string[2] == 'F')) {
        return NOMOVE;
    }
    int board_big = string[1] - '1';
    int board_small = string[3] - '1';
    return board_big << 4 | board_small;
}
