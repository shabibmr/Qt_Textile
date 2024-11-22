#ifndef ADDCONVERSION_H
#define ADDCONVERSION_H

#include <QWidget>
#include <QMap>
#include <QCompleter>

#include "customwidgets/gmsearchlineedit.h"
#include "database/inventory/uom/uomdatabasehelper.h"
#include "datamodels/inventory/uom/uomdatamodel.h"
#include "datamodels/inventory/uom/uomconversiondatamodel.h"


namespace Ui {
class addConversion;
}

class addConversion : public QWidget
{
    Q_OBJECT

public:
    explicit addConversion(QWidget *parent = 0);
    ~addConversion();
    QMap<int,QString> map;
    UomDataBaseHelper* dbHelper;
    QString invItem = "";
    QString convId = "";
    QString itemCode;

    void setTitle(QString text);
    void disableSave();
public slots:
    void setBaseUnit(int baseUnit, QString invItem, QString ItemCode)    ;
    void setData(UomConversionDataModel* obj, QString code = "");

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_clicked();

private:
    Ui::addConversion *ui;

signals:
    void closing();
};

#endif // ADDCONVERSION_H
