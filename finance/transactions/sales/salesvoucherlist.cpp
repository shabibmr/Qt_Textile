#include "salesvoucherlist.h"
#include "ui_salesvoucherlist.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

SalesVoucherList::SalesVoucherList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesVoucherList)
{
    ui->setupUi(this);
    ui->totalAmountLineEdit->setValidator(new QDoubleValidator(0.0002,999999,2,this));
    ui->totalAmountLineEdit->setAlignment(Qt::AlignRight);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    dbhelper = new salesVoucherDatabaseHelper();
    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    //    eagerModel = new EagerlyLoaded2<QSqlQueryModel>();
    setTableView();
}

SalesVoucherList::~SalesVoucherList()
{
    delete ui;
}

void SalesVoucherList::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void SalesVoucherList::keyPressEvent(QKeyEvent *e)
{
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }
}

void SalesVoucherList::on_createNewPushButton_clicked()
{

    editor = new SalesVoucherEditor(dbhelper,this);

    editor->setObjectName("Editor");

    //    editor->setStyleSheet("QWidget#Editor {border-top-left-radius:15px;border-top-right-radius:5px;}");
    //    editor->setWindowFlags(Qt::FramelessWindowHint|Qt::Window);
    editor->setWindowFlags(Qt::Window);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));
}


void SalesVoucherList::setTableView()
{
    QDate fromDate = ui->dateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model =  dbhelper->getVoucherListByDateTimeModel(fromDateTime, toDateTime);

    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);


    if(LoginValues::Privilege>3){
        float total =0;
        for(int i=0;i<ui->tableView->model()->rowCount();i++){
            total += model->record(i).value(7).toFloat();
        }
        ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
    }
    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
}

void SalesVoucherList::on_okPushButton_clicked()
{
    setTableView();
}

void SalesVoucherList::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
}

void SalesVoucherList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"Pref : "<<voucherPrefix;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

    editor = new SalesVoucherEditor(dbhelper,this);
    editor->setWindowFlags(Qt::Window);
    editor->showVoucher(voucher);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));

}
