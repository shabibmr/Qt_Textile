#include "employeetableview.h"

EmployeeTableView::EmployeeTableView(AttendanceListDataModel *obj,  QTableView *parent)
    :QTableView (parent)
{
    attendanceList = obj;

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

EmployeeTableView::~EmployeeTableView(){}

void EmployeeTableView::setRowName(const QString &value)
{
    RowName = value;
}

void EmployeeTableView::setAvlcols(const QList<int> &value)
{
    avlcols = value;
}

void EmployeeTableView::setColumns()
{

    connect(this->model(),SIGNAL(updateValues()),this,SLOT(setValues()));
    connect(this->model(),SIGNAL(newLineAdded(int)),this,SLOT(setFocusAfterInsert(int)));

//    setColumnWidth(0,30);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

}


void EmployeeTableView::setFocusAfterInsert(int row)
{

    QModelIndex index =  model()->index(row, 2);
    this->setCurrentIndex(index);
}

void EmployeeTableView::setFocusToFirstRow()
{
    QModelIndex index =  model()->index(0, avlcols[0]);
    this->setCurrentIndex(index);
}

void EmployeeTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    QMargins m =   viewportMargins();

    setViewportMargins(0+ frameWidth(),m.top(),m.right(),30);
    m = viewportMargins();
    QRect rect = this->geometry();
}

void EmployeeTableView::setAttendanceList(AttendanceListDataModel *value)
{
    attendanceList = value;
}
