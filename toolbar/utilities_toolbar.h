#ifndef UTILITIES_TOOLBAR_H
#define UTILITIES_TOOLBAR_H

#include <QWidget>
#include "addressbook/adressbook_display.h"
#include "Notifications/notificationslist.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "CommonUi/uicontroller.h"
#include "CommonUi/sidebar.h"

#include "customwidgets/SearchWidgets/Widgets/gmwidgetsearchlite.h"
#include "customwidgets/Material/components/qtmaterialautocomplete.h"



namespace Ui {
class Utilities_ToolBar;
}

class Utilities_ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit Utilities_ToolBar(QWidget *parent = nullptr);
    ~Utilities_ToolBar();
    AdressBook_Display *AdressBook_DisplayWidget;
    NotificationsList *notificationWidget;
    QtMaterialIconButton *dbSwitchButton;
    QtMaterialIconButton *shareButton;

//    GMWidgetSearchLite *widgetSearch;
    QSqlQueryModel *widModel;
    QCompleter *searchCompleter;
    UiSettingsDatabaseHelper *uiHelper;

    GMWidgetSearchLite *widSearch;

public slots:
    void setCompanyLabel(QString companyName);



private slots:
    void on_eventsPushButton_clicked();

    void on_contactsPushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_notificationsPushButton_clicked();

    void selectUiBySearch(QString uiName);

    void showShareOptions();
private:
    Ui::Utilities_ToolBar *ui;
    QWidget *myParent;

signals:
    void searchText(QString text);
    void uiSelected(int ui_id, bool);
    void companySwitchClicked();

    // QObject interface
public:
//    bool eventFilter(QObject *watched, QEvent *event);

};

#endif // UTILITIES_TOOLBAR_H
