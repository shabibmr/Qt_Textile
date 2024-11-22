#ifndef GMTABLEWIDGETLEDGER_H
#define GMTABLEWIDGETLEDGER_H

#include <QObject>
#include <QTableWidget>
#include <QPaintEvent>
#include <QPainter>
#include <datamodels/transaction/generalvoucherdataobject.h>

class GMTableWidgetLedger : public QTableWidget
{
public:
    explicit GMTableWidgetLedger(GeneralVoucherDataObject* voucher,QTableWidget *parent = nullptr);
    ~GMTableWidgetLedger();

    GeneralVoucherDataObject *voucher;


protected:
    void resizeEvent(QResizeEvent *event) override;

public :
    void footerAreaPaintEvent(QPaintEvent *event);
    int footerAreaHeight();

private slots:
    void updateFooterAreaHeight(int newBlockCount);
    void updateFooterArea(const QRect &, int);

private:
    QWidget *footerArea;
};


class FooterArea : public QWidget
{
public:
    FooterArea(GMTableWidgetLedger *editor) : QWidget(editor) {
        table = editor;
    }

    QSize sizeHint() const override {
        return QSize(table->footerAreaHeight(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        table->footerAreaPaintEvent(event);
    }

private:
    GMTableWidgetLedger *table;
};
#endif // GMTABLEWIDGETLEDGER_H


