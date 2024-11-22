#ifndef EXCEL_H
#define EXCEL_H

#include <QWidget>

namespace Ui {
class Excel;
}

class Excel : public QWidget
{
    Q_OBJECT

public:
    explicit Excel(QWidget *parent = 0);
    ~Excel();

private:
    Ui::Excel *ui;
};

#endif // EXCEL_H
