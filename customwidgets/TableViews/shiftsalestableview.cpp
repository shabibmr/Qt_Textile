#include "shiftsalestableview.h"

ShiftSalesTableView::ShiftSalesTableView(ShiftDataModel *obj,  QTableView *parent)
    :QTableView (parent)
{
    qDebug()<<Q_FUNC_INFO;
    shift = obj;

       this->setStyleSheet(

                "QTableView::item{"
                "gridline-color : #ffffff;"
                "border: 0px solid #fffff8;"
                "}"
                "QTableView::item:selected{"
                "color:#00acc1;"
                "background-color:#e0f7fa;"
                "}"
                "QTableView#EditInvTable{"
                "background-color: #ffffff;"
                "color: #006064;"
                "border : none;"
                "/*selection-background-color: #999*/}"
                "QHeaderView::section:horizontal"
                "{"
                "background-color: #00bcd4;"
                "color: white;"
                "border: 1px solid #ffffff;"
                 "}"
                );

    this->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::AnyKeyPressed|QAbstractItemView::AllEditTriggers);

}

ShiftSalesTableView::~ShiftSalesTableView(){}

void ShiftSalesTableView::setRowName(const QString &value)
{
    RowName = value;
}

void ShiftSalesTableView::setAvlcols(const QList<int> &value)
{
    qDebug()<<Q_FUNC_INFO;
    avlcols = value;
}

void ShiftSalesTableView::setColumns()
{
    qDebug()<<Q_FUNC_INFO;

    connect(this->model(),SIGNAL(updateValues()),this,SLOT(setValues()));
    connect(this->model(),SIGNAL(newLineAdded(int)),this,SLOT(setFocusAfterInsert(int)));

//    setColumnWidth(0,30);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

}


void ShiftSalesTableView::setFocusAfterInsert(int row)
{

    QModelIndex index =  model()->index(row, 2);
    this->setCurrentIndex(index);
}

void ShiftSalesTableView::setFocusToFirstRow()
{
    QModelIndex index =  model()->index(0, avlcols[0]);
    this->setCurrentIndex(index);
}

void ShiftSalesTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    QMargins m =   viewportMargins();

    setViewportMargins(0+ frameWidth(),m.top(),m.right(),30);
    m = viewportMargins();
    QRect rect = this->geometry();
}

void ShiftSalesTableView::setShift(ShiftDataModel *value)
{
    shift = value;
}
