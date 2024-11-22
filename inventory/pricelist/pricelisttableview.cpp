#include "pricelisttableview.h"

PriceListTableView::PriceListTableView(PriceListDataObject *obj, QTableView *parent)
    : QTableView (parent)
{
    priceList = obj;

    lastRow = new QTableWidget(this);
    lastRow->hide();
    lastRow->setFocusPolicy(Qt::NoFocus);
    int height= this->horizontalHeader()->height();
    lastRow->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setStyleSheet("QTableView::item{"
                        "border-top : 1px solid black"
                        "border-bottom : 2px solid black"
                        "gridline-color : #fff"
                        "}");
    //    hScrollBarB = new QtMaterialScrollBar;
    //    hScrollBarB->setOrientation(Qt::Horizontal);
    //    hScrollBarB->setBackgroundColor(QColor(255,255,255,0));
    //    lastRow->setHorizontalScrollBar(hScrollBarB);

    //    hScrollBarA = new QtMaterialScrollBar;
    //    hScrollBarA->setOrientation(Qt::Horizontal);
    //    hScrollBarA->setBackgroundColor(QColor(255,255,255,0));

    //    this->horizontalScrollBar()->hide();


    lastRow->setStyleSheet("QTableWidget { border: 1px solid grey;"
                           "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.617, y2:0, stop:0 #e0f2f1, stop:1 #80cbc4);"
                           "selection-background-color: #999}");

    lastRow->setFocusPolicy(Qt::NoFocus);
    lastRow->verticalHeader()->hide();
    lastRow->horizontalHeader()->hide();
    lastRow->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    lastRow->setRowHeight(0,50);
    lastRow->setRowCount(1);
    lastRow->setColumnCount(5);
    lastRow->setFrameStyle(QFrame::NoFrame);
    updateLastRowGeometry();
//    viewport()->stackUnder(lastRow);
    connect(horizontalHeader(),&QHeaderView::sectionResized, this,
            &PriceListTableView::updateSectionWidth);
}


void PriceListTableView::setRowName(const QString &value)
{
    RowName = value;
    if(colorCount()>1){
        QTableWidgetItem *Item = new QTableWidgetItem;
        Item->setText("Grand Total");
        Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
        lastRow->setItem(0,1,Item);
    }
}

void PriceListTableView::setValues()
{
    QTableWidgetItem *Item1 = new QTableWidgetItem;
    Item1->setText("Grand Total");
    Item1->setFlags(Item1->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,1,Item1);

}

void PriceListTableView::setFocusAfterInsert(int row)
{
    QModelIndex index =  model()->index(row, 2);
    this->setCurrentIndex(index);
}

void PriceListTableView::setColumns()
{

    int colCount = this->model()->columnCount();
    qDebug()<<"Col Count at 78 = "<<colCount;
    lastRow->setColumnCount(this->model()->columnCount());

//    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
//    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));
//    connect(this->model(),SIGNAL(updateValues()),this,SLOT(setValues()));
    connect(this->model(),SIGNAL(newLineAdded(int)),this,SLOT(setFocusAfterInsert(int)));

    setColumnWidth(0,25);
    setColumnWidth(colCount-1,25);

    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    for(int i = 0;i < this->model()->columnCount();i++){
        lastRow->setColumnWidth(i,this->columnWidth(i));
    }


    QTableWidgetItem *Item = new QTableWidgetItem;
    Item->setText("Grand Total");
    Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,1,Item);

    QMargins m =   viewportMargins();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
//    setViewportMargins(200,200,200,100);
    m = viewportMargins();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
    setValues();
}

void PriceListTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    QMargins m =   viewportMargins();
    //    qDebug()<<"margins : "<<m.left()<<m.top()<<m.right()<<m.bottom();

//    setViewportMargins(0+ frameWidth(),m.top(),m.right(),30);
    m = viewportMargins();
    QRect rect = this->geometry();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
//    lastRow->setGeometry(rect.x(),rect.height()-30,rect.width(),30);
    updateLastRowGeometry();

}

void PriceListTableView::setPriceList(PriceListDataObject *value)
{
    priceList = value;
}

void PriceListTableView::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
    return;
    lastRow->setColumnWidth(logicalIndex, newSize);
    updateLastRowGeometry();
}


void PriceListTableView::updateLastRowGeometry()
{
    return;
    QRect rect = this->geometry();

    lastRow->setGeometry(rect.x()-frameWidth()-12,rect.height()-30,rect.width(),30);
    //    qDebug()<<lastRow->geometry();
}


