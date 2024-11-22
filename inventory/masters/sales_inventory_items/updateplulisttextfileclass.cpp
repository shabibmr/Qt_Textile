#include "updateplulisttextfileclass.h"
#include "counter/PrinterHelper/counterprinterhelper.h"
#include "login/loginvalues.h"

UpdatePLUListTextFileClass::UpdatePLUListTextFileClass()
{
    sHelper = new SalesInventoryItemDatabaseHelper();
}

void UpdatePLUListTextFileClass::UpdateFile(QString filename)
{
    QFile file;
    qDebug()<<"Current Dir Path : "<<QDir::currentPath();
    QString PluDirectoryString = ConfigurationSettingsDatabaseHelper::getValue(PluDirectory,"D:/ACE THRASS").toString();
    QDir dir(PluDirectoryString);

    file.setFileName(dir.absoluteFilePath(filename));


    if ( file.open(QIODevice::ReadWrite) )
    {
        file.resize(0);
        QTextStream stream( &file );
        QSqlQueryModel *model =sHelper->getInventoryItemsQueryModelForScale();
        QString itemLine;
        int n=0;
        for(int i=0;i<model->rowCount();i++,n++){
            itemLine ="";
            itemLine+= model->record(i).value(4).toString()+"|";
            itemLine+= model->record(i).value(2).toString()+"|";
            itemLine+= model->record(i).value(1).toString()+"|";
            itemLine+= QString::number(model->record(i).value(3).toFloat(),'f',0)+"|";
            itemLine+= model->record(i).value(6).toString()+"\n";
            stream << itemLine;
        }
        file.close();

        QMessageBox box;
        box.setText("Update Completed with"+QString::number(n)+"records file : "+file.fileName());
        box.exec();
    }
    else{
        QMessageBox box;
        box.setText("ERROR "+file.errorString());
        box.exec();
    }
}

void UpdatePLUListTextFileClass::printPLU()
{
    QSqlQueryModel *model =sHelper->getInventoryItemsQueryModelForScale();

    QString html = "<html>";

    html += "";

    html += "<html><body><font size =\"6\">";
    html += LoginValues::company.CompanyName;
    html += "</font";
    html += "<table width =\"100%\"><tr>";
    int n=0;
    for(int i=0;i<model->rowCount();i++,n++){

        html += "<tr><td>";
        html += model->record(i).value(1).toString();
        html += "</td>";
        html += "<td>";
        html += model->record(i).value(4).toString();
        html += "</td></tr>";
    }
    html += "";

    CounterPrinterHelper printer;
    printer.printPLU(html);


}

