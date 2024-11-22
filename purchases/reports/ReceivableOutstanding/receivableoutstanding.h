#ifndef RECEIVABLEOUTSTANDING_H
#define RECEIVABLEOUTSTANDING_H

#include <QWidget>

namespace Ui {
class ReceivableOutstanding;
}

class ReceivableOutstanding : public QWidget
{
    Q_OBJECT

public:
    explicit ReceivableOutstanding(QWidget *parent = 0);
    ~ReceivableOutstanding();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::ReceivableOutstanding *ui;
};

#endif // RECEIVABLEOUTSTANDING_H
