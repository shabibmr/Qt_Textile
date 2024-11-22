#ifndef USERSLIST_H
#define USERSLIST_H

#include <QWidget>
#include <QKeyEvent>
#include <QToolButton>
#include "adduserprofile.h"

namespace Ui {
class UsersList;
}

class UsersList : public QWidget
{
    Q_OBJECT

public:
    explicit UsersList(QWidget *parent = 0);
    AddUserProfile* AddUserProfileWidget;
    ~UsersList();
    UserProfileDatabaseHelper *dbhelper;
    QList<QSharedPointer<UserProfileDataModel> > voucherList;

private slots:
    void keyPressEvent(QKeyEvent *e);
    void on_createNewPushButton_clicked();
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void setTable();
    void delButtonClicked(bool);

private:
    Ui::UsersList *ui;
};

#endif // USERSLIST_H
