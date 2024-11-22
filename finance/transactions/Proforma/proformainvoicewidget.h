#ifndef PROFORMAINVOICEWIDGET_H
#define PROFORMAINVOICEWIDGET_H

#include <QWidget>

namespace Ui {
class ProFormaInvoiceWidget;
}

class ProFormaInvoiceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProFormaInvoiceWidget(QWidget *parent = 0);
    ~ProFormaInvoiceWidget();

private:
    Ui::ProFormaInvoiceWidget *ui;
};

#endif // PROFORMAINVOICEWIDGET_H
