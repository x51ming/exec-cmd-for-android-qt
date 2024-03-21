#include "qlog.h"
#include <QThread>
#include <QStandardPaths>
#include <QFile>
#include "jni_utils.h"





QLog::QLog() {
    wd = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation);
    m_pid = 0;
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
    if (m_process==nullptr) return true;
    m_process->terminate();
    m_process->waitForFinished();
    if (m_process->state() != QProcess::NotRunning) {
        setLine("fail to stop last process");
        return false;
    }
    // setLine(QString("exit code: ")+QString::number(m_process->exitCode()));
    // setLine(QString("exit err: ")+m_process->errorString());
    delete m_process; releaseWakeLock();
    m_process = nullptr;
    notify("Qt", "stopped");
    return true;
}

void QLog::kill()
{
    if (m_process==nullptr) return;
    m_process->kill();
    m_process->waitForFinished();
    // setLine(QString("exit code: ")+QString::number(m_process->exitCode()));
    // setLine(QString("exit err: ")+m_process->errorString());
    delete m_process; releaseWakeLock();
    m_process = nullptr;
    notify("Qt", "killed");
}

void QLog::install(const QString &file, const QString &target)
{
    QString cached = wd + "/" + target;
    if (QFile::exists(cached)) {
        QFile::remove(cached);
    }
    if (QFile::copy(file, cached)) {
        QFile::setPermissions(cached, QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner);
        setLine(QString("installed to ")+cached);
    } else {
        setLine("failed to install to "+cached);
    }
}



void QLog::notify(const QString &title, const QString &message)
{
    QJniObject javaNotification = QJniObject::fromString(message);
    QJniObject javaTitle = QJniObject::fromString(title);
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/QNotification",
        "notify",
        "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)V",
        QNativeInterface::QAndroidApplication::context(),
        javaTitle.object<jstring>(),
        javaNotification.object<jstring>());
}

void QLog::connectProcess()
{
    QObject::connect(m_process, &QProcess::readyReadStandardOutput, [this](){
        auto lines = m_process->readAllStandardOutput();
        for (const auto& line : lines.split('\n')) {
            if (line.length() == 0) continue;
            if (line.length() > 256)
                setLine(line.left(256));
            else
                setLine(line);
        }
    });
    QObject::connect(m_process, &QProcess::readyReadStandardError, [this](){
        auto lines = m_process->readAllStandardError();
        for (const auto& line : lines.split('\n')) {
            if (line.length() == 0) continue;
            if (line.length() > 256)
                setLine(line.left(256));
            else
                setLine(line);
        }
    });
    QObject::connect(m_process, &QProcess::finished, [this](){
        setLine(QString("FINISHED. pid:")
                +QString::number(m_process->processId())
                +QString("; exit code:")
                +QString::number(m_process->exitCode()));
        notify("Qt", "finished");
    });
}

void QLog::execute(
    const QString &cmdline,
    const QStringList& envs)
{
    // execute the command
    // and set the result to m_line

    if (!stop()) {
        setLine("fail to stop last process");
        return;
    }
    QStringList args = cmdline.split(" ");
    m_process = new QProcess();
    acquireWakeLock();
    connectProcess();
    m_process->setEnvironment(envs);
    m_process->setProcessChannelMode(
        QProcess::MergedChannels);
    m_process->setWorkingDirectory(wd);
    if (!args[0].startsWith("/")){
        args[0] = wd + "/" + args[0];
    }
    m_process->start("/system/bin/linker64", args);
    notify("Qt", "Running");
}
