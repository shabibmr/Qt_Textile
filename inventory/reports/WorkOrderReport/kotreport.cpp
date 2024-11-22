#include "kotreport.h"

KOTReport::KOTReport(QWidget *parent) :  QWidget(parent)
{
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#ffffff}");

    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);

    dbHelper = new WorkOrderDatabaseHelper;
    setWidgets();
    setTableView();
}

void KOTReport::setWidgets()
{
    appBar = new AppBarWidget("KOT Report");
    appBar->backButton->hide();
    appBar->shareButton->hide();

    tableView = new ReportsTableView;

    QVBoxLayout *vLay = new QVBoxLayout;
    this->setLayout(vLay);

    vLay->addWidget(appBar);
    vLay->addWidget(tableView);


}

void KOTReport::setTableView()
{
    model = dbHelper->getWorkOrdersByReqVoucherNo();

    KOTReportDelegate *delegate = new KOTReportDelegate(model);
    connect(delegate, &KOTReportDelegate::updateStatus, this, [=](int row){
       dbHelper->updateWOStatusByReqVoucherNo(model->record(row) .value(0).toString(), 170);
       setTableView();
    });

    tableView->setItemDelegate(delegate);
    tableView->setModel(model);
    tableView->lastRow->hide();


    tableView->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    tableView->setColumnWidth(5,25);
    tableView->hideColumn(0);

}
