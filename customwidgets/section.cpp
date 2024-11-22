#include <QPropertyAnimation>

#include<QDebug>
#include "Section.h"

Section::Section(const QString & title, const int animationDuration,QIcon *myicon, QWidget* parent)
    : QWidget(parent), animationDuration(animationDuration)
{

    toggleButton = new QToolButton(this);
    headerLine = new QFrame(this);
    toggleAnimation = new QParallelAnimationGroup(this);
    contentArea = new QScrollArea(this);
    mainLayout = new QGridLayout(this);

    toggleButton->setStyleSheet("QToolButton {border: none;color:#9FFDA1;}");
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    //toggleButton->setArrowType(Qt::ArrowType::RightArrow);
    toggleButton->setText(title);

    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);
    toggleButton->setIcon(*myicon);
    QFont font =toggleButton->font();
    font.setPointSize(11);
    toggleButton->setFont(font);
    headerLine->setFrameShape(QFrame::NoFrame);
    headerLine->setFrameShadow(QFrame::Sunken);
    headerLine->setLineWidth(0);

    headerLine->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // start out collapsed
    contentArea->setMaximumHeight(0);
    contentArea->setMinimumHeight(0);
    contentArea->setFrameShape(QFrame::NoFrame);



    // let the entire widget grow and shrink with its content
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));

    mainLayout->setVerticalSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    int row = 0;
    mainLayout->addWidget(toggleButton, row, 0, 1, 1, Qt::AlignLeft);
    mainLayout->addWidget(headerLine, row++, 2, 1, 1);
    mainLayout->addWidget(contentArea, row, 0, 1, 3);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    QObject::connect(toggleButton, &QToolButton::clicked, [this](const bool checked)
    {
       // toggleButton->setArrowType(checked ? Qt::ArrowType::DownArrow : Qt::ArrowType::RightArrow);
        toggleAnimation->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        toggleAnimation->start();
        emit sectionClicked();

    });
}

void Section::setContentLayout(QLayout *contentLayout)
{
    delete contentArea->layout();
    contentArea->setLayout(contentLayout);
    const auto collapsedHeight = sizeHint().height() - contentArea->maximumHeight();
    auto contentHeight = contentLayout->sizeHint().height();

    // to be removed - for debug purpose only

    if (contentArea->layout()) {
        while(contentArea->layout()->count() > 0){
            QLayoutItem *item = contentArea->layout()->takeAt(0);
            QWidget* widget = item->widget();
            if(widget){
                qDebug()<<Q_FUNC_INFO<<__LINE__<<"found widget"<<widget;
            }

        }
    }

        // remove till here

    for (int i = 0; i < toggleAnimation->animationCount() - 1; ++i)
    {
        QPropertyAnimation* SectionAnimation = static_cast<QPropertyAnimation *>(toggleAnimation->animationAt(i));
        SectionAnimation->setDuration(animationDuration);
        SectionAnimation->setStartValue(collapsedHeight);
        SectionAnimation->setEndValue(collapsedHeight + contentHeight);
    }

    QPropertyAnimation* contentAnimation = static_cast<QPropertyAnimation *>(toggleAnimation->animationAt(toggleAnimation->animationCount() - 1));
    contentAnimation->setDuration(animationDuration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);
}
