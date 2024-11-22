#ifndef DELIVERYNOTES_H
#define DELIVERYNOTES_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "deliverynoteeditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class deliveryNotes;
}

class deliveryNotes : public QWidget
{
    Q_OBJECT

public:
    explicit deliveryNotes(bool hideCreate=false, QWidget *parent = 0);
    ~deliveryNotes();
    DeliveryNoteEditor *AddDeliveryNoteWidget;
    QList<GeneralVoucherDataObject> voucherList;
    DeliveryNoteDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTableView();
//    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::deliveryNotes *ui;
};

#endif // DELIVERYNOTES_H
