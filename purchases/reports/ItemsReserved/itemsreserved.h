#ifndef ITEMSRESERVED_H
#define ITEMSRESERVED_H

#include <QWidget>

namespace Ui {
class ItemsReserved;
}

class ItemsReserved : public QWidget
{
    Q_OBJECT

public:
    explicit ItemsReserved(QWidget *parent = 0);
    ~ItemsReserved();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::ItemsReserved *ui;
};

#endif // ITEMSRESERVED_H
