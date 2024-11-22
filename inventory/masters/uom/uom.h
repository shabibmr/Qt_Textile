#ifndef UOM_H
#define UOM_H

#include <QWidget>
#include <QKeyEvent>
#include <QSqlRecord>
#include "adduom.h"
#include "addconversion.h"
#include "database/inventory/uom/uomdatabasehelper.h"
#include "datamodels/inventory/uom/uomdatamodel.h"
namespace Ui {
class UOM;
}

class UOM : public QWidget
{
    Q_OBJECT

public:
    explicit UOM(QWidget *parent = 0);
    ~UOM();

    addUOM* addUOMWidget;
    addConversion* addUOMCompoundWidget;
    UomDataBaseHelper *dbHelper;
    QList<UomDataModel*> uomList;
    QList<UomConversionDataModel> convList;
    QSqlQueryModel *model;

public slots:
    void settable();

private slots:
    void on_createNewMeasurementPushButton_clicked();

    void on_createNewConversionPushButton_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

    void on_UomConversionTableView_doubleClicked(const QModelIndex &index);

private:
    Ui::UOM *ui;
};

#endif // UOM_H
