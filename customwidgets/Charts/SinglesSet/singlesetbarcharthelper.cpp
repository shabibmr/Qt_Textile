#include "singlesetbarcharthelper.h"
#include <QSqlRecord>

SingleSetBarchartHelper::SingleSetBarchartHelper(QDate DateFrom,
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


    series = new QBarSeries;

}

QChartView* SingleSetBarchartHelper::setBarChart()
{

    QStringList categories;
    series->clear();
    categories<<fromDate.toString("dd-MM-yyyy") + " to "
                + toDate.toString("dd-MM-yyyy");
    float maxval = 0;
    for(int i=0;i<model->rowCount();i++){
        qDebug()<<i;
        QBarSet* set = new QBarSet(model->record(i).value(headingColumn).toString());
        qDebug()<<"name : "<<model->record(i).value(headingColumn).toString();
        qDebug()<<"values : "<<model->record(i).value(dataColumn).toString();
        QString val = model->record(i).value(dataColumn).toString();
        val.replace("\"","");
        val.replace("\'","");

        qDebug()<<"Val = "<<val;
        qDebug()<<"Val = "<<val.toFloat();
        *set << val.toFloat();
        series->append(set);

    }

    qDebug()<<"series size : "<<series->count();
    qDebug()<<"Max Val : "<<maxval;

     qDebug()<<series->barSets()[0]->label();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(Title);
    chart->setAnimationOptions(QChart::SeriesAnimations);


    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,maxval*2);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}


