#ifndef RECEIPTNOTES_H
#define RECEIPTNOTES_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "receiptnotevouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class ReceiptNotes;
}

class ReceiptNotes : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiptNotes( bool hideCreate=false, QWidget *parent = 0);
    ~ReceiptNotes();


    ReceiptNoteVoucherEditor *AddReceiptNoteWidget;
    ReceiptNoteDatabaseHelper *dbhelper;
    QSqlQueryModel *model;
    GMItemSortFilterProxyModel *sort;

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);

    void on_tableView_doubleClicked(const QModelIndex &index);
    void setTableView();

private:
    Ui::ReceiptNotes *ui;
};

#endif // RECEIPTNOTES_H
