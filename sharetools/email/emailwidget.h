#ifndef EMAILWIDGET_H
#define EMAILWIDGET_H

#include <QWidget>
#include <datamodels/utils/emailmodel.h>

namespace Ui {
class EmailWidget;
}

class EmailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmailWidget(QWidget *parent = nullptr);
    ~EmailWidget();

    EmailModel *emailData;

    void setWidgets();

    void initData();

    void setData();

public slots:

    void sendMail();

private:
    Ui::EmailWidget *ui;
};

#endif // EMAILWIDGET_H
