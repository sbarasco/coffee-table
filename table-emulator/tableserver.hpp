#ifndef TABLESERVER_H
#define TABLESERVER_H

#include <QThread>
#include <QTcpServer>
#include <QVector>
#include "constants.h"


class TableServer : public QTcpServer
 {
     Q_OBJECT
 public:
     TableServer(QObject *parent);
 protected:
     void incomingConnection(int socketDescriptor);
 };

#endif
