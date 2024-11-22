#ifndef MATERIALINOUT_H
#define MATERIALINOUT_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "materialinouteditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
namespace Ui {
class materialInOut;
}

class materialInOut : public QWidget
{
    Q_OBJECT

public:
    explicit materialInOut(QWidget *parent = 0);
    ~materialInOut();

    MaterialInOutEditor *AddMaterialInOutWidget;
    QList<GeneralVoucherDataObject> voucherList;
    materialinoutDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();
    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::materialInOut *ui;
};

#endif // MATERIALINOUT_H
