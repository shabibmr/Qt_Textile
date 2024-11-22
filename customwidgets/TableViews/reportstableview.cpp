#include "reportstableview.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"


ReportsTableView::ReportsTableView(QString rowName,bool show, QTableView *parent)
    : QTableView (parent)
{

    this->RowName = rowName;
    lastRow = new QTableWidget(this);
    lastRow->setFocusPolicy(Qt::NoFocus);
    this->setObjectName("EditInvTable");
    this->setContentsMargins(0,0,0,0);

    int rowHeight = ConfigurationSettingsDatabaseHelper::getValue(tableRowHeight, 30).toInt();

    qDebug()<<Q_FUNC_INFO<<__LINE__<<this->rowHeight(0);

    QHeaderView *verticalHeader = this->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(rowHeight);


    int height= this->horizontalHeader()->height();
//    qDebug()<<"V HEIGHT :"<<height;
    this->verticalHeader()->setVisible(show);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    //    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet(
                "QTableView#EditInvTable::item{"
                "gridline-color : #ffffff;"
                "border : 0px none #ffffff;"
                "border-bottom: 1px solid #ddd;"
                "}"
                "QTableView::item:selected{"
                "color:#00acc1;"
                "background-color:#e0f7fa;"
                "}"
                "QTableView#EditInvTable{"
                "background-color: #ffffff;"
                "border-right: 1px solid #ddd;"
                "border-left: 1px solid #ddd;"
                "gridline-color : #ffffff;"
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
    lastRow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lastRow->setStyleSheet("QTableWidget { border: none;"
                           "background-color: #ffffff;"
                           "color: #006064;"
                           "selection-background-color: #999}");
    lastRow->setFont(QFont("Calibri", 12, QFont::Bold));


    lastRow->setFocusPolicy(Qt::NoFocus);
    lastRow->verticalHeader()->hide();
    lastRow->horizontalHeader()->hide();
    lastRow->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    lastRow->setRowHeight(0,50);
    lastRow->setRowCount(1);
    lastRow->setColumnCount(20);
    lastRow->setFrameStyle(QFrame::NoFrame);

    updateLastRowGeometry();
    viewport()->stackUnder(lastRow);
    //    this->stackUnder(lastRow);
        lastRow->show();
    connect(horizontalHeader(),&QHeaderView::sectionResized, this,
            &ReportsTableView::updateSectionWidth);
}


void ReportsTableView::setRowName(const QString &value, const int &col, int alignment)
{

    //    qDebug()<<col;
    if(col>= 0 && col< modelQry->columnCount()){
        QTableWidgetItem *Item = new QTableWidgetItem(value);
        Item->setText(value);
        Item->setBackgroundColor(QColor(Qt::red));
        Item->setFont(QFont("Calibri", 12, QFont::Bold));
        Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
        Item->setTextAlignment(alignment);
        lastRow->setItem(0,col,Item);
        //        lastRow->update();
        //        qDebug()<<"last row set item : (73)"<<lastRow->takeItem(0,col)->text();

    }

}

void ReportsTableView::setValues()
{
    //    qDebug()<<"Tota avl size"<<avlcols.size()<<modelQry->rowCount();

    //    qDebug()<<Q_FUNC_INFO;

    for(int i=0;i<avlcols.size();i++){
        float total =0;
        for(int j=0;j<modelQry->rowCount();j++){
            QModelIndex ind = this->model()->index(j,avlcols[i]);
            //            total += modelQry->record(j).value(avlcols[i]).toFloat();
            total += this->model()->data(ind).toFloat();
            //            qDebug()<<"calculate sum of "<<avlcols[i]<<modelQry->record(j).value(avlcols[i]).toFloat()<<total;
        }

        QTableWidgetItem *Item = new QTableWidgetItem;
        Item->setText(QString::number(total,'f',2));
        Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
        Item->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
        lastRow->setItem(0,avlcols[i],Item);

        //        qDebug()<<"set last row of "<<avlcols[i];
    }
}

