#include <QApplication>
#include <QSizePolicy>
#include <iostream>
#include "tablearea.hpp"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }
    QString port(argv[1]);
    qRegisterMetaType<QVector<quint32> >("QVector<quint32>");
    QApplication app(argc, argv);
    TableArea table;
    table.setPort(port);
    QSizePolicy qsp(QSizePolicy::Preferred,QSizePolicy::Preferred);
    qsp.setHeightForWidth(true);
    table.setSizePolicy(qsp);
    table.show();

    return app.exec();
}
