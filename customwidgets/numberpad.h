#ifndef NUMBERPAD_H
#define NUMBERPAD_H

#include <QWidget>

namespace Ui {
class NumberPad;
}

class NumberPad : public QWidget
{
    Q_OBJECT

public:
    explicit NumberPad(QWidget *parent = 0);
    ~NumberPad();

    QString text ="";

private slots:
    void on_B1_clicked();

    void on_B2_clicked();

    void on_B3_clicked();

    void on_B4_clicked();

    void on_B5_clicked();

    void on_B6_clicked();

    void on_B7_clicked();

    void on_B8_clicked();

    void on_B9_clicked();

    void on_B0_clicked();

    void on_BDOT_clicked();

    void on_BDEL_clicked();

    void on_BClear_clicked();

    void on_BENTER_clicked();

    void on_B00_clicked();

    void on_BMinus_clicked();

signals:
    void clicked(QString v);
    void deleted();
    void enter();
    void clearAll();
    void minus();
    void plus();
    void dot();
private:
    Ui::NumberPad *ui;
};

#endif // NUMBERPAD_H
