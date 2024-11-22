#include "voucherprinter.h"


#define CODE128_B_START 104
#define CODE128_STOP 106


VoucherPrinter::VoucherPrinter(GeneralVoucherDataObject *v,QObject *parent) : QObject(parent)
{
    userHelper = new UserProfileDatabaseHelper;
    this->voucher = v;
    setPrinter(new QPrinter(QPrinter::ScreenResolution));
}


// get Voucher Heading HTML
// get Table heading HTML

QString VoucherPrinter::getVoucherHeadingHTML()
{
//    QString companyName = LoginValues::company;
}

