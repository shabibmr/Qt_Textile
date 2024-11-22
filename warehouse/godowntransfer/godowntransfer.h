#ifndef GODOWNTRANSFER_H
#define GODOWNTRANSFER_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "database/inventory/godowntransfer/godowntransferdatabasehelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"
#include "godowntransfereditor.h"

namespace Ui {
class GodownTransfer;
}

class GodownTransfer : public QWidget
{
    Q_OBJECT

public:
    explicit GodownTransfer(QWidget *parent = 0);
    ~GodownTransfer();
//    AddGodownTransfer *AddGodownTransferWidget;
    GodownTransferEditor *AddGodownTransferWidget;
    GodownTransferDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

public slots:
    void setHeading(QString label);
    void setTableView();

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void on_okPushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::GodownTransfer *ui;
};

#endif // GODOWNTRANSFER_H
