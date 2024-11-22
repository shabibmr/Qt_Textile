#ifndef CUSTOMERSCREEN_H
#define CUSTOMERSCREEN_H

#include <QWidget>
#include <utils/SettingsHelper/settingshelper.h>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "QScrollBar"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

namespace Ui {
class CustomerScreen;
}

class CustomerScreen : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerScreen(QWidget *parent = nullptr);
    ~CustomerScreen();

    QFont fnt;


    GeneralVoucherDataObject* voucher;

    void setVoucher(GeneralVoucherDataObject *value);

    SettingsHelper *settings;

public slots:

    void setTable();



    void refreshScreen();
private:
    Ui::CustomerScreen *ui;


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};



#endif // CUSTOMERSCREEN_H
