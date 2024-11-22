#ifndef DUPLICATEITEMSDIALOG_H
#define DUPLICATEITEMSDIALOG_H

#include <QDialog>
#include <QLabel>
#include "customwidgets/Material/components/qtmaterialflatbutton.h"

namespace Ui {
class DuplicateItemsDialog;
}

class DuplicateItemsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DuplicateItemsDialog(QWidget *parent = nullptr);
    ~DuplicateItemsDialog();

    void addItemToLayout(QString itemId, QString itemName);
    void showDuplicateItems(QString currName, QString refName);

private:
    Ui::DuplicateItemsDialog *ui;
signals:
    void itemSelected(QString itemID);
};

#endif // DUPLICATEITEMSDIALOG_H
