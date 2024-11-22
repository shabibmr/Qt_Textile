#ifndef ITEMANALYSIS_H
#define ITEMANALYSIS_H

#include <QWidget>

namespace Ui {
class ItemAnalysis;
}

class ItemAnalysis : public QWidget
{
    Q_OBJECT

public:
    explicit ItemAnalysis(QWidget *parent = 0);
    ~ItemAnalysis();

private slots:
    void on_OKButton_clicked();

    void on_excelButton_clicked();

    void on_printButton_clicked();

    void on_backButton_clicked();

private:
    Ui::ItemAnalysis *ui;
};

#endif // ITEMANALYSIS_H
