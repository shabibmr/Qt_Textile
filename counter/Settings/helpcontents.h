#ifndef HELPCONTENTS_H
#define HELPCONTENTS_H

#include <QWidget>

namespace Ui {
class HelpContents;
}

class HelpContents : public QWidget
{
    Q_OBJECT

public:
    explicit HelpContents(QWidget *parent = 0);
    ~HelpContents();

private:
    Ui::HelpContents *ui;
};

#endif // HELPCONTENTS_H
