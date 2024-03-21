#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtCore/private/qandroidextras_p.h>
#include <QProcess>
#include <QStandardPaths>
#include <QFile>
#include <QQuickStyle>
#include "qlog.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("reader");
    QCoreApplication::setOrganizationName("nolva.cn");
    if (argc > 1 && strcmp(argv[1], "-service") == 0){
        QAndroidService app(argc, argv);
        auto wd = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        qDebug() << "service wd:" << wd;

        QFile f(wd+"/cmd.txt");
        QStringList envs;
        QString cmd;
        if (f.open(QIODevice::ReadOnly)) {
            for (const auto &line:f.readAll().split('\n')){
                if (line.isEmpty()) continue;
                if (line[0] == '@') cmd = line.mid(1);
                else envs << QString(line);
            }
        }else{
            qDebug() << "fail to read cmd.txt";
            return 0;
        }
        if (cmd == ""){
            qDebug() << "cmd is empty";
            return 0;
        }

        QProcess m_process;
        m_process.setStandardOutputFile(
            wd+"/stdout.log", QIODevice::Append);
        m_process.setStandardErrorFile(
            wd+"/stderr.log", QIODevice::Append);
        m_process.setWorkingDirectory(wd);
        m_process.setEnvironment(envs);
        QString arg = cmd;
        if (!arg.startsWith("/")) arg = wd + "/" + arg;
        m_process.start("/system/bin/linker64", arg.split(" "));
        int code = app.exec();
        m_process.terminate();
        m_process.waitForFinished();
        return code;
    }

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QQuickStyle::setStyle("Material");

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
