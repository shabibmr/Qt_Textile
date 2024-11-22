#ifndef TERMSCONDITIONS_H
#define TERMSCONDITIONS_H

#include <QWidget>

#include "database/finance/terms/termsdatabasehelper.h"

namespace Ui {
class TermsConditions;
}

class TermsConditions : public QWidget
{
    Q_OBJECT

public:
    explicit TermsConditions(QWidget *parent = 0);
    ~TermsConditions();

    int id=0;
    QString title;
    QString paymentTerm;
    QString Delivery;
    QString warranty;
    QString Validity;
    QString exclusion;
    QString extra;

    TermsDatabaseHelper* dbHelper;
    void disableSave();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::TermsConditions *ui;
    
public slots:
    void editTerms(int id);
};

#endif // TERMSCONDITIONS_H
