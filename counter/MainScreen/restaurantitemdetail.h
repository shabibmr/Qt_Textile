#ifndef RESTAURANTITEMDETAIL_H
#define RESTAURANTITEMDETAIL_H

#include <QWidget>
#include <QMessageBox>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "customwidgets/Validator/myvalidator.h"
#include "customwidgets/qtbutton.h"

namespace Ui {
class RestaurantItemDetail;
}

class RestaurantItemDetail : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantItemDetail(GeneralVoucherDataObject *voucher,int row,QWidget *parent = 0);
    ~RestaurantItemDetail();

    CompoundItemDataObject *Item;

    int row;

    void setItem(CompoundItemDataObject *value);

    bool allowNegativeStock = false;

private slots:

    void on_saveButton_clicked();

    void on_cancelButton_clicked();

    void getLeftWords();

    void getRightWords();

    void on_wordButton_Clicked();

signals:
    void saveRestaurantItemDetails();
    void itemDeletedFromDetail(CompoundItemDataObject c);

private:
    Ui::RestaurantItemDetail *ui;
    GeneralVoucherDataObject *voucher;
};

#endif // RESTAURANTITEMDETAIL_H
