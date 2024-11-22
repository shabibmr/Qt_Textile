#ifndef SHIPPINGDETAILSEDITOR_H
#define SHIPPINGDETAILSEDITOR_H

#include <QWidget>
#include <QLabel>
#include <QDesktopWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QScreen>
#include <QHeaderView>

#include "customwidgets/Basic/Material/gmmateriallineedit.h"
#include "customwidgets/Basic/Material/gmmaterialcombobox.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"

#include "customwidgets/Material/components/qtmaterialraisedbutton.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/qtmaterialbadge.h"
#include "customwidgets/Basic/Material/gmmaterialnumberedit.h"

#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "database/finance/salesvoucher/salesvoucherdatabasehelper.h"
#include "datamodels/addressbook/addressbookdatamodel.h"
#include "customwidgets/Misc/customshadoweffect.h"


namespace Ui {
class ShippingDetailsEditor;
}

class ShippingDetailsEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ShippingDetailsEditor(GeneralVoucherDataObject *voucher, QWidget *parent = nullptr);

    GeneralVoucherDataObject *voucher;
    AddressBookDataModel *customer;

    QFont boldFont = QFont("Roboto", 11, QFont::Bold);
    QFont subTextFont = QFont("Roboto", 9, QFont::Normal);
    QString darkColor = "#1a3966";
    QString lightColor = "#cd9d7d";


    QtMaterialAppBar *appBar;
    QtMaterialIconButton *backButton;
    QtMaterialIconButton *Notifications;
    QtMaterialFlatButton *Cart;
    QtMaterialFlatButton *Store;


    GMMaterialLineEdit *email;
    GMMaterialLineEdit *firstName;
    GMMaterialLineEdit *lastName;
    GMMaterialLineEdit *address;
    GMMaterialLineEdit *city;
    GMMaterialLineEdit *country;
    GMMaterialLineEdit *region;
    GMMaterialLineEdit *postalCode;
    GMMaterialLineEdit *phone;
    GMMaterialNumberEdit *total;

    QtMaterialFlatButton *continueButton;

    QListWidget *itemsList;
    QTableWidget *summaryWidget;




    ~ShippingDetailsEditor();
private slots:
    void setWidgets();
    void closeWindow();
    void displayItems();
    void setSummaryTable();
private:
    Ui::ShippingDetailsEditor *ui;
};

#endif // SHIPPINGDETAILSEDITOR_H
