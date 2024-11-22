#ifndef APPBARWIDGET_H
#define APPBARWIDGET_H

#include <QWidget>
#include <QLabel>

#include "customwidgets/Material/components/qtmaterialappbar.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"

#include "sharetools/sharetools.h"

class AppBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AppBarWidget(QString reportName, QWidget *parent = nullptr);

    QString reportName;
    QLabel *windowLabel;
    QString appBarColor = "white";
    QString titleColor = "#00bcd4";
    QString iconColor = "#00bcd4";
    QFont titleFont = QFont("Calibri", 18, QFont::Normal);

    ShareTools *shareOptionsWidget;


    QtMaterialAppBar *appBar;
    QtMaterialIconButton *shareButton;
    QtMaterialIconButton *backButton;

signals:
    void backButtonClicked();
    void shareButtonClicked();

public slots:
    void showShareOptions();
    void addExtraActions(QWidget *widget);
};

#endif // APPBARWIDGET_H
