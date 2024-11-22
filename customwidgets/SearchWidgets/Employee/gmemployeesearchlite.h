#ifndef GMEMPLOYEESEARCHLITE_H
#define GMEMPLOYEESEARCHLITE_H

#include <QObject>
#include <QLineEdit>

#include "customwidgets/Material/components/qtmaterialtextfield.h"
#include "customwidgets/Material/components/qtmaterialautocomplete.h"
#include "datamodels/hr/userprofiledatamodel.h"
#include "database/hr/userprofiledatabasehelper.h"

#include "QSqlQueryModel"
#include "gmemployeesearchlistwidgetlite.h"


class GMEmployeeSearchLite : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMEmployeeSearchLite(UserProfileDatabaseHelper *dbHelper, QSqlQueryModel *model, QString text = "Search...",
                                    QLineEdit *parent=Q_NULLPTR);

    QStringList list;
    UserProfileDatabaseHelper *empHelper;

    QSqlQueryModel* empModel;

    GMEmployeeSearchListWidgetLite *listWidget;
    UserProfileDataModel emp;

public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void itemSelected(int, QString);
    void noneSelected();
//    void itemCustomBarcodeSelected(EmployeeDataModel dept);

signals:

    void SelectedEmpID(UserProfileDataModel);
    void SelectedGroup(QString);
    void clearItem();
    void plusButton();
    void minusButton();
    void ItemSet();
    void blankenterpressed();
    void astriskpressed();
//    void SalesOrderSelectedSignal(QString vno);
private slots:
//    void SOSelectedSlot(QString Vno);
};

#endif // GMEMPLOYEESEARCHLITE_H
