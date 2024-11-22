#include "itemwiseentrytableview.h"


ItemwiseEntryTableView::ItemwiseEntryTableView(GeneralVoucherDataObject *obj,QTableView *parent)
    : QTableView (parent)
{
    voucher = obj;

    lastRow = new QTableWidget(this);
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
                        "selection-color: black;"
                        "}"
                        "QHeaderView::section:horizontal"
                        "{"
                            "background-color: QLinearGradient(x1:0, y1:0, x2:0, y2:1, stop:0 #667db6, stop: 0.5 #0082c8, stop: 0.6 #0082c8, stop:1 #667db6);"
                            "color: white;"
                            "border: 1px solid #ffffff;"
                        "}"
                        );

    this->setSelectionBehavior(SelectionBehavior::SelectItems);

    //    hScrollBarB = new QtMaterialScrollBar;
    //    hScrollBarB->setOrientation(Qt::Horizontal);
    //    hScrollBarB->setBackgroundColor(QColor(255,255,255,0));
    //    lastRow->setHorizontalScrollBar(hScrollBarB);

    //    hScrollBarA = new QtMaterialScrollBar;
    //    hScrollBarA->setOrientation(Qt::Horizontal);
    //    hScrollBarA->setBackgroundColor(QColor(255,255,255,0));

    //    this->horizontalScrollBar()->hide();


    installEventFilter(this);

    lastRow->setStyleSheet("QTableWidget { border: 1px solid grey;"
                           "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.617, y2:0, stop:0 #e0f2f1, stop:1 #80cbc4);"
                           "selection-background-color: #999}");

    lastRow->setFocusPolicy(Qt::NoFocus);
    lastRow->verticalHeader()->hide();
    lastRow->horizontalHeader()->hide();
    lastRow->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    lastRow->setRowHeight(0,50);
    lastRow->setRowCount(1);
    lastRow->setColumnCount(6);
    lastRow->setFrameStyle(QFrame::NoFrame);
    updateLastRowGeometry();
    viewport()->stackUnder(lastRow);
    lastRow->show();
    connect(horizontalHeader(),&QHeaderView::sectionResized, this,
            &ItemwiseEntryTableView::updateSectionWidth);

    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
//    this->setSelectionBehavior(QAbstractItemView::SelectRows);
}


void ItemwiseEntryTableView::setRowName(const QString &value)
{
    RowName = value;
    if(colorCount()>1){
        QTableWidgetItem *Item = new QTableWidgetItem;
        Item->setText("Grand Total");
        Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
        lastRow->setItem(0,1,Item);
    }
}

void ItemwiseEntryTableView::setValues()
{

    float cntrTotal = 0;
    float discqTotal =0;
    float qtyFull =0;
    for(int i=0;i<voucher->InventoryItems.size();i++){
         cntrTotal+=voucher->InventoryItems[i].BaseItem.requestQty;
         discqTotal +=voucher->InventoryItems[i].BaseItem.discQuantity;
         qtyFull +=voucher->InventoryItems[i].BaseItem.quantityFull;
    }

    QTableWidgetItem *Item1 = new QTableWidgetItem;
    Item1->setText("Grand Total");
    Item1->setFlags(Item1->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,2,Item1);

    QTableWidgetItem *Item2 = new QTableWidgetItem;
    Item2->setText(QString::number(qtyFull,'f',2));
    Item2->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    Item2->setFlags(Item2->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,5,Item2);

    QTableWidgetItem *Item3 = new QTableWidgetItem;
    Item3->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    Item3->setText(QString::number(discqTotal,'f',2));
    Item3->setFlags(Item3->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,8,Item3);

    QTableWidgetItem *Item5 = new QTableWidgetItem;
    Item5->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    Item5->setText(QString::number(cntrTotal,'f',2));
    Item5->setFlags(Item5->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,6,Item5);

    QTableWidgetItem *Item4 = new QTableWidgetItem;
    Item4->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    Item4->setText(QString::number(voucher->grandTotal,'f',2));
    Item4->setFlags(Item4->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,this->model()->columnCount()-2,Item4);
//    qDebug()<<"Last Row Set Total : "<<voucher->grandTotal;
}

void ItemwiseEntryTableView::setFocusAfterInsert(int row)
{
//    horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    QModelIndex index =  model()->index(row, 2);
    this->setCurrentIndex(index);
    this->scrollTo(index);
}

void ItemwiseEntryTableView::setFocusToFirstRow(int row)
{
    qDebug()<<"Setting row focus to first;";
    this->setFocus();
    QModelIndex index =  model()->index(row, avlcols[0]);
    this->setCurrentIndex(index);
}

void ItemwiseEntryTableView::setColumns()
{

    qDebug()<<Q_FUNC_INFO;
    int colCount = this->model()->columnCount();
    lastRow->setColumnCount(this->model()->columnCount());

//    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
//    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));
    connect(this->model(),SIGNAL(updateValues()),this,SLOT(setValues()));
    connect(this->model(),SIGNAL(newLineAdded(int)),this,SLOT(setFocusAfterInsert(int)));

    setColumnWidth(0,25);
    setColumnWidth(colCount-1,25);

//    this->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);


    for(int i = 0;i < this->model()->columnCount();i++){
        lastRow->setColumnWidth(i,this->columnWidth(i));
    }


    QTableWidgetItem *Item = new QTableWidgetItem;
    Item->setText("Grand Total");
    Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,2,Item);

    QMargins m =   viewportMargins();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
    setViewportMargins(200,200,200,100);
    m = viewportMargins();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
    setValues();
}

void ItemwiseEntryTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    QMargins m =   viewportMargins();
    //    qDebug()<<"margins : "<<m.left()<<m.top()<<m.right()<<m.bottom();

    setViewportMargins(0+ frameWidth(),m.top(),m.right(),30);
    m = viewportMargins();
    QRect rect = this->geometry();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
    lastRow->setGeometry(rect.x(),rect.height()-30,rect.width(),30);
    updateLastRowGeometry();

}

void ItemwiseEntryTableView::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
}

void ItemwiseEntryTableView::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{

    lastRow->setColumnWidth(logicalIndex, newSize);
    updateLastRowGeometry();
}


void ItemwiseEntryTableView::updateLastRowGeometry()
{
    QRect rect = this->geometry();

    lastRow->setGeometry(rect.x()-frameWidth()-9,rect.height()-30,rect.width(),30);
    //    qDebug()<<lastRow->geometry();
}



