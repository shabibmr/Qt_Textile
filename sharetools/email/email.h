#ifndef EMAIL_H
#define EMAIL_H

#include <QWidget>

namespace Ui {
class email;
}

class email : public QWidget
{
    Q_OBJECT

public:
    explicit email(QWidget *parent = 0);
    ~email();

private:
    Ui::email *ui;
};

#endif // EMAIL_H
