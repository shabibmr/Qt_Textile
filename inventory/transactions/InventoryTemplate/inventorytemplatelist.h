#ifndef INVENTORYTEMPLATELIST_H
#define INVENTORYTEMPLATELIST_H

#include <QWidget>

namespace Ui {
class InventoryTemplateList;
}

class InventoryTemplateList : public QWidget
{
    Q_OBJECT

public:
    explicit InventoryTemplateList(QWidget *parent = nullptr);
    ~InventoryTemplateList();

private:
    Ui::InventoryTemplateList *ui;
};

#endif // INVENTORYTEMPLATELIST_H
