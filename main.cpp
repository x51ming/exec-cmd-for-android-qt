#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/private/qandroidextras_p.h>
#include <QProcess>
#include <QStandardPaths>
#include <QFile>
#include "qlog.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("reader");
    QCoreApplication::setOrganizationName("nolva.cn");
    if (argc > 1 && strcmp(argv[1], "-service") == 0){
        QAndroidService app(argc, argv);
        auto wd = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QProcess* m_process;
        m_process = new QProcess();
        QFile f(wd+"/cmd.txt");
        QStringList envs;
        QString cmd;
        if (f.open(QIODevice::ReadOnly)) {
            for (auto &line:f.readAll().split('\n')){
                if (line[0] == '@'){
                    cmd = line.mid(1);
                } else {
                    envs << QString(line);
                }
            }
        }else{
            delete m_process;
            qDebug() << "fail to read cmd.txt";
            return 0;
        }
        m_process->setStandardOutputFile(wd+"/stdout.log", QIODevice::Append);
        m_process->setStandardErrorFile(wd+"/stderr.log", QIODevice::Append);
        m_process->setWorkingDirectory(wd);
        m_process->setEnvironment(envs);
        QString arg = cmd;
        if (!arg.startsWith("/")) arg = wd + "/" + arg;
        m_process->start("/system/bin/linker64", arg.split(" "));
        return app.exec();
    }

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/Terminal/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
