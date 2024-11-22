#include "ledgervouchertableview.h"

LedgervoucherTableView::LedgervoucherTableView(GeneralVoucherDataObject *obj, int Type, QTableView *parent)
    :QTableView (parent)
{
    voucher = obj;
    //    installEventFilter(this);
    lastRow = new QTableWidget(this);
    vType = Type;

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



    //    QFile file(":/style/style.qss");
    //    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    //    {
    //        sSheet= (file.readAll());
    //        file.close();

    //    }
    //    this->setStyleSheet(sSheet);

    lastRow->setStyleSheet("QTableView { border: none; border-radius :3px ;"
                           "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0.617, y2:0, stop:0 rgba(255, 255, 255, 255), stop:1 rgba(255, 255, 139, 255));"
                           "selection-background-color: #999}");


    this->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::AnyKeyPressed);
    lastRow->setFocusPolicy(Qt::NoFocus);
    lastRow->verticalHeader()->hide();
    lastRow->horizontalHeader()->hide();
    lastRow->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    lastRow->setRowHeight(0,height);
    lastRow->setRowCount(1);
    lastRow->setFrameStyle(QFrame::NoFrame);
    //    qDebug()<<"Col count : "<<this->model()->columnCount();


    updateLastRowGeometry();

    viewport()->stackUnder(lastRow);
    lastRow->show();

    connect(horizontalHeader(),&QHeaderView::sectionResized, this,
            &LedgervoucherTableView::updateSectionWidth);
}

LedgervoucherTableView::~LedgervoucherTableView(){}

void LedgervoucherTableView::setRowName(const QString &value)
{
    RowName = value;
    if(colorCount()>1){
        QTableWidgetItem *Item = new QTableWidgetItem;
        Item->setText("Grand Total");
        Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
        lastRow->setItem(0,1,Item);
    }
}

void LedgervoucherTableView::setAvlcols(const QList<int> &value)
{
    avlcols = value;
}

void LedgervoucherTableView::setColumns()
{
    qDebug()<<"At set Col Count : "<<this->model()->columnCount();
    lastRow->setColumnCount(this->model()->columnCount());

    connect(this->model(),SIGNAL(updateValues()),this,SLOT(setValues()));
    connect(this->model(),SIGNAL(newLineAdded(int)),this,SLOT(setFocusAfterInsert(int)));

    setColumnWidth(0,25);
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
    setViewportMargins(200,200,200,100);
    m = viewportMargins();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
    setValues();
}

void LedgervoucherTableView::setValues()
{
    qDebug()<<"SETTING TOTALS ROW";
    QTableWidgetItem *Item1 = new QTableWidgetItem;
    Item1->setText("Grand Total");
    Item1->setFlags(Item1->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,1,Item1);

    //    QTableWidgetItem *Item2 = new QTableWidgetItem;
    //    Item2->setText(QString::number(voucher->quantityTotal,'f',2));
    //    Item2->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

    //    Item2->setFlags(Item2->flags() ^ Qt::ItemIsEditable);
    //    lastRow->setItem(0,3,Item2);

        qDebug()<<"Cr total : "<<voucher->crTotal;
        qDebug()<<"Dr total : "<<voucher->drTotal;



    QTableWidgetItem *Item3 = new QTableWidgetItem;
    Item3->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);

    if(vType!=1)
        Item3->setText(QString::number(voucher->crTotal,'f',2));
    else
        Item3->setText(QString::number(voucher->drTotal,'f',2));

    Item3->setFlags(Item3->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,2,Item3);

    if( vType == 3 ){
        QTableWidgetItem *Item4 = new QTableWidgetItem;
        Item4->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        Item4->setText(QString::number(voucher->drTotal,'f',2));
        Item4->setFlags(Item4->flags() ^ Qt::ItemIsEditable);
        lastRow->setItem(0,3,Item4);
    }
}

void LedgervoucherTableView::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
    lastRow->setColumnWidth(logicalIndex, newSize);
    updateLastRowGeometry();
}

void LedgervoucherTableView::setFocusAfterInsert(int row)
{

    QModelIndex index =  model()->index(row, 2);
    this->setCurrentIndex(index);
}

void LedgervoucherTableView::resizeEvent(QResizeEvent *event)
{
    //    qDebug()<<"Calling resize";
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

void LedgervoucherTableView::setVoucher(GeneralVoucherDataObject *value)
{
    voucher = value;
    setValues();
}


void LedgervoucherTableView::updateLastRowGeometry()
{
    QRect rect = this->geometry();
    
    lastRow->setGeometry(rect.x()-frameWidth()-12,rect.height()-30-1,rect.width(),30);
    //    qDebug()<<lastRow->geometry();
}
