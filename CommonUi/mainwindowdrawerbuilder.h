#ifndef MAINWINDOWDRAWERBUILDER_H
#define MAINWINDOWDRAWERBUILDER_H

#include <QWidget>
#include <QSqlRecord>
#include <QScrollArea>
#include <QMenu>

#include "customwidgets/Material/components/qtmaterialdrawer.h"
#include "customwidgets/Material/components/qtmaterialtabs.h"
#include "customwidgets/Material/components/qtmaterialflatbutton.h"
#include "customwidgets/Material/components/qtmaterialraisedbutton.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"

#include "database/Settings/uisettingsdatabasehelper.h"
#include "CommonUi/uicontroller.h"
#include "customwidgets/Misc/sidebarbutton.h"


class MainWindowDrawerBuilder : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindowDrawerBuilder(QWidget *parent = nullptr);
    ~MainWindowDrawerBuilder();

    QVBoxLayout *drawerLayout;

    QString bgcolor = "#ffffff";
    QString textColor = "#006064";
    QString selectionColor = "#e0f7fa";
    QString highlightedColor = "#3f51b5" ; // "#00acc1";
    QString headerBgColor= "#00bcd4"; //#0082c8
    QString iconColor = "#00bcd4";
    QString titleColor = "#00bcd4";

    QString module="";
    QString uiType="";
    UiSettingsDatabaseHelper *dbHelper;

    void setModule(const QString &value);

    void clearLayout(QLayout *layout);

    QList<QWidget *> buttonsList;
    QMap<QString, QVBoxLayout *> layoutList;
    QVBoxLayout *lastLayout;
public slots:
    void setDrawer();
    void closeMenu(QVBoxLayout *layout);
    void openMenu(QString uiType,  QVBoxLayout *layout);
//    void closeDrawer();
//    void addUiTab();


private slots:
    void initGlobal();
    void resetWidgets();

signals:
    void uiSelected(int, bool)    ;
};

#endif // MAINWINDOWDRAWERBUILDER_H
