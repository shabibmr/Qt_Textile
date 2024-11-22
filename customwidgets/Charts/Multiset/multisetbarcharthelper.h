#ifndef MULTISETBARCHARTHELPER_H
#define MULTISETBARCHARTHELPER_H

#include <QtCharts>
#include <QSqlQueryModel>

class MultisetBarChartHelper
{
public:

    MultisetBarChartHelper(QDate DateFrom,QDate DateTo,QSqlQueryModel *model,
                            int headingCol,int dataColumn,QString Title);

    QChartView *setBarChart();

    QList<QBarSet*> barSets;
    QBarSeries *series;
    QStringList categories;

private:
    QDate fromDate;
    QDate toDate;
    QSqlQueryModel *model;
    int headingColumn;
    int dataColumn;
    QString Title;
    QChartView *chartView;
};

#endif // MULTISETBARCHARTHELPER_H
