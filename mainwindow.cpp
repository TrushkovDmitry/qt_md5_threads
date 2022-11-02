#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::max_thread = QThreadPool::globalInstance()->maxThreadCount();
    MainWindow::FS = new FileScanner();
    connect(FS, &FileScanner::updated, this, &MainWindow::thread_md5);
    connect(this, &MainWindow::print_md5_result, this, &MainWindow::print_result);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete FS;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setDirectory("D:\\");
    QStringList directoryNames;
    if (dialog.exec())
        directoryNames = dialog.selectedFiles();
    foreach (QString directory, directoryNames) {
        ui->lineEdit->setText(directory);
        MainWindow::directory = directory;
        break;
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if (is_invalide_inputs()){
        return;
    }
    MainWindow::file_count = 0;
    MainWindow::num_processed_files = 0;
    QThreadPool::globalInstance()->setMaxThreadCount(ui->lineEdit_2->text().toInt());
    ui->textEdit->setText("Обработка директории " + MainWindow::directory + " ...");
    MainWindow::timer.start();
    FS->scan(MainWindow::directory);
}

bool MainWindow::is_invalide_inputs(){
    if (ui->lineEdit->displayText().isEmpty()){
        ui->textEdit->setText("Выберите директорию");
        return true;
    }
    if (ui->lineEdit_2->displayText().isEmpty()){
        ui->textEdit->setText("Введите количество потоков");
        return true;
    }
    int p = 0;
    QIntValidator validator(1, max_thread, this);
    QString str = ui->lineEdit_2->text();
    if (validator.validate(str, p) != QValidator::Acceptable){
        ui->textEdit->setText("Введите число от 1 до " + QString::number(max_thread));
        return true;
    }
    return false;
}

void MainWindow::print_result(QString row)
{
    ui->textEdit->append(row);
    MainWindow::num_processed_files++;
    if (num_processed_files == file_count){
        ui->textEdit->append("Обработано файлов: " + QString::number(file_count));
        ui->textEdit->append("Время вычисления md5: " + QString::number(1.0*timer.elapsed()/1000) + " сек");
    }
}

void MainWindow::thread_md5(QString file)
{
    MainWindow::file_count++;
    QtConcurrent::run(this, &MainWindow::md5print, file);
}

void MainWindow::md5print(QString file_path)
{
    QFile f(file_path);
//    qDebug() << "md5 on" << QThread::currentThread();
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Md5);
        if (hash.addData(&f)) {
            emit print_md5_result(file_path + "       " + hash.result().toHex());
            return;
        }
    }
    return;
}
