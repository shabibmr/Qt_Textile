#ifndef SHIFTENTRYWIDGET_H
#define SHIFTENTRYWIDGET_H

#include <QWidget>
#include "counter/MainScreen/shiftvalueseditor.h"
#include "counter/MainScreen/denominations.h"
//#include "database/

#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"

namespace Ui {
class ShiftEntryWidget;
}

class ShiftEntryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShiftEntryWidget(QWidget *parent = nullptr);
    ~ShiftEntryWidget();

    ShiftDataModel *shift;
    Denominations *denominations;
    ShiftValuesEditor *shifteditor;

    RestaurantShiftDatabaseHelper *dbHelper;
    QtMaterialFloatingActionButton *saveButton;
    QtMaterialIconButton *backButton;

    void disablSave();

private slots:
    void setWidgets();
    void initExtraActions();
    void initShift();

private:
    Ui::ShiftEntryWidget *ui;

signals:
    void shiftClosingStarted(ShiftDataModel *shift);
};

#endif // SHIFTENTRYWIDGET_H
