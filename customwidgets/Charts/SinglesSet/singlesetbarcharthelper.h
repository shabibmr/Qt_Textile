#ifndef SINGLESETBARCHARTHELPER_H
#define SINGLESETBARCHARTHELPER_H
#include <QtCharts>
#include <QSqlQueryModel>

class SingleSetBarchartHelper
{
public:
    SingleSetBarchartHelper(QDate DateFrom,QDate DateTo,QSqlQueryModel *model,
                            int headingCol,int dataColumn,QString Title);

    QChartView* setBarChart();

    QList<QBarSet*> barSets;
    QBarSeries *series;


private:
    QDate fromDate;
    QDate toDate;
    QSqlQueryModel *model;
    int headingColumn;
    int dataColumn;
    QString Title;

};

#endif // SINGLESETBARCHARTHELPER_H
