#ifndef TERMSLIST_H
#define TERMSLIST_H

#include <QWidget>
#include "termsconditions.h"
#include "database/finance/terms/termsdatabasehelper.h"

namespace Ui {
class TermsList;
}

class TermsList : public QWidget
{
    Q_OBJECT

public:
    explicit TermsList(QWidget *parent = 0);
    ~TermsList();

    TermsConditions* TermsConditionsWidget;
    TermsDatabaseHelper* dbHelper;

    setTable();
private slots:
    void on_createNewPushButton_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::TermsList *ui;
};

#endif // TERMSLIST_H
