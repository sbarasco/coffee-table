#ifndef TABLETHREAD_H
#define TABLETHREAD_H

#include <QThread>
#include <QTcpSocket>


class TableThread : public QThread
{
    Q_OBJECT

private:
    quint32 getPixel(quint32 x, quint32 y, char* drawBuffer);
    quint32 getPixel(quint32 num, char* drawBuffer);
public:
    TableThread(int socketDescriptor, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);
    void data_ready(QVector<quint32> data);

private:
    int socketDescriptor;
};

#endif
