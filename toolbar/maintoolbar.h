#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QListWidgetItem>
#include <QWidget>
#include <QDebug>

namespace Ui {
class MainToolBar;
}

class MainToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit MainToolBar(QWidget *parent = 0);
    ~MainToolBar();

public slots:
    void itemClicked(QListWidgetItem*);

private:
    Ui::MainToolBar *ui;
signals:
    void mainItemClicked(QListWidgetItem*);
};

#endif // MAINTOOLBAR_H
