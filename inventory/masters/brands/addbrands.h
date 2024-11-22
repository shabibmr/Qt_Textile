#ifndef ADDBRANDS_H
#define ADDBRANDS_H

#include <QWidget>
#include "database/inventory/brands/brandsdatabasehelper.h"

namespace Ui {
class AddBrands;
}

class AddBrands : public QWidget
{
    Q_OBJECT

public:
    explicit AddBrands(QWidget *parent = 0);
    ~AddBrands();
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
private:
    Ui::AddBrands *ui;
signals:
    void closing();
    void NotificationAccepted();
    void NotificationRejected(QString reason);
};

#endif // ADDBRANDS_H
