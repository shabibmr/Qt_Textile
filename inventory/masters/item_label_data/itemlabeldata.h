#ifndef ITEMLABELDATA_H
#define ITEMLABELDATA_H

#include <QWidget>
#include "database/inventory/label/itemlabeldatabasehelper.h"

namespace Ui {
class ItemLabelData;
}

class ItemLabelData : public QWidget
{
    Q_OBJECT

public:
    explicit ItemLabelData(QWidget *parent = nullptr);
    ~ItemLabelData();

    ItemLabelDataModel *label;
    ItemLabelDatabaseHelper *dbHelper;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ItemLabelData *ui;
};

#endif // ITEMLABELDATA_H
