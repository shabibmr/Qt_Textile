#ifndef PRINTOPTIONS_H
#define PRINTOPTIONS_H

#include <QWidget>
#include <QSpinBox>
#include <QDebug>

#include <QPrinter>
#include <QPrintDialog>

#include "customwidgets/Material/components/qtmaterialcheckbox.h"
#include "customwidgets/Material/components/qtmaterialradiobutton.h"
#include "customwidgets/Material/components/qtmaterialautocomplete.h"
#include "customwidgets/Material/components/qtmaterialfab.h"



namespace Ui {
class PrintOptions;
}

class PrintOptions : public QWidget
{
    Q_OBJECT

public:
    explicit PrintOptions(QWidget *parent = nullptr);
    ~PrintOptions();

    QString windowTitle="Print Options";
    QString bgcolor = "#e0f7fa";
    QString iconColor = "#00bcd4";

    QPrinter *printer;
    QtMaterialCheckBox *selectPrinter;
    QSpinBox *numCopies;
//    QtMaterialAutoComplete *printSize;

    QtMaterialRadioButton *a4Size;
    QtMaterialRadioButton *a5Size;

    QtMaterialRadioButton *thermal;

    QtMaterialRadioButton *dotMatrix;


    QtMaterialFloatingActionButton *okButton;

private slots:

    void setWidgets();
    void openPrinterDialog(bool checked);
    void setNumCopies(int value);

private:
    Ui::PrintOptions *ui;
signals:
    void printerSet(QPrinter *);

    void dotMatrixSelected();

    void printerTypeSelected(int type);
};

enum PrintTypes{
    DotMatrix,
    Thermal,
    Default
};

#endif // PRINTOPTIONS_H
