#ifndef INVENTORYTEMPLATEEDITOR_H
#define INVENTORYTEMPLATEEDITOR_H

#include <QWidget>

namespace Ui {
class InventorytemplateEditor;
}

class InventorytemplateEditor : public QWidget
{
    Q_OBJECT

public:
    explicit InventorytemplateEditor(QWidget *parent = nullptr);
    ~InventorytemplateEditor();

private:
    Ui::InventorytemplateEditor *ui;
};

#endif // INVENTORYTEMPLATEEDITOR_H
