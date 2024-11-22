#ifndef COSTESTIMATES_H
#define COSTESTIMATES_H

#include <QWidget>

namespace Ui {
class CostEstimates;
}

class CostEstimates : public QWidget
{
    Q_OBJECT

public:
    explicit CostEstimates(QWidget *parent = 0);
    ~CostEstimates();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::CostEstimates *ui;
};

#endif // COSTESTIMATES_H
