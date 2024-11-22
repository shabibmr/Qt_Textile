#ifndef VOUCHERPRINTER_H
#define VOUCHERPRINTER_H

#include <QObject>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "login/loginvalues.h"

class VoucherPrinter : public QObject
{
    Q_OBJECT
public:
    explicit VoucherPrinter(GeneralVoucherDataObject *voucher,QObject *parent = nullptr);
    QPrinter *printer;
    GeneralVoucherDataObject *voucher;
    UserProfileDatabaseHelper *userHelper;

    int borderMargins = 50;

    QString getVoucherHeadingHTML();




    void setPrinter(QPrinter *printer)
    {
        this->printer = printer;
    }



    static double mmToPixels(QPrinter& printer, int mm)
    {

        return mm * 0.039370147 * printer.resolution();
    }

    static double PixelsToMM(QPrinter& printer, float pixel)
    {
        return pixel/(0.039370147*printer.resolution());

    }

    QString toCamelCase(const QString& s)
    {
        QStringList parts = s.split(' ', QString::SkipEmptyParts);
        for (int i = 0; i < parts.size(); ++i)
        {parts[i].replace(0, 1, parts[i][0].toUpper());}

        return parts.join(" ");
    }

    QString toCapsFirstCase(const QString& str)
    {
        QString tmp = str;
        // if you want to ensure all other letters are lowercase:
        tmp = tmp.toLower();
        tmp[0] = str[0].toUpper();
        return tmp;
    }

    QString formatNumber(float numberf)
    {
        QString number = QString::number(numberf,'f',2);
        int i = number.lastIndexOf('.');
        if(i >0)
        {
            i -= 3;
            while(i > 0)
            {
                number.insert(i, ',');
                i -= 3;
            }
        }
        return number;
    }

signals:

};

#endif // VOUCHERPRINTER_H
