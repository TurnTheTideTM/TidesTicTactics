//
// Created by RV Administrator on 17.06.2015.
//

#include "Engine.h"

void Engine::checkUp(SEARCHINFO* info) {
    if (info->timeset && GetTimeMs() >= info->stoptime) {
        info->stopped = true;
    }
    // AUSKOMMENTIEREN WENN IN IDE (wg. Buffered stdout)
    ReadInput(info);
}

void Engine::pickNextMove(int moveNum, Movelist* movelist) {
    S_MOVE temp;
    int index = 0;
    double bestScore = 0;
    int bestNum = moveNum;

    for(index = moveNum; index < movelist->count; ++index) {
        if (movelist->moves[index].score > bestScore) {
            bestScore = movelist->moves[index].score;
            bestNum = index;
        }
    }
    temp = movelist->moves[moveNum];
    movelist->moves[moveNum] = movelist->moves[bestNum];
    movelist->moves[bestNum] = temp;

}
void Engine::clearForSearch(Board* board, SEARCHINFO* info) {
    int index = 0;
    for(index = 0; index < 81; ++index) {
        board->searchKillers[index]= NOMOVE;
    }

    board->transpositiontable->overWrite = 0;
    board->transpositiontable->hits = 0;
    board->transpositiontable->cuts = 0;

    info->starttime = GetTimeMs();
    info->stopped = false;
    info->nodes = 0;
    info->fh = 0;   // fail high
    info->fhf = 0;  // fail high first
}

int Engine::quiescence(int alpha, int beta, Board* board, SEARCHINFO* info) {

    if(( info->nodes & 2047 ) == 0) {
        checkUp(info);
    }

    info->nodes++;

    if (board->movecount > MAXMOVES - 1) {
        return board->getScore();
    }

    int Score = board->getScore();

    if(Score >= beta) {
        return beta;
    }

    if(Score > alpha) {
        alpha = Score;
    }

    Movelist movelist;
    board->getCaptureMoves(&movelist);		// generates captures, not moves

    int moveNum;
    int Legal = 0;
    Score;

    for(moveNum = 0; moveNum < movelist.count; ++moveNum) {

        pickNextMove(moveNum, &movelist);

        board->move(movelist.moves[moveNum].move);
        Legal++;
        Score = -quiescence( -beta, -alpha, board, info);
        board->undo();

        if(info->stopped) {
            return 0;
        }

        if(Score > alpha) {
            if(Score >= beta) {
                if(Legal==1) {
                    info->fhf++;
                }
                info->fh++;
                return beta;
            }
            alpha = Score;
        }
    }

    return alpha;
}

