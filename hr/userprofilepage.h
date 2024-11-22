#ifndef USERPROFILEPAGE_H
#define USERPROFILEPAGE_H

#include <QWidget>

namespace Ui {
class UserProfilePage;
}

class UserProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit UserProfilePage(QWidget *parent = 0);
    ~UserProfilePage();

private:
    Ui::UserProfilePage *ui;
};

#endif // USERPROFILEPAGE_H
