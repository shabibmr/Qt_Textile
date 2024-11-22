#ifndef GMWIDGETSEARCHLITE_H
#define GMWIDGETSEARCHLITE_H

#include <QObject>
#include <QLineEdit>

#include "customwidgets/Material/components/qtmaterialtextfield.h"
#include "customwidgets/Material/components/qtmaterialautocomplete.h"
#include "database/Settings/uisettingsdatabasehelper.h"

#include "QSqlQueryModel"
#include "gmwidgetsearchlistwidgetlite.h"


class GMWidgetSearchLite : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMWidgetSearchLite(QString text = "Search...",
                                    QLineEdit *parent=Q_NULLPTR);

    QStringList list;
    UiSettingsDatabaseHelper *widgetHelper;

    QSqlQueryModel* widModel;

    GMWidgetSearchListWidgetLite *listWidget;
    UiSettingsDataModel *widObj;

public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void itemSelected(int, QString);
    void noneSelected();
//    void itemCustomBarcodeSelected(WidgetDataModel dept);

signals:

    void SelectedWidget(UiSettingsDataModel *);
    void clearItem();
    void plusButton();
    void uiSelected(int ui_id, bool);
    void minusButton();
    void ItemSet();
    void blankenterpressed();
    void astriskpressed();
//    void SalesOrderSelectedSignal(QString vno);
private slots:
//    void SOSelectedSlot(QString Vno);
};

#endif // GMWIDGETSEARCHLITE_H
