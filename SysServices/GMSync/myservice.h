#ifndef MYSERVICE_H
#define MYSERVICE_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include "qtservice.h"
#include <QNetworkAccessManager>
#include <QTimer>

class MyService : public QtService<QCoreApplication>,QObject
{


public:
    /**
     * @brief The constructor
     * @param argc
     * @param argv
     */
    MyService(int argc, char **argv);

    /**
     * @brief The deconstructor
     */
    ~MyService();

    /**
     * @brief Start the service
     */
    void start();

    /**
     * @brief Pause the service
     */
    void pause();

    /**
     * @brief Resume the service
     */
    void resume();

    /**
     * @brief Stop the service
     */
    void stop();


public slots:
    void callPost();
private:
    //Add in any pointers
    QTimer *datasync;
};

#endif // MYSERVICE_H
