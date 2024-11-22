#ifndef PREVIOUSQUOTATIONSLIST_H
#define PREVIOUSQUOTATIONSLIST_H

#include <QWidget>
#include <QDate>
#include <QKeyEvent>
#include "projects/History/previousquotationwidget.h"


namespace Ui {
class PreviousQuotationsList;
}

class PreviousQuotationsList : public QWidget
{
    Q_OBJECT

public:
    explicit PreviousQuotationsList(QWidget *parent = 0);
    ~PreviousQuotationsList();
    PreviousQuotationWidget* addQuotationObject;
    QList<GeneralVoucherDataObject> voucherList;
    OldQuotationDatabaseHelper *dbhelper;

public slots:
    void setHeading(QString label);

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void setTable();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_okPushButton_clicked();
    void delButtonClicked(bool);
    void enterPressed();

    void on_fromDateDateEdit_editingFinished();

private:
    Ui::PreviousQuotationsList *ui;
};

#endif // PREVIOUSQUOTATIONSLIST_H
