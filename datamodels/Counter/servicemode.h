#ifndef SERVICEMODE_H
#define SERVICEMODE_H

#include <QString>

class ServiceMode
{
public:
    ServiceMode();

    static int Delivery;
    static int TakeAway;
    static int DineIn;
    static int Store;


    static QString getStatusString(int id);
};


#endif // SERVICEMODE_H
