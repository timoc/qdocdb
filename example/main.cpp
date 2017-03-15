#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "../qdocdb/qdocdbconnector.h"
#include "../qdocdb/qdocdbserver.h"
#include "../qdocdb/qdocdbclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QDocdbServer* dbserver = new QDocdbServer("qdocdblocal");

    qmlRegisterType<QDocdbConnector>("com.example.qdocdb.classes", 1, 0, "QDocdbConnector");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    int r = app.exec();
    delete dbserver;

    return r;
}
