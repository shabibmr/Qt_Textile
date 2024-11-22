#ifndef MODEOFSERVICEBUTTON_H
#define MODEOFSERVICEBUTTON_H

#include <QToolButton>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

class ModeOfServiceButton : public QToolButton
{
    Q_OBJECT
public:
    explicit ModeOfServiceButton(QWidget *parent = Q_NULLPTR);

    QGraphicsDropShadowEffect* effect;

   ~ModeOfServiceButton() override;

signals:
   void rightClicked();

public slots:
    void setEffectEnable(bool v);

   // QWidget interface
protected:
   virtual void mousePressEvent(QMouseEvent *event) override;
   virtual void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // MODEOFSERVICEBUTTON_H
