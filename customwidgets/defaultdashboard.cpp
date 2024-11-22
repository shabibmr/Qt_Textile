#include "defaultdashboard.h"
#include "ui_defaultdashboard.h"

#include "customwidgets/Delegates/reportcommondelegate.h"



DefaultDashBoard::DefaultDashBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefaultDashBoard)
{
    ui->setupUi(this);

    this->setObjectName("Background");
    QString styleStr = "QWidget#Background{background-color: qlineargradient(spread:pad, x1:0.687, y1:0.875, x2:0.99, y2:0.965909,"
                       " stop:0.728395 rgba(174, 211, 255, 255), stop:1 rgba(255, 255, 255, 255));"
                       "border-bottom-color: rgb(67, 167, 167);}";

    this->setStyleSheet("QWidget#Background{background-color:rgba(174, 211, 255, 255)}");
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    setBarchart();

}

DefaultDashBoard::~DefaultDashBoard()
{
    delete ui;
}


void DefaultDashBoard::on_toolButton_clicked()
{
    setBarchart();
}




void DefaultDashBoard::setPieChart()
{
    QSqlQuery q1;
    QSqlQuery q2;
    //    if ( ui->lay2 != NULL )
    //    {
    //        QLayoutItem* item;
    //        while ( ( item = ui->lay2->takeAt( 0 ) ) != NULL )
    //        {
    //            delete item->widget();
    //            delete item;
    //        }
    //        //delete m_view->layout();
    //    }

    DrilldownChart *chart1 = new DrilldownChart();
    chart1->setTheme(QChart::ChartThemeLight);
    chart1->setAnimationOptions(QChart::AllAnimations);
    chart1->legend()->setVisible(true);
    chart1->legend()->setAlignment(Qt::AlignRight);

    QPieSeries *yearSeries = new QPieSeries(this);
    yearSeries->setName("Sales by year - All");
    QMap<QString,QString> map;
    QList<QString> months;
    months << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun" << "Jul" << "Aug" << "Sep" << "Oct" << "Nov" << "Dec";
    QList<QString> names;
    //names << "Jane" << "John" << "Axel" << "Mary" << "Susan" << "Bob";
    QList<QString> gpids;


    q1.exec("SELECT _ID,groupName from sales_inventory_group where sl_no>0");
    while(q1.next()){
        map.insert(q1.value(0).toString(),q1.value(1).toString());
        names<<q1.value(1).toString();
    }



    foreach (QString name, names) {
        QPieSeries *series = new QPieSeries(this);
        series->setName("Sales by month - " + name);

        foreach (QString month, months)
            *series << new DrilldownSlice(qrand() % 1000, month, yearSeries);

        QObject::connect(series, SIGNAL(clicked(QPieSlice*)), chart1, SLOT(handleSliceClicked(QPieSlice*)));

        *yearSeries << new DrilldownSlice(series->sum(), name, series);
    }

    //   QObject::connect(yearSeries, SIGNAL(clicked(QPieSlice*)), chart1, SLOT(handleSliceClicked(QPieSlice*)));

    chart1->changeSeries(yearSeries);



    QChartView *chartView1 = new QChartView(chart1);
    chartView1->setRenderHint(QPainter::Antialiasing);

    //    ui->lay2->addWidget(chartView1);



}

