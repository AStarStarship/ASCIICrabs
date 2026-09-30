// Copyright AStarship <https://astarship.net>.
// POSIX substitute for Windows conio.h (keyboard input)
#pragma once
#ifndef CONIO_H
#define CONIO_H

#include <termios.h>
#include <unistd.h>

inline int _getch() {
    char c;
    return read(STDIN_FILENO, &c, 1) == 1 ? static_cast<int>(static_cast<unsigned char>(c)) : -1;
}

#endif // CONIO_H
