#ifndef SERVICESTATUS_H
#define SERVICESTATUS_H


class ServiceStatus
{
public:
    ServiceStatus();
    static int Registered;
    static int Open;
    static int Inspected;
    static int UnknownError;
    static int WaitingforQuote;
    static int WaitingforSpares;
    static int MoveToFactory;
    static int inFactory;
    static int Miscellaneous;
    static int Fixed;
    static int Assigned;
    static int Rejected;
};

#endif // SERVICESTATUS_H
