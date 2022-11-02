#include "filescanner.h"


FileScanner::FileScanner(QObject *parent)
    : QObject{parent}
{

}

void FileScanner::scan(QString path)
{
    QtConcurrent::run(this,&FileScanner::performscan, path);
}

void FileScanner::performscan(QString path)
{
//    qDebug() << "scan on" << QThread::currentThread();
    QDir dir(path);
    QFileInfoList list_dirs = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs);
    QFileInfoList list_files= dir.entryInfoList(QDir::Files);
    foreach (QFileInfo file, list_files) {
        emit updated(file.filePath());
    }
    foreach(QFileInfo dir, list_dirs)
    {
        performscan(dir.filePath());
    }
    return;
}



