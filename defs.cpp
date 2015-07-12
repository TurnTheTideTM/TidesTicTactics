//
// Created by RV Administrator on 18.06.2015.
//

#include <string.h>
#include <io.h>
#include <winsock.h>
#include "defs.h"

#ifdef WIN32
#else
#include "sys/time.h"
#endif

Coordinate NOMOVE = 170;
int ISMATE = 10081;
int SIZE_TRANS_MB = 250;
int MAXMOVES = 81;
int INFINITY = 25565;
int BOARDSCORE = 100;

int GetTimeMs() {
#ifdef WIN32
    return (int)GetTickCount();
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec*1000 + t.tv_usec/1000;
#endif
}

// http://home.arcor.de/dreamlike/chess/
int InputWaiting()
{
#ifndef WIN32
    fd_set readfds;

    FD_ZERO (&readfds);
    FD_SET (fileno(stdin), &readfds);
    tv.tv_sec=0; tv.tv_usec=0;
    select(16, &readfds, 0, 0, &tv);

    return (FD_ISSET(fileno(stdin), &readfds));
#else
   static int init = 0, pipe;
   static HANDLE inh;
   DWORD dw;

   if (!init) {
     init = 1;
     inh = GetStdHandle(STD_INPUT_HANDLE);
     pipe = !GetConsoleMode(inh, &dw);
     if (!pipe) {
        SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
        FlushConsoleInputBuffer(inh);
      }
    }
    if (pipe) {
      if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) return 1;
      return (int)dw;
    } else {
      GetNumberOfConsoleInputEvents(inh, &dw);
      return (int) (dw <= 1 ? 0 : dw);
	}
#endif
}

void ReadInput(SEARCHINFO *info) {
    int             bytes;
    char            input[256] = "", *endc;

    if (InputWaiting()) {
        info->stopped = true;
        do {
            bytes=read(fileno(stdin),input,256);
        } while (bytes<0);
        endc = strchr(input,'\n');
        if (endc) *endc=0;

        if (strlen(input) > 0) {
            if (!strncmp(input, "quit", 4))    {
                printf("quit");
                info->quit = true;
            }
        }
        return;
    }
}


std::string PRMOVE(Coordinate m) {
    int boardBig = (m & 0xF0) >> 4;
    int boardSmall = m & 0xF;
    std::string returnMove= {'B', (char) ('1' + boardBig), 'F', (char) ('1' + boardSmall), ' '};
    return returnMove;
};