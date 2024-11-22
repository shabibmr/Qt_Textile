#include "hashtagedit.h"



HashTagEdit::HashTagEdit(LedgerMasterDatabaseHelper *iHelper,
                         QSqlQueryModel *m,
                         LedgerMasterDataModel *led,
                         QLineEdit *parent):
    QLineEdit(parent){

    this->setPlaceholderText("Search... ");

}

HashTagEdit::~HashTagEdit()
{

}
