#ifndef CHECKLISTPROMPTDIALOG_H
#define CHECKLISTPROMPTDIALOG_H

#include <QDialog>
#include "customwidgets/Material/components/qtmaterialflatbutton.h"
#include "customwidgets/itemchecklistwidget.h"

namespace Ui {
class ChecklistPromptDialog;
}

class ChecklistPromptDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChecklistPromptDialog(GeneralVoucherDataObject *obj, QWidget *parent = nullptr);
    ~ChecklistPromptDialog();

    GeneralVoucherDataObject *voucher;

private slots:
    void openItemCheckList();
private:
    Ui::ChecklistPromptDialog *ui;

signals:
    void saveConfirmed(bool)    ;
    void checkedItems(bool);
};

#endif // CHECKLISTPROMPTDIALOG_H
