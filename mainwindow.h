#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_IPPathButton_clicked();

    void on_IPBackupPathButton_clicked();

    void on_HashBackupPathButton_clicked();

    void on_HashBackupPathCopyButton_clicked();

    void on_HashSavePathButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
