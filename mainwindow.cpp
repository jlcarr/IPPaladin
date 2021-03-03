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


void MainWindow::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Select Directory", QDir::homePath());
    qDebug(qPrintable(file_name));
}
