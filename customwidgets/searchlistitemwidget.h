#ifndef SEARCHLISTITEMWIDGET_H
#define SEARCHLISTITEMWIDGET_H

#include <QWidget>

namespace Ui {
class SearchListItemWidget;
}

class SearchListItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchListItemWidget(QWidget *parent = 0);
    ~SearchListItemWidget();
    QString itemID;
    QFont nameFont;
    void setData(QString itemName,QString brandName,QString modelNo,QString id);
    QString getID();
    QString getItemName();
private:
    Ui::SearchListItemWidget *ui;
};

#endif // SEARCHLISTITEMWIDGET_H
