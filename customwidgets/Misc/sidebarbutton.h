#ifndef SIDEBARBUTTON_H
#define SIDEBARBUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QLabel>

class SideBarButton : public QToolButton
{
    Q_OBJECT
public:

    QLabel *label;
    QLabel *blabel;
    QFont font;

    explicit SideBarButton(QString text="",QToolButton *parent=Q_NULLPTR);
    virtual ~SideBarButton() override;

    void setText(QString text);

    // QWidget interface
protected:
//    void paintEvent(QPaintEvent *event) override;
private:
    QString lblText;
    bool m_elided;
};

#endif // SIDEBARBUTTON_H