void DefaultDashBoard::setBarchart()
{

    if ( ui->v8 != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->v8->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        //delete m_view->layout();
    }

    //    QDate fDate = ui->fromDateEdit->date();
    //    QDate tDate = ui->toDateEdit->date();

    //    QString fromDate = fDate.toString("yyyy-MM-dd");
    //    QString toDate = tDate.toString("yyyy-MM-dd");

    //    QString whereClause = " where `voucher_date` between '"+fromDate+"' and  '"+toDate+"' ";

    QBarSet *set0 = new QBarSet("Sales");
    QBarSet *set1 = new QBarSet("Purchase");
    QBarSet *set2 = new QBarSet("Payment");
    QBarSet *set3 = new QBarSet("Receipts");

    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));



    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);
    series->append(set3);


    //    series->append(set3);
    //    series->append(set4);



    QStringList categories;

    ShiftDataModel *shift = new ShiftDataModel;

    shift->voucherPrefix="A";

    ShiftDatabaseHelper *shiftHelper =new ShiftDatabaseHelper;

    shift->ShiftStartDateTime = fromDateTime;
    shift->ShiftEndDateTime = toDateTime;

    shift->BillsFrom = shiftHelper->getFirstBillByDate(shift);
    shift->BillsTo = shiftHelper->getLastBillByDate(shift);

    shift->SalesTotal = shiftHelper->getSalesTotal(shift);
    shift->PurchaseTotal = shiftHelper->getPurchaseTotal(shift);
    shift->PaymentsTotal = shiftHelper->getPaymentTotal(shift);
    shift->ReceiptTotal = shiftHelper->getReceiptTotal(shift);


    *set0<<shift->SalesTotal;
    *set1<<shift->PurchaseTotal;
    *set2<<shift->PaymentsTotal;
    *set3<<shift->ReceiptTotal;

    categories<<shift->ShiftStartDateTime.date().toString("dd-MM-yyyy") + " to "
                + shift->ShiftEndDateTime.date().toString("dd-MM-yyyy");

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Sales-Purchases-Payments-Receipts");
    chart->setAnimationOptions(QChart::SeriesAnimations);


    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);

    chart->createDefaultAxes();
    chart->setAxisX(axis, series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *lay = new QVBoxLayout(ui->widget_3);
    //    ui->lay2->addWidget(chartView);
    ui->v8->addWidget(chartView);


    LedgerHelper *ledHelper = new LedgerHelper;

//    ui->tableView->setModel(ledHelper->getLedgerTransValueByFrequency(GMVoucherTypes::SalesVoucher,
//                                                                      fromDate, toDate));
//    ui->tableView_2->setModel(ledHelper->getLedgerTransValueByFrequency(GMVoucherTypes::PurchaseVoucher,fromDate, toDate));

//    ui->tableView_3->setModel(ledHelper->getLedgerTransValueByFrequency(GMVoucherTypes::ReceiptVoucher,
//                                                                        fromDate, toDate));

//    ui->tableView_4->setModel(ledHelper->getLedgerTransValueByFrequency(GMVoucherTypes::PaymentVoucher,
//                                                                        fromDate, toDate));

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    LedgerHelperMT *lHelper1 = new LedgerHelperMT();
    LedgerHelperMT *lHelper2 = new LedgerHelperMT();
    LedgerHelperMT *lHelper3 = new LedgerHelperMT();
    LedgerHelperMT *lHelper4 = new LedgerHelperMT();

    connect(lHelper1,&LedgerHelperMT::readComplete,this,&DefaultDashBoard::setSalesModel,Qt::QueuedConnection);
    connect(lHelper2,&LedgerHelperMT::readComplete,this,&DefaultDashBoard::setPurchaseTable,Qt::QueuedConnection);
    connect(lHelper3,&LedgerHelperMT::readComplete,this,&DefaultDashBoard::setRecTable,Qt::QueuedConnection);
    connect(lHelper4,&LedgerHelperMT::readComplete,this,&DefaultDashBoard::setPaytable,Qt::QueuedConnection);

    QFuture<void> t1 = QtConcurrent::run(lHelper1,&LedgerHelperMT::getLedgerTransValueByFrequency,GMVoucherTypes::SalesVoucher,fromDate, toDate,QString("%"));
    QFuture<void> t2 = QtConcurrent::run(lHelper2,&LedgerHelperMT::getLedgerTransValueByFrequency,GMVoucherTypes::PurchaseVoucher,fromDate, toDate,QString("%"));
    QFuture<void> t3 = QtConcurrent::run(lHelper3,&LedgerHelperMT::getLedgerTransValueByFrequency,GMVoucherTypes::ReceiptVoucher,fromDate, toDate,QString("%"));
    QFuture<void> t4 = QtConcurrent::run(lHelper4,&LedgerHelperMT::getLedgerTransValueByFrequency,GMVoucherTypes::PaymentVoucher,fromDate, toDate,QString("%"));

    ui->tableView->setItemDelegate(new ReportCommonDelegate);
    ui->tableView_2->setItemDelegate(new ReportCommonDelegate);
    ui->tableView_3->setItemDelegate(new ReportCommonDelegate);
    ui->tableView_4->setItemDelegate(new ReportCommonDelegate);

    ui->tableView->verticalHeader()->hide();
    ui->tableView_2->verticalHeader()->hide();
    ui->tableView_3->verticalHeader()->hide();
    ui->tableView_4->verticalHeader()->hide();
}


void DefaultDashBoard::setSalesModel(QSqlQueryModel *model)
{
    ui->tableView->setModel(model);
}

void DefaultDashBoard::setPurchaseTable(QSqlQueryModel *model)
{
    ui->tableView_2->setModel(model);
}

void DefaultDashBoard::setRecTable(QSqlQueryModel *model)
{
    ui->tableView_3->setModel(model);
}

void DefaultDashBoard::setPaytable(QSqlQueryModel *model)
{
    ui->tableView_4->setModel(model);
}