void ReportsTableView::setColumns()
{
    //    this->model();
    int colCount = this->model()->columnCount();
    //    qDebug()<<Q_FUNC_INFO;
    lastRow->setColumnCount(colCount);

    //    connect(model,SIGNAL(updateValues()),DataTableWidget,SLOT(setValues()));
    //    connect(model,SIGNAL(newLineAdded(int)),DataTableWidget,SLOT(setFocusAfterInsert(int)));
    //    connect(this->model(),SIGNAL(updateValues()),this,SLOT(setValues()));
    //    connect(this->model(),SIGNAL(newLineAdded(int)),this,SLOT(setFocusAfterInsert(int)));

    //    setColumnWidth(0,25);
    //    setColumnWidth(colCount-1,25);

    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    for(int i = 0;i < colCount;i++){
        lastRow->setColumnWidth(i,this->columnWidth(i));

        QTableWidgetItem *Item = new QTableWidgetItem;
        Item->setText("");

        Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
        lastRow->setItem(0,i,Item);
    }

    QTableWidgetItem *Item = new QTableWidgetItem;
    Item->setText(RowName);

    Item->setFlags(Item->flags() ^ Qt::ItemIsEditable);
    lastRow->setItem(0,2,Item);

    QMargins m =   viewportMargins();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
    setViewportMargins(200,200,200,100);
    m = viewportMargins();
    //    qDebug()<<"Bottom margin : "<<m.bottom();
    setValues();

}

void ReportsTableView::gmHideColumn(int i)
{
    this->hideColumn(i);
    lastRow->hideColumn(i);
}

void ReportsTableView::setAvlcols(const QList<int> &value)
{
    //    qDebug()<<"col for total display"<<value;
    avlcols = value;
}

void ReportsTableView::setModelQry(QSqlQueryModel *value)
{
    modelQry = value;
}


void ReportsTableView::resizeEvent(QResizeEvent *event)
{
    QTableView::resizeEvent(event);
    QMargins m =   viewportMargins();
//    qDebug()<<"margins (before): "<<m.left()<<m.top()<<m.right()<<m.bottom();

    setViewportMargins(frameWidth(),m.top(),m.right(),50);
    m = viewportMargins();
//    qDebug()<<"margins (after) : "<<m.left()<<m.top()<<m.right()<<m.bottom();
    //    QRect rect = this->geometry();
//    qDebug()<<"Bottom margin : "<<m.bottom();
    //    lastRow->setGeometry(rect.x()/*-9*/,rect.height()-25,rect.width(),25);
    updateLastRowGeometry();

}

void ReportsTableView::showVerticalHeader(bool b)
{
    this->verticalHeader()->setVisible(b);
}

void ReportsTableView::updateSectionWidth(int logicalIndex, int /* oldSize */, int newSize)
{
    lastRow->setColumnWidth(logicalIndex, newSize);
    updateLastRowGeometry();
}

void ReportsTableView::updateLastRowGeometry()
{
    QRect rect = this->geometry();

    if(this->horizontalScrollBar()->isVisible()){
        //        qDebug()<<"scroll bar on";
        lastRow->setGeometry(rect.x()-frameWidth()-9,rect.height()-26-this->horizontalScrollBar()->height(),rect.width(),26);
    }
    else {
        //        qDebug()<<"scroll bar off";
        lastRow->setGeometry(rect.x()+frameWidth(),rect.height()-50 ,rect.width(),50);
    }
    //    qDebug()<<lastRow->geometry();
}


//void ReportsTableView::setModel(QAbstractItemModel *model)
//{
//    qDebug()<<Q_FUNC_INFO;
//    modelQry = qobject_cast< QSqlQueryModel*> (model);
//    lastRow->setColumnCount(modelQry->columnCount());
//    QTableView::setModel(model);
//}
