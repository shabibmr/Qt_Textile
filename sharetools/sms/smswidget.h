#ifndef SMSWIDGET_H
#define SMSWIDGET_H

#include <QWidget>
#include "customwidgets/Models/gmsqlquerymodel.h"

namespace Ui {
class SMSWidget;
}

class SMSWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SMSWidget(QWidget *parent = nullptr);
    ~SMSWidget();

    GMSQLQUeryModel *model;

    void setTable();

private slots:
    void on_sendButton_clicked();

    void on_cancelBuuton_clicked();

    void on_phoneLine_textChanged(const QString &arg1);



private:
    Ui::SMSWidget *ui;
};

#endif // SMSWIDGET_H
