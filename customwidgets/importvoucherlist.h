#ifndef IMPORTVOUCHERLIST_H
#define IMPORTVOUCHERLIST_H

#include <QWidget>
#include <QCheckBox>

namespace Ui {
class ImportVoucherList;
}

class ImportVoucherList : public QWidget
{
    Q_OBJECT

public:
    explicit ImportVoucherList(QWidget *parent = 0);
    ~ImportVoucherList();
    QStringList vouchers;

    void setVouchers(QList<QString> vNumbers);


private:
    Ui::ImportVoucherList *ui;

signals:
    void itemsList(QStringList vNumbers);
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // IMPORTVOUCHERLIST_H
