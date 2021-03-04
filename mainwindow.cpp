#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

#include <QFileDialog>
#include <QDir>
#include <QFileDevice>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void copyDirectory(QString source, QString destination, bool lock = true)
{
    QDir sourceDirectory(source);
    if(!sourceDirectory.exists()) return;

    foreach (QString subdirectory, sourceDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString subSource_path = source + QDir::separator() + subdirectory;
        QString subDestination_path = destination + QDir::separator() + subdirectory;
        qDebug("source directory path: %s", qPrintable(subSource_path));
        qDebug("destination directory path: %s", qPrintable(subDestination_path));
        sourceDirectory.mkpath(subDestination_path);
        if(lock) copyDirectory(subSource_path, subDestination_path);
        //QFile(subDestination_path).setPermissions(QFileDevice::ReadOwner);
    }

    foreach (QString file, sourceDirectory.entryList(QDir::Files)) {
        QString sourceFile_path = source + QDir::separator() + file;
        QString destinationFile_path = destination + QDir::separator() + file;
        QFile::copy(sourceFile_path, destinationFile_path);
        if(lock) QFile(destinationFile_path).setPermissions(QFileDevice::ReadOwner);
        qDebug("source file path: %s", qPrintable(sourceFile_path));
        qDebug("destination file path: %s", qPrintable(destinationFile_path));
    }
}



void MainWindow::on_IPPathButton_clicked()
{
    QString file_name = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath());
    qDebug("IP Path: %s", qPrintable(file_name));
    if(!file_name.isEmpty())
        ui->IPPathInput->setText(file_name);
}

void MainWindow::on_IPBackupPathButton_clicked()
{
    QString file_name = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath());
    qDebug("IP Backup Path: %s", qPrintable(file_name));
    if(!file_name.isEmpty()){
        ui->IPBackupPathInput->setText(file_name);
        if(ui->HashBackupPathInput->text().isEmpty())
            ui->HashBackupPathInput->setText(file_name);
    }
}

void MainWindow::on_BackupButton_clicked()
{
    copyDirectory(ui->IPPathInput->text(), ui->IPBackupPathInput->text());
}

void MainWindow::on_HashBackupPathButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select Directory", QDir::homePath());
    qDebug("Hash Backup Path: %s", qPrintable(file_name));
    if(!file_name.isEmpty())
        ui->HashBackupPathInput->setText(file_name);
}

void MainWindow::on_HashBackupPathCopyButton_clicked()
{
    ui->HashBackupPathInput->setText(ui->IPBackupPathInput->text());
}

void MainWindow::on_HashSavePathButton_clicked()
{
    QString file_name = QFileDialog::getExistingDirectory(this, "Select Directory", QDir::homePath());
    qDebug("Hash Save Path: %s", qPrintable(file_name));
    if(!file_name.isEmpty())
        ui->HashSavePathInput->setText(file_name);
}
