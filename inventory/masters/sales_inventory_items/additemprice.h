#ifndef ADDITEMPRICE_H
#define ADDITEMPRICE_H

#include <QWidget>

namespace Ui {
class addItemPrice;
}

class addItemPrice : public QWidget
{
    Q_OBJECT

public:
    explicit addItemPrice(QWidget *parent = 0);
    ~addItemPrice();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::addItemPrice *ui;
};

#endif // ADDITEMPRICE_H
