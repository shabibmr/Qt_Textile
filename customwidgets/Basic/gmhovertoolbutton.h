#ifndef GMHOVERTOOLBUTTON_H
#define GMHOVERTOOLBUTTON_H

#include <QToolButton>

class GMHoverToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit GMHoverToolButton(QString imageUrl = ":/images/Necklace.PNG", QToolButton* parent = Q_NULLPTR);
    ~GMHoverToolButton();

private:
    QString url ;
};

#endif // GMHOVERTOOLBUTTON_H
