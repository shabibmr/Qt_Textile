#ifndef SECTION_H
#define SECTION_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>
#include <QIcon>
#include <QFont>

class Section : public QWidget {
    Q_OBJECT
private:

    QGridLayout* mainLayout;
    QToolButton* toggleButton;
    QFrame* headerLine;
    QParallelAnimationGroup* toggleAnimation;
    QScrollArea* contentArea;
    int animationDuration;

public:
    explicit Section(const QString & title = "", const int animationDuration = 100,QIcon *ico=new QIcon(""), QWidget* parent = 0);

    void setContentLayout(QLayout *contentLayout);

signals:
    void sectionClicked();
};

#endif // SECTION_H
