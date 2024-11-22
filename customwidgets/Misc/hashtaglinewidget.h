#ifndef HASHTAGLINEWIDGET_H
#define HASHTAGLINEWIDGET_H

#include <QWidget>

namespace Ui {
class HashTagLineWidget;
}

class HashTagLineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HashTagLineWidget(QWidget *parent = nullptr);
    ~HashTagLineWidget();

private:
    Ui::HashTagLineWidget *ui;
};

#endif // HASHTAGLINEWIDGET_H
