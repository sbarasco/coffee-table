#include "tableserver.hpp"
#include "tablethread.hpp"
#include "tablearea.hpp"
#include <QTcpSocket>
#include <iostream>

TableServer::TableServer(QObject *parent): QTcpServer(parent)
{
}

void TableServer::incomingConnection(int socketDescriptor)
{
    TableThread *thread = new TableThread(socketDescriptor, this);
    connect(thread, SIGNAL(data_ready(QVector<quint32>)), (TableArea*)parent(), SLOT(setData(QVector<quint32>)));
    thread->start();
}

