#ifndef CREDITNOTE_H
#define CREDITNOTE_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "creditnotevouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class creditNote;
}

class creditNote : public QWidget
{
    Q_OBJECT

public:
    explicit creditNote(QWidget *parent = 0);
    ~creditNote();
    CreditNoteVoucherEditor* addCreditNoteWidget;
    QList<GeneralVoucherDataObject> voucherList;
    CreditNoteVoucherDatabaseHelper *dbhelper;
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
    Ui::creditNote *ui;
};

#endif // CREDITNOTE_H
