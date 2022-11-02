#ifndef FILESCANNER_H
#define FILESCANNER_H

#include <QObject>
#include <QtConcurrent>
#include <QFuture>


class FileScanner : public QObject
{
    Q_OBJECT
    void performscan(QString path);
public:
    explicit FileScanner(QObject *parent = nullptr);
    void scan(QString path);
signals:
    void updated(QString file);
private slots:
};

#endif // FILESCANNER_H
