#ifndef GMCARDWIDGET_H
#define GMCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

#include "customwidgets/Material/components/qtmaterialfab.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"

namespace Ui {
class GMCardWidget;
}

class GMCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GMCardWidget(QString Label="",
                          QString description="",
                          QString Data="",
                          bool showViewButton=true,
                          QString bgColor = "#e0f7fa",
                          QString textColor = "#006064",
                          QString iconColor = "#00bcd4",
                          QWidget *parent = nullptr);
    ~GMCardWidget();

    QString label;
    QString description;
    QString data;

    QLabel *title;
    QLabel *descLabel;
    QWidget *dataWid;
    QVBoxLayout* dataLayout;

    bool showViewButton=true;
    QtMaterialIconButton *expandButton;

    QString bgColor;
    QString textColor="#FFFFFF";
    QString iconColor;

//    void setData(const QString &value);
    void setDataWidget(QWidget *wid);

private slots:
    void setWidgets();
    void createWidgets();

signals:
    void clicked();
private:
    Ui::GMCardWidget *ui;
};

#endif // GMCARDWIDGET_H
