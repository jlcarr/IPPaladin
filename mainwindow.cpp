#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

#include <QFileDialog>
#include <QDir>
#include <QFileDevice>

#include <openssl/sha.h>

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

/**
 * @brief copyDirectory
 * @param source
 * @param destination
 * @param lock
 * A function for recursively copying a directory from the source into the destination.
 * The copied data may be locked from editing to preserve its integrity.
 */
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
        copyDirectory(subSource_path, subDestination_path);
        //if(lock) QFile(subDestination_path).setPermissions(QFileDevice::ReadOwner);
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

/**
 * @brief cryptoHash
 * @param data
 * @return
 * A function for computing the cryptographic hash of a function.
 * Uses OpenSSL's implementation of the SHA512 hashing algorithm.
 */
QByteArray hashFile(QString file_name)
{
    const int buffer_size = 1024*1024;
    QByteArray returnHash(SHA256_DIGEST_LENGTH, (unsigned char) 0);
    SHA256_CTX hash;
    if(!SHA256_Init(&hash)) return returnHash;

    QFile file(file_name);
    if(file.open(QIODevice::ReadOnly))
    {
        char buffer[buffer_size];

        int remainingBytes = file.size();
        int bytesRead;
        int nextReadSize = qMin(remainingBytes, buffer_size);
        while (nextReadSize > 0 && (bytesRead = file.read(buffer, nextReadSize)) > 0)
        {
            qDebug("read bytes: %s", qPrintable(QByteArray(buffer, bytesRead).toHex()));
            remainingBytes -= bytesRead;
            if(!SHA256_Update(&hash, buffer, bytesRead)) return returnHash;
            nextReadSize = qMin(remainingBytes, buffer_size);
        }
        file.close();
    }
    if(!SHA256_Final((unsigned char*)returnHash.data(), &hash)) return returnHash;
    qDebug("Hash value: %s\n", qPrintable(returnHash.toHex()));
    return returnHash;
}

void hashDirectory(QString path){
    QDir directory(path);
    if(!directory.exists()) return;

    foreach (QString subdirectory, directory.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString sub_path = path + QDir::separator() + subdirectory;
        hashDirectory(sub_path);
    }

    foreach (QString file, directory.entryList(QDir::Files)) {
        QString file_path = path + QDir::separator() + file;
        qDebug("file path: %s", qPrintable(file_path));
        hashFile(file_path);
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
    if(!file_name.isEmpty())
    {
        ui->IPBackupPathInput->setText(file_name);
        if(ui->HashBackupPathInput->text().isEmpty())
            ui->HashBackupPathInput->setText(file_name);
    }
}

void MainWindow::on_BackupButton_clicked()
{
    qDebug("Backing up %s", qPrintable(ui->IPPathInput->text()));
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

void MainWindow::on_HashButton_clicked()
{
    hashDirectory(ui->HashBackupPathInput->text());
}
