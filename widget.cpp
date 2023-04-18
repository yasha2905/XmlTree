#include "widget.h"
#include "ui_widget.h"

#include <QWidget>
#include <QXmlStreamReader>
#include <QTreeWidgetItem>
#include <QTextBrowser>
#include <QStack>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    // Clear Tree widget
    ui->treeWidget->clear();

    //object for xml reading creating
    QXmlStreamReader xmlReader;

    //Lets create to show the Tree
    //Top stack element - is the current parent element
    QStack<QTreeWidgetItem*> itemStack;

    // loading text to the object
    xmlReader.addData(ui->textBrowser->toPlainText());

    //loop the whole doc
    while (!xmlReader.atEnd()){
        // open tag case
        if (xmlReader.isStartElement()){
            //lets get the name of opening tag
            QString str = xmlReader.name().toString();
            //getting object QStringList
            QStringList sl = str.split(",");
            // New tree element creating
            QTreeWidgetItem* item = new QTreeWidgetItem(sl);

            // Add the new one as the chield if the parent element exists,
            if (itemStack.count()>0){
                itemStack.top()->addChild(item);
            }
            //otherwise adding as the root element
            else {
                ui->treeWidget->addTopLevelItem(item);
            }
            // An openning tag is becoming as the parent
            itemStack.push(item);
        }
        //Close tag case
        if (xmlReader.isEndElement()){
            // if the stack is not empty going up
            if (itemStack.count()>0)
                itemStack.pop();
        }
        // reading the next unit deom the doc
        xmlReader.readNext();
    }
    //If parsing error print it
    //выводим её в консоль qDebug()
    if (xmlReader.hasError())
        qDebug() << xmlReader.errorString();
}

