#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

#include <QFileDialog>
#include <QDir>

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

void MainWindow::on_IPPathButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select Directory", QDir::homePath());
    qDebug("IP Path: %s", qPrintable(file_name));
    if(!file_name.isEmpty())
        ui->IPPathInput->setText(file_name);
}

void MainWindow::on_IPBackupPathButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select Directory", QDir::homePath());
    qDebug("IP Backup Path: %s", qPrintable(file_name));
    if(!file_name.isEmpty()){
        ui->IPBackupPathInput->setText(file_name);
        if(ui->HashBackupPathInput->text().isEmpty())
            ui->HashBackupPathInput->setText(file_name);
    }
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
    QString file_name = QFileDialog::getOpenFileName(this, "Select Directory", QDir::homePath());
    qDebug("Hash Save Path: %s", qPrintable(file_name));
    if(!file_name.isEmpty())
        ui->HashSavePathInput->setText(file_name);
}
