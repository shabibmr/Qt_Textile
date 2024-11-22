#ifndef ATTENDANCEREPORT_H
#define ATTENDANCEREPORT_H

#include <QWidget>
#include "database/hr/attendancedatabasehelper.h"
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

namespace Ui {
class AttendanceReport;
}

class AttendanceReport : public QWidget
{
    Q_OBJECT

public:
    explicit AttendanceReport(QWidget *parent = nullptr);
    ~AttendanceReport();

    AttendanceDatabaseHelper *dbHelper;
    QSqlQueryModel* model;
    GMItemSortFilterProxyModel* sort;

    void setTable();

private slots:
    void on_OKButton_2_clicked();

private:
    Ui::AttendanceReport *ui;
};

#endif // ATTENDANCEREPORT_H
