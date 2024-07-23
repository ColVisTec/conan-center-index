#include <QCoreApplication>
#include <QObject>
#include <QString>
#include <QTimer>
#include <QFile>

#include <QNetworkAccessManager>
#include <QSqlDatabase>
#include <qtconcurrentfilter.h>
#include <QDomText>
#include <QLibraryInfo>
#include <QDir>

#include "greeter.h"

int main(int argc, char *argv[]){
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion("1.0.0");

    QString name = argc > 0 ? argv[1] : "";
    if (name.isEmpty()) {
        name = "World";
    }

    // check for valid Qt-Plugins folder (see GitHub-Issue #23660; GitHub-PR #24193)
    QString rootPluginFolder = QLibraryInfo::path(QLibraryInfo::PluginsPath);
    QString logMessage;

    if (rootPluginFolder.isEmpty())
    {
        logMessage = "Qt-Plugins folder not found!";
        qCritical() << logMessage;
        return 1;
    }

    QDir dir (rootPluginFolder);
    QFileInfoList list = dir.entryInfoList(QDir::Dirs| QDir::NoSymLinks | QDir::NoDotAndDotDot);

    if (list.isEmpty())
    {
        logMessage = "Qt-Plugins folder is empty!";
        qCritical() << logMessage;
        return 2;
    }

    logMessage = "List of Plugin Modules: ";
    qDebug() << logMessage;
    for (auto &l : list)
    {
        qDebug() << l.baseName();
    }
    // end: check for valid Qt-Plugins folder


    Greeter* greeter = new Greeter(name, &app);
    QObject::connect(greeter, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, greeter, SLOT(run()));

    QFile f(":/resource.txt");
    if(!f.open(QIODevice::ReadOnly))
        qFatal("Could not open resource file");
    qDebug() << "Resource content:" << f.readAll();
    f.close();

    QNetworkAccessManager networkTester;

    QSqlDatabase sqlTester;

    QVector<int> v;
    v << 1 << 2 << 3 << 4;
    QtConcurrent::blockingFilter(v, [](int i)
    {
        return i % 2;
    });

    QDomText xmlTester;

    return app.exec();
}
