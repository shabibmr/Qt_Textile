#ifndef REQUIREMENTSLIST_H
#define REQUIREMENTSLIST_H

#include <QWidget>
#include <QKeyEvent>
#include <QDate>
#include <QToolButton>
#include <QCheckBox>
#include "projects/requirements/requirement.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "database/projects/requirementsdatabasehelper.h"
#include "sharetools/print/printerhelper.h"

namespace Ui {
class RequirementsList;
}

class RequirementsList : public QWidget
{
    Q_OBJECT

public:
    explicit RequirementsList(QWidget *parent = 0);
    ~RequirementsList();
    Requirement *RequirementWidget;
    QList<GeneralVoucherDataObject> voucherList;
    RequirementsDatabaseHelper *dbhelper;

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void setTable();
    void on_okPushButton_clicked();
    void delButtonClicked(bool);

    void on_importButton_clicked();

private:
    Ui::RequirementsList *ui;
};

#endif // REQUIREMENTSLIST_H
