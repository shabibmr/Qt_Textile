#include "filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent)
{

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    setWidgets();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void FilterWidget::showFromDate(bool flag)
{
    if(flag)
        fromDateWidget->show();
    else
        fromDateWidget->hide();
}

void FilterWidget::showToDate(bool flag)
{
    if(flag)
        toDateWidget->show();
    else
        toDateWidget->hide();
}

void FilterWidget::addFilter(QWidget *widget)
{
    filterLayout->addWidget(widget);
}

void FilterWidget::setSortModel(QSqlQueryModel *model, QTableView *table, QList<int> sortCols)
{
    sort->setAvlCols(sortCols);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    table->setModel(sort);

}

int FilterWidget::getRow(const QModelIndex &index)
{
    return sort->mapToSource(index).row();

}

void FilterWidget::setWidgets()
{
    filterLayout = new QHBoxLayout;

    dateEditSS = "GMDateEdit{background-color:" + bgcolor + ";border: none; color: " + textColor + "} "
            + "QDateEdit::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
            + "QDateEdit::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }";


    qDebug()<<Q_FUNC_INFO<<__LINE__;
    fromDateEdit = new GMDateEdit(&fromDate);
    fromDateEdit->setFixedWidth(80);
    fromDateEdit->setFrame(false);
    fromDateEdit->setStyleSheet(dateEditSS);
    fromDateEdit->setFont(normalFont);

    fromDateLabel = new QLabel("From");
    fromDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");


    QVBoxLayout *fromDateLay = new QVBoxLayout;
    fromDateLay->addWidget(fromDateLabel);
    fromDateLay->addWidget(fromDateEdit);

    fromDateWidget = new QWidget;
    fromDateWidget->setLayout(fromDateLay);
    fromDateWidget->hide();
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    toDateEdit = new GMDateEdit(&toDate);
    toDateEdit->setFixedWidth(80);
    toDateEdit->setFrame(false);
    toDateEdit->setStyleSheet(dateEditSS);
    toDateEdit->setFont(normalFont);


    toDateLabel = new QLabel("To");
    toDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *toDateLay = new QVBoxLayout;
    toDateLay->addWidget(toDateLabel);
    toDateLay->addWidget(toDateEdit);

    toDateWidget = new QWidget;
    toDateWidget->setLayout(toDateLay);
    toDateWidget->hide();
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    sort = new GMReportSortFilterProxyModel();

    filterLineEdit = new QLineEdit;
    filterLineEdit->setFrame(false);
    filterLineEdit->setPlaceholderText("Search.. ");
    connect(filterLineEdit, &QLineEdit::textChanged, this, [=](){
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<filterLineEdit->text();
        QRegExp regExp(( filterLineEdit->text()),Qt::CaseInsensitive );

        sort->setFilterRegExp(regExp);
    });

    qDebug()<<Q_FUNC_INFO<<__LINE__;

    okButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "refresh"));
    okButton->setParent(this);
    okButton->setColor(QColor(iconColor));
    connect(okButton, &QtMaterialIconButton::clicked, this, [=](){
        emit refresh();
    });

    QHBoxLayout *fLayout = new QHBoxLayout;
    fLayout->setContentsMargins(0,0,0,0);
    fLayout->addWidget(fromDateWidget);
    fLayout->addWidget(toDateWidget);
    fLayout->addLayout(filterLayout);
    fLayout->addWidget(okButton);
    fLayout->addStretch(1);

    fLayout->addWidget(filterLineEdit);
    qDebug()<<Q_FUNC_INFO<<__LINE__;

    this->setLayout(fLayout);


}
