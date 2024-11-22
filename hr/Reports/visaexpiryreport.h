#ifndef VISAEXPIRYREPORT_H
#define VISAEXPIRYREPORT_H

#include <QWidget>
#include "database/hr/userprofiledatabasehelper.h"
namespace Ui {
class VisaExpiryReport;
}

class VisaExpiryReport : public QWidget
{
    Q_OBJECT

public:
    explicit VisaExpiryReport(QWidget *parent = nullptr);
    ~VisaExpiryReport();
    UserProfileDatabaseHelper *dbHelper;
    QSqlQueryModel *model;
private slots:
    void on_okButton_clicked();
    void setTable();
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::VisaExpiryReport *ui;
};

#endif // VISAEXPIRYREPORT_H
