#ifndef DEBITNOTE_H
#define DEBITNOTE_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>

#include "debitnotevouchereditor.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class debitNote;
}

class debitNote : public QWidget
{
    Q_OBJECT

public:
    explicit debitNote(QWidget *parent = 0);
    ~debitNote();
    DebitNoteVoucherEditor* addDebitNoteWidget;
    QList<GeneralVoucherDataObject> voucherList;
    DebitNoteVoucherDatabaseHelper *dbhelper;
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
    Ui::debitNote *ui;
};

#endif // DEBITNOTE_H
