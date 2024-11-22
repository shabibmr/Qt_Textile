#ifndef GODOWNSELECTORWIDGET_H
#define GODOWNSELECTORWIDGET_H

#include <QWidget>
#include "customwidgets/gmcombobox.h"
#include <QComboBox>
#include <QLabel>
#include "datamodels/transaction/generalvoucherdataobject.h"
#include "customwidgets/Material/components/qtmaterialfab.h"

namespace Ui {
class GodownSelectorWidget;
}

class GodownSelectorWidget : public QWidget
{
    Q_OBJECT
    QString windowTitle="Godown";
    QPointer<GMComboBox> toGodownWidget;
    GeneralVoucherDataObject *voucher;
        QtMaterialFloatingActionButton *selectedButton;

    QString godown;


public:
    explicit GodownSelectorWidget(QWidget *parent = nullptr);
    ~GodownSelectorWidget();
    void initGlobalObjects();

    void setWidgets();


private slots:


private:
    Ui::GodownSelectorWidget *ui;

signals:
    void sectionSelected(QString);
};

#endif // GODOWNSELECTORWIDGET_H
