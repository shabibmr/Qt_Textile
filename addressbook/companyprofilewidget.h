#ifndef COMPANYPROFILEWIDGET_H
#define COMPANYPROFILEWIDGET_H

#include <QWidget>
#include "database/addressbook/companyprofiledatabasehelper.h"
#include <QFileDialog>

namespace Ui {
class CompanyProfileWidget;
}

class CompanyProfileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompanyProfileWidget(QWidget *parent = 0);
    ~CompanyProfileWidget();
    CompanyProfileDataBaseHelper *dbHelper;
    CompanyProfileDataModel *profile;

private slots:
    void on_OKButton_clicked();

    void on_cancelButton_clicked();

    void on_uploadButton_clicked();

private:
    Ui::CompanyProfileWidget *ui;
};

#endif // COMPANYPROFILEWIDGET_H
