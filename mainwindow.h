#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QIntValidator>
#include <QCryptographicHash>
#include <QElapsedTimer>
#include "filescanner.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void print_md5_result(QString row);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void print_result(QString row);

private:
    Ui::MainWindow *ui;
    QString directory;
    int max_thread;
    int file_count;
    int num_processed_files;
    QElapsedTimer timer;
    FileScanner *FS;
    void md5print(QString file_path);
    void thread_md5(QString file);
    bool is_invalide_inputs();

};
#endif // MAINWINDOW_H
