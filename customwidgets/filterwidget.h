#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QPointer>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QSqlQueryModel>
#include <QTableView>
#include <QKeyEvent>

#include "customwidgets/Basic/gmdateedit.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"

#include "customwidgets/gmreportsortfilterproxymodel.h"

class FilterWidget : public QWidget
{
    Q_OBJECT

public:
    FilterWidget(QWidget *parent = nullptr);

    QFont normalFont = QFont("Calibri", 9, QFont::Normal);
    QString textColor = "#006064";
    QString iconColor = "#00bcd4";
    QString bgcolor = "#ffffff";
    QString dateEditSS;

    QPointer<GMDateEdit> fromDateEdit;
    QPointer<GMDateEdit> toDateEdit;
    QLabel *fromDateLabel;
    QLabel *toDateLabel;
    QWidget *fromDateWidget;
    QWidget *toDateWidget;
    QHBoxLayout *filterLayout;
    QtMaterialIconButton *okButton;
    QLineEdit *filterLineEdit;
    GMReportSortFilterProxyModel* sort;



    QDate fromDate = QDate::currentDate();
    QDate toDate = QDate::currentDate();

    void showFromDate(bool flag);
    void showToDate(bool flag);

    void addFilter(QWidget *widget);

    void setSortModel(QSqlQueryModel *model, QTableView *table, QList<int> sortCols);
    int getRow(const QModelIndex &index);


private:
    void setWidgets();

signals:
    void refresh();


};

#endif // FILTERWIDGET_H
