#include "gmtablewidgetledger.h"

GMTableWidgetLedger::GMTableWidgetLedger(GeneralVoucherDataObject* v, QTableWidget *parent) : QTableWidget(parent)
{
    voucher =v;
}

GMTableWidgetLedger::~GMTableWidgetLedger()
{

}

void GMTableWidgetLedger::resizeEvent(QResizeEvent *event)
{
    QTableWidget::resizeEvent(event);

    QRect cr = contentsRect();
    footerArea->setGeometry(QRect(cr.left(), cr.top(),cr.width(), footerAreaHeight()));


}

void GMTableWidgetLedger::footerAreaPaintEvent(QPaintEvent *event)
{
//    QPainter painter(footerArea);
//    painter.fillRect(event->rect(), Qt::lightGray);

////![extraAreaPaintEvent_0]

////![extraAreaPaintEvent_1]
////    QTextBlock block = firstVisibleBlock();

//    QTableWidget *table = new QTableWidget;

//    int blockNumber = block.blockNumber();
//    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
//    int bottom = top + (int) blockBoundingRect(block).height();
////![extraAreaPaintEvent_1]

////![extraAreaPaintEvent_2]
//    while (block.isValid() && top <= event->rect().bottom()) {
//        if (block.isVisible() && bottom >= event->rect().top()) {
//            QString number = QString::number(blockNumber + 1);
//            painter.setPen(Qt::black);
//            painter.drawText(0, top,  table->width(), fontMetrics().height(),
//                             Qt::AlignRight, number);
//        }

//        block = block.next();
//        top = bottom;
//        bottom = top + (int) blockBoundingRect(block).height();
//        ++blockNumber;
//    }
}

int GMTableWidgetLedger::footerAreaHeight()
{
    if(this->rowCount()>0){
        return this->rowHeight(0)+3;
    }
    else {
        return 0;
    }
}

void GMTableWidgetLedger::updateFooterAreaHeight(int newBlockCount)
{
    setViewportMargins(0, 0, 0, footerAreaHeight());
}

void GMTableWidgetLedger::updateFooterArea(const QRect &rect, int dy)
{
//    if (dy)
//        lineNumberArea->scroll(0, dy);
//    else
//        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

//    if (rect.contains(viewport()->rect()))
//        updateFooterAreaHeight(0);
}
