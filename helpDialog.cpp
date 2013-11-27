#include <QDebug>
#include <QFile>
#include <QString>
#include <QTextStream>
#include "helpDialog.h"
#include "ui_helpDialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    // ui->text->loadResource(QTextDocument::HtmlResource, QUrl("media/text/help.html"));
    // qDebug() << ui->text->searchPaths();
    // ui->text->setSource(QUrl("media/text/help.html"));
    // ui->text->show();
    // file.open(QFile::ReadOnly|QFile::Text);
    // QTextStream istream = QTextStream(file);
    QString filename = ":media/text/help.html";
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        ui->text->setHtml(stream.readAll());
    } else {
        qDebug() << "could not open" << filename;
    }
    file.close();
}

HelpDialog::~HelpDialog()
{
    delete ui;
}
