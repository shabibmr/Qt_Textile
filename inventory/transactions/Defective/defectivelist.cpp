#include "defectivelist.h"
#include "ui_defectivelist.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
DefectiveList::DefectiveList( bool hideCreate, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DefectiveList)
{
    ui->setupUi(this);

//    ui->createNew->setHidden(hideCreate);
    ui->fromDate->setDate(QDate::currentDate());

    ui->toDate->setDate(QDate::currentDate());

    dbHelper = new DefectiveDatabaseHelper();

    ui->tableView->setItemDelegate(new ReportCommonDelegate());
    setTableView();


}

DefectiveList::~DefectiveList()
{
    delete ui;
}


void DefectiveList::on_okButton_clicked()
{
    setTableView();
}

void DefectiveList::setTableView()
{
    QDate fromDate = ui->fromDate->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDate->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbHelper->getVoucherListByDateModel(fromDate,toDate);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    float total =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        total+= model->record(i).value(6).toFloat();
    }
//    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
//    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

}

void DefectiveList::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    GeneralVoucherDataObject *voucher = dbHelper->getVoucherByVoucherNoAsPtr(voucherNo, voucherPrefix);

//    addDefectiveVoucherWidget = new addDefectiveVoucher(this);
//    addDefectiveVoucherWidget->setWindowFlags(Qt::Window);
//    addDefectiveVoucherWidget->showVoucher(voucher);
//    addDefectiveVoucherWidget->showMaximized();
//    QObject::connect(addDefectiveVoucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));

    editor = new WastageEntryEditor(this);
    editor->setWindowFlags(Qt::Window);
    editor->showVoucher(voucher);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));



}

void DefectiveList::on_createNew_clicked()
{
//    addDefectiveVoucherWidget = new addDefectiveVoucher(this);
//    addDefectiveVoucherWidget->setWindowFlags(Qt::Window);
//    addDefectiveVoucherWidget->showMaximized();
//    QObject::connect(addDefectiveVoucherWidget,SIGNAL(closing()),this,SLOT(setTableView()));

    editor = new WastageEntryEditor(this);
    editor->setWindowFlags(Qt::Window);
    editor->showMaximized();
    QObject::connect(editor,SIGNAL(closing()),this,SLOT(setTableView()));


}


