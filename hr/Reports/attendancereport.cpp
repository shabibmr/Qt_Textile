#include "attendancereport.h"
#include "ui_attendancereport.h"

AttendanceReport::AttendanceReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttendanceReport)
{
    ui->setupUi(this);
    qDebug()<<Q_FUNC_INFO;

    dbHelper = new AttendanceDatabaseHelper;

    ui->fromDateDateEdit_2->setDate(QDate::currentDate().addDays(- QDate::currentDate().day()));
    ui->toDateDateEdit_2->setDate(QDate::currentDate());
    setTable();
}

AttendanceReport::~AttendanceReport()
{
    delete ui;
}

void AttendanceReport::setTable()
{
    qDebug()<<Q_FUNC_INFO;
    QDate fromDate = ui->fromDateDateEdit_2->date();
    QDate toDate = ui->toDateDateEdit_2->date();

    model = dbHelper->getAttendanceReportModel(fromDate, toDate);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AttendanceReport::on_OKButton_2_clicked()
{
    qDebug()<<Q_FUNC_INFO;
    setTable();
}