int Engine::alphaBeta(int alpha, int beta, int depth, Board* board, SEARCHINFO* info) {
    if(depth == 0) {
        return quiescence(alpha, beta, board, info);
    }
    if ((info->nodes & 2047) == 0) {
        checkUp(info);
    }
    info->nodes++;
    if (board->movecount > MAXMOVES - 1) {
        return board->getScore();
    }

    int score = -INFINITY;
    Coordinate pvMove = NOMOVE;

   if(board->transpositiontable->probeEntry(board, &pvMove, &score, alpha, beta, depth)) {
        board->transpositiontable->cuts++;
        return score;
    }

    Movelist movelist;
    board->getMoves(&movelist);

    int moveNum = 0;
    int legal = 0;
    int oldAlpha = alpha;
    Coordinate bestMove = NOMOVE;

    score = -INFINITY;
    int bestScore = -INFINITY;

    if(pvMove != NOMOVE) {
        for(moveNum = 0; moveNum < movelist.count; ++moveNum) {
            if(movelist.moves[moveNum].move == pvMove) {
                movelist.moves[moveNum].score = 200000;
                break;
            }
        }
    }

    bool foundPv = false;
    for(moveNum = 0; moveNum < movelist.count; ++moveNum) {
        pickNextMove(moveNum, &movelist);
        board->move(movelist.moves[moveNum].move);
        legal++;
        if (foundPv) {
            score = -alphaBeta(-alpha - 1, -alpha, depth - 1, board, info);
            if(score > alpha && score < beta) {
                score = -alphaBeta(-beta, -alpha, depth - 1, board, info);
            }
        } else {
            score = -alphaBeta(-beta, -alpha, depth - 1, board, info);
        }
        board->undo();
        if (info->stopped) {
            return 0;
        }
        if(score > bestScore) {
            bestScore = score;
            bestMove = movelist.moves[moveNum].move;
            if (score > alpha) {
                if (score >= beta) {
                    if (legal == 1) {
                        info->fhf++;
                    }
                    info->fh++;
                    //printf("        Cut Move: %s Beta: %d  Alpha %d Movedepth %d\n",PRMOVE(board->prev << 4 | board->next).c_str(), beta, alpha, board->movecount);
                    board->searchKillers[board->movecount] = movelist.moves[moveNum].move;
                    board->transpositiontable->storeEntry(board, bestMove, beta, HF_BETA, depth);
                    return beta;
                } else {
                    //printf("   New Best Move: %s Beta: %d  Alpha %d Movedepth %d\n",PRMOVE(board->prev << 4 | board->next).c_str(), beta, alpha, board->movecount);
                }
                foundPv = true;
                alpha = score;
                bestMove = movelist.moves[moveNum].move;
            }else {
                //printf("Did not cut Move: %s Beta: %d  Alpha %d Movedepth %d\n",PRMOVE(board->prev << 4 | board->next).c_str(), beta, alpha, board->movecount);
            }
        }
    }
    if(legal == 0) {
        switch (board->bigBoard.whoWon()) {
            case COLOR_X:
                if(board->toMove == COLOR_X) {
                    return ISMATE - board->movecount;
                } else {
                    return -ISMATE + board->movecount;
                }
            case COLOR_O:
                if(board->toMove == COLOR_O) {
                    return ISMATE - board->movecount;
                } else {
                    return -ISMATE + board->movecount;
                }
            case COLOR_BOTH:
                return 0;
            case COLOR_NONE:
                return 0;
            default:
                return 0;
        }
    }
    if(alpha != oldAlpha) {
        board->transpositiontable->storeEntry(board, bestMove, bestScore, HF_EXACT, depth);
    } else {
        board->transpositiontable->storeEntry(board, bestMove, bestScore, HF_ALPHA, depth);
    }

    return alpha;
}
void Engine::searchPosition(Board* board, SEARCHINFO* info, bool makemove) {
    Coordinate bestMove = NOMOVE;
    int bestScore;
    int currentDepth = 0;
    int pvMoves = 0;
    int pvNum = 0;

    clearForSearch(board, info);

    for(currentDepth = 1; currentDepth <= info->depth; ++currentDepth) {
        bestScore = alphaBeta(-INFINITY, INFINITY, currentDepth, board, info);
        if(info->stopped) {
            break;
        }
        pvMoves = board->transpositiontable->getPvLine(81, board);
        bestMove = board->pvArray[0];
        printf("score: %d depth: %d nodes: %lld time: %dms ",
               bestScore,currentDepth,info->nodes,GetTimeMs()-info->starttime);
        if (info->POST_THINKING) {
            for(pvNum = 0; pvNum < pvMoves; ++pvNum) {
                printf(PRMOVE(board->pvArray[pvNum]).c_str());
            }
            printf("\n");
        }
        printf("Hits: %d Overwrite: %d NewWrite: %d Cut: %d FH: %ld FHF %ld\n",board->transpositiontable->hits,board->transpositiontable->overWrite,board->transpositiontable->newWrite,board->transpositiontable->cuts, info->fh, info->fhf);
        if (abs(bestScore) + MAXMOVES > ISMATE) {
            break;
        }
        if (MAXMOVES - board->movecount - currentDepth < 1) {
            break;
        }
    }
    if (bestMove == NOMOVE) {
        printf("Found no valid moves.\n");
    } else {
        printf("move %s\n", PRMOVE(bestMove).c_str());
        if(makemove) {
            board->move(bestMove);
        }
    }
}