#ifndef SERVICEREGISTRATIONWIDGET_H
#define SERVICEREGISTRATIONWIDGET_H

#include <QWidget>

namespace Ui {
class ServiceRegistrationWidget;
}

class ServiceRegistrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServiceRegistrationWidget(QWidget *parent = 0);
    ~ServiceRegistrationWidget();

private:
    Ui::ServiceRegistrationWidget *ui;
};

#endif // SERVICEREGISTRATIONWIDGET_H
