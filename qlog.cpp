#include "qlog.h"
#include <QThread>
#include <QStandardPaths>
#include <QFile>
#include <QTimer>
#include <QDir>
#include "jni_utils.h"

QLog::QLog() {
    wd = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    logfile.setFileName(wd+"/stdout.log");
    if (!logfile.open(QIODevice::ReadWrite|QIODevice::Text)){
        return;
    }
    errfile.setFileName(wd+"/stderr.log");
    if (!errfile.open(QIODevice::ReadWrite|QIODevice::Text)){
        return;
    }
    logfile.seek(logfile.size());
    errfile.seek(errfile.size());
    timer.setInterval(100);
    connect(&timer, &QTimer::timeout, [this](){
        if (!logfile.atEnd())
        for (const auto &line:logfile.readAll().split('\n')){
            if (line.isEmpty()) continue;
            if (line.length() > 256) {
                setLine(line.left(256));
            } else {
                setLine(line);
            }
        }

        if (!errfile.atEnd())
            for (const auto &line:errfile.readAll().split('\n')){
                if (line.isEmpty()) continue;
                if (line.length() > 256) {
                    setLine(line.left(256));
                } else {
                    setLine(line);
                }
            }
    });
    timer.start(100);
}

QString QLog::line() const
{
    return m_line;
}

void QLog::setLine(const QString &line)
{
    m_line = line;
    emit lineChanged();
}

bool QLog::stop()
{
    stopService("org/qtproject/example/QtAndroidService");
    return true;
}

void QLog::kill()
{
    return;
}

void QLog::install(const QString &file, const QString &target)
{
    QString cached = wd + "/" + target;
    if (QFile::exists(cached)) {
        QFile::remove(cached);
    }
    if (QFile::copy(file, cached)) {
        QFile::setPermissions(cached, QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner);
        setLine("installed to "+cached);
    } else {
        setLine("failed to install to "+cached);
    }
}


void QLog::notify(const QString &title, const QString &message)
{
    ::notify(title, message);
}

void QLog::debug()
{
    QFile f(wd+"/cmd.txt");
    setLine("=====cmd.txt=====");
    if (f.open(QIODevice::ReadOnly|QIODevice::Text)) {
        for (auto &line:QTextStream(&f).readAll().split('\n')) {
            setLine(line);
        }
    } else {
        setLine("fail to open cmd.txt");
    }
    setLine("=====files=====");
    for (auto &file:QDir(wd).entryList()) {
        setLine(file);
    }
    setLine("=====finished=====");
}

void QLog::execute(
    const QString &cmdline,
    const QStringList& envs)
{
    stop();
    QFile f(wd+"/cmd.txt");
    if (f.open(QIODevice::WriteOnly|QIODevice::Truncate)) {
        QTextStream out(&f);
        out << "@" << cmdline << "\n";
        for (auto &env:envs) out << env << "\n";
    }
    startService("org/qtproject/example/QtAndroidService");
}
