#ifndef PASSPORTEXPIRYLIST_H
#define PASSPORTEXPIRYLIST_H

#include <QWidget>
#include "database/hr/userprofiledatabasehelper.h"

namespace Ui {
class PassportExpiryList;
}

class PassportExpiryList : public QWidget
{
    Q_OBJECT

public:
    explicit PassportExpiryList(QWidget *parent = nullptr);
    ~PassportExpiryList();

    UserProfileDatabaseHelper *dbHelper;
    QSqlQueryModel *model;

private slots:
    void on_pushButton_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void setTable();
private:
    Ui::PassportExpiryList *ui;
};

#endif // PASSPORTEXPIRYLIST_H
