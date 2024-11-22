#include "stockentrytableview.h"



StockEntryTableView::StockEntryTableView(GeneralVoucherDataObject *obj,QTableView *parent)
    : QTableView (parent)
{
    voucher = obj;

    lastRow = new QTableWidget(this);
    lastRow->setHidden(true);
    lastRow->setFocusPolicy(Qt::NoFocus);
    int height= this->horizontalHeader()->height();
    lastRow->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setObjectName("EditInvTable");
    this->setStyleSheet("QTableView::item{"
                        "gridline-color : #ffffff;"
                        "border: 0px solid #fffff8;"
                        "}"
                        "QTableView#EditInvTable{"
                        "border-bottom : 2px solid black;"
                        "background-image: url(:/images/watermark.png);"
                        "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.617, y2:0, stop:0 #e0f2f1, stop:1 #80cbc4);"
                        "background-repeat: no-repeat;"
                        "background-position: center;"
                        "selection-background-color: #f05053;"
                        "}"
                        "QHeaderView::section:horizontal"
                        "{"
                            "background-color: QLinearGradient(x1:0, y1:0, x2:0, y2:1, stop:0 #667db6, stop: 0.5 #0082c8, stop: 0.6 #0082c8, stop:1 #667db6);"
                            "color: white;"
                            "border: 1px solid #ffffff;"
                        "}"
                        );



    //    hScrollBarB = new QtMaterialScrollBar;
    //    hScrollBarB->setOrientation(Qt::Horizontal);
    //    hScrollBarB->setBackgroundColor(QColor(255,255,255,0));
    //    lastRow->setHorizontalScrollBar(hScrollBarB);

    //    hScrollBarA = new QtMaterialScrollBar;
    //    hScrollBarA->setOrientation(Qt::Horizontal);
    //    hScrollBarA->setBackgroundColor(QColor(255,255,255,0));

    //    this->horizontalScrollBar()->hide();


//    lastRow->setStyleSheet("QTableWidget { border: 1px solid grey;"
//                           "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.617, y2:0, stop:0 #e0f2f1, stop:1 #80cbc4);"
//                           "selection-background-color: #999}");

//    lastRow->setFocusPolicy(Qt::NoFocus);
//    lastRow->verticalHeader()->hide();
//    lastRow->horizontalHeader()->hide();
//    lastRow->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
//    lastRow->setRowHeight(0,50);
//    lastRow->setRowCount(1);
//    lastRow->setColumnCount(6);
//    lastRow->setFrameStyle(QFrame::NoFrame);
//    updateLastRowGeometry();
//    viewport()->stackUnder(lastRow);
//    lastRow->show();
//    connect(horizontalHeader(),&QHeaderView::sectionResized, this,
//            &StockEntryTableView::updateSectionWidth);
}


void StockEntryTableView::setRowName(const QString &value)
{
    RowName = value;
    if(colorCount()>1){
        QTableWidgetItem *Item = new QTableWidgetItem;
        Item->setText("Grand Total");
        Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
        lastRow->setItem(0,1,Item);
    }
}

void StockEntryTableView::setValues()
{

//    qDebug()<<"Last Row Set Total : "<<voucher->grandTotal;
}

void StockEntryTableView::setFocusAfterInsert(int row)
{
    QModelIndex index =  model()->index(row, 2);
    this->setCurrentIndex(index);
    this->scrollTo(index);
}

void StockEntryTableView::setFocusToFirstRow()
{
    qDebug()<<"Setting row focus to first;";
    QModelIndex index =  model()->index(0, avlcols[0]);
    this->setCurrentIndex(index);
}

void StockEntryTableView::setColumns()
{

    int colCount = this->model()->columnCount();


////    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
////    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));
    connect(this->model(),SIGNAL(updateValues()),this,SLOT(setValues()));
    connect(this->model(),SIGNAL(newLineAdded(int)),this,SLOT(setFocusAfterInsert(int)));

    setColumnWidth(0,25);
    setColumnWidth(colCount-1,25);

    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

//    for(int i = 0;i < this->model()->columnCount();i++){
//        lastRow->setColumnWidth(i,this->columnWidth(i));
//    }


//    QTableWidgetItem *Item = new QTableWidgetItem;
//    Item->setText("Grand Total");
//    Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
//    lastRow->setItem(0,1,Item);

//    QMargins m =   viewportMargins();
//    //    qDebug()<<"Bottom margin : "<<m.bottom();
//    setViewportMargins(200,200,200,100);
//    m = viewportMargins();
//    //    qDebug()<<"Bottom margin : "<<m.bottom();
//    setValues();

}

void StockEntryTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
//    QMargins m =   viewportMargins();
//    //    qDebug()<<"margins : "<<m.left()<<m.top()<<m.right()<<m.bottom();

//    setViewportMargins(0+ frameWidth(),m.top(),m.right(),30);
//    m = viewportMargins();
//    QRect rect = this->geometry();
//    //    qDebug()<<"Bottom margin : "<<m.bottom();
//    lastRow->setGeometry(rect.x(),rect.height()-30,rect.width(),30);
//    updateLastRowGeometry();

}

void StockEntryTableView::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
}

void StockEntryTableView::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{

//    lastRow->setColumnWidth(logicalIndex, newSize);
//    updateLastRowGeometry();
}


void StockEntryTableView::updateLastRowGeometry()
{
//    QRect rect = this->geometry();

//    lastRow->setGeometry(rect.x()-frameWidth()-9,rect.height()-30,rect.width(),30);
    //    qDebug()<<lastRow->geometry();
}
