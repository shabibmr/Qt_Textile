#ifndef BARCODEPRINTWIDGET_H
#define BARCODEPRINTWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateEdit>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include "sharetools/print/printerhelper.h"

#include "customwidgets/Material/components/qtmaterialsnackbar.h"
#include "customwidgets/Material/components/qtmaterialfab.h"


namespace Ui {
class BarcodePrintWidget;
}

class BarcodePrintWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BarcodePrintWidget(inventoryItemDataModel *invItem, QWidget *parent = nullptr);
    ~BarcodePrintWidget();

    QString bgcolor = "#ffffff";
    inventoryItemDataModel *invItem;

    QLineEdit *barcodeCopiesLineEdit;
    QDateEdit *mfdDate;
    QDateEdit *expDate;

    QCheckBox *barcodeSize;
    QCheckBox *withMFD;

    QtMaterialSnackbar *snackbar;
    QtMaterialFloatingActionButton *okButton;
    QtMaterialFloatingActionButton *cancelButton;


private slots:
    void setWidgets();
    void printBarcode();
    void closeWindow();

signals:
    void closing();


private:
    Ui::BarcodePrintWidget *ui;
};

#endif // BARCODEPRINTWIDGET_H
