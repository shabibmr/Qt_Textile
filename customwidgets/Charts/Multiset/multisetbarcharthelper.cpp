#include "multisetbarcharthelper.h"
#include <QSqlRecord>

MultisetBarChartHelper::MultisetBarChartHelper(QDate DateFrom,
                                               QDate DateTo,
                                               QSqlQueryModel *model1,
                                               int headingCol,
                                               int dataCol,
                                               QString title)
{
    this->fromDate=DateFrom;
    this->toDate=DateTo;
    this->model = model1;
    this->headingColumn = headingCol;
    this->dataColumn = dataCol;
    this->Title = title;

    chartView = new QChartView();
    series = new QBarSeries;

}

QChartView *MultisetBarChartHelper::setBarChart()
{

    categories<<fromDate.toString("dd-MM-yyyyy") + " to "
                + toDate.toString("dd-MM-yyyy");

    for(int i=0;i<model->rowCount();i++){
        QPointer<QBarSet> set = new QBarSet(model->record(i).value(headingColumn).toString());
        *set << model->record(i).value(dataColumn).toFloat();
        series->append(set);
    }


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(Title);
    chart->setAnimationOptions(QChart::SeriesAnimations);


    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    //    QValueAxis *axisY = new QValueAxis();
    //    axisY->setRange(0,15);
    //    chart->addAxis(axisY, Qt::AlignLeft);
    //    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    delete chartView;
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;


}
