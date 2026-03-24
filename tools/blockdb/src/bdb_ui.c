#include "bdb_ui.h"

#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

int bdb_ui_width()
{
    int width = 0;

    {
        const char *env = getenv("COLUMNS");
        if(env && *env)
        {
            int val = atoi(env);
            if(val > 0)
                return val;
        }
    }

#if defined(_WIN32)

    {
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        if(h != INVALID_HANDLE_VALUE)
        {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if(GetConsoleScreenBufferInfo(h, &csbi))
            {
                width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
                if(width > 0)
                    return width;
            }
        }
    }

#else

    {
        struct winsize ws;
        if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
        {
            if(ws.ws_col > 0)
                return ws.ws_col;
        }
    }

#endif

    return 80;
}

void bdb_ui_progress(int current, int total)
{
    int term_w = 0;
    int info_len = 0;
    int bar_w = 0;
    int filled = 0;

    if(total <= 0) 
        total = 1;
    if(current < 0) 
        current = 0;
    if(current > total) 
        current = total;

    term_w = bdb_ui_width();
    info_len = snprintf(0, 0, " %d/%d", current, total);
    if(info_len < 0) 
        info_len = 0;

    bar_w = term_w - info_len - 3; /* '[' + ']' + space */
    if(bar_w < 10)
        bar_w = 10; /* minimum */

    filled = (current * bar_w) / total;

    fputs("\r[", stderr);
    for(int i = 0; i < bar_w; i++)
    {
        if(i < filled)
            fputc('#', stderr);
        else
            fputc(' ', stderr);
    }

    fprintf(stderr, "] %d/%d", current, total);
}

void bdb_ui_progress_end()
{
    fputc('\n', stderr);
}

