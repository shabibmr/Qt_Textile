#ifndef SLEEP_H
#define SLEEP_H

#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif

class Sleep{
public:
void qSleep(int ms)
{


#ifdef Q_OS_WIN
    Sleep(uint(ms));

#endif
}
};

#endif // SLEEP_H
