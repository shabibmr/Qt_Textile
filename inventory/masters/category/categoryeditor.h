#ifndef CATEGORYEDITOR_H
#define CATEGORYEDITOR_H

#include <QWidget>
#include "database/inventory/brands/brandsdatabasehelper.h"



class CategoryEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryEditor(QWidget *parent = 0);
    ~CategoryEditor();
    QString id ="";
    BrandsDatabaseHelper *dbHelper;
    BrandsDataModel *brand;
    void setBrand(BrandsDataModel *value);
    void disableSave();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();


    void sendNotifications(QStringList dbNameList);
public slots:
    void edit();

signals:
    void closing();
    void NotificationAccepted();
    void NotificationRejected(QString reason);
};

#endif // CATEGORYEDITOR_H
