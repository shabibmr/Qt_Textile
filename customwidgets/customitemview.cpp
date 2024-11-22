#include "customitemview.h"

#include <QGridLayout>

CustomItemView::CustomItemView(QWidget *parent, inventoryItemDataModel *item):QAbstractItemView( parent )
{
    QGridLayout *layout = new QGridLayout( this->viewport() );
    NameLabel = new QLabel();
    NameLabel->setText(item->ItemName);
    layout->addWidget( NameLabel, 0, 0 );
    ModelNumberLabel = new QLabel();
    ModelNumberLabel->setText(item->ItemCode);
    layout->addWidget( ModelNumberLabel, 0, 1 );

}

