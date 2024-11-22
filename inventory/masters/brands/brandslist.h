#ifndef BRANDSLIST_H
#define BRANDSLIST_H

#include <QWidget>
#include <QKeyEvent>
#include "database/inventory/brands/brandsdatabasehelper.h"
#include "addbrands.h"
#include <QToolButton>

namespace Ui {
class BrandsList;
}

class BrandsList : public QWidget
{
    Q_OBJECT

public:
    explicit BrandsList(QWidget *parent = 0);
    ~BrandsList();
    AddBrands *AddBrandsWidget;
    BrandsDatabaseHelper *dbHelper;
    QString x;
    QList<BrandsDataModel *> brandsList;
public slots:
    void settable();
    void searchTextChanged(QString text);



private slots:
    void on_createNewPushButton_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void keyPressEvent(QKeyEvent *e);

    void delButtonClicked(bool);

private:
    Ui::BrandsList *ui;
};

#endif // BRANDSLIST_H
