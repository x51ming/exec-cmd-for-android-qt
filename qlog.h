#ifndef QLOG_H
#define QLOG_H

#include <QObject>
#include <QQuickItem>
#include <QProcess>
#include <QJniObject>

class QLog : public QQuickItem
{
    Q_OBJECT
    QML_ELEMENT
public:
    QLog();

    Q_PROPERTY(QString line READ line WRITE setLine NOTIFY lineChanged)

    QString line() const;
    void setLine(const QString &line);

    Q_INVOKABLE void execute(
        const QString &cmd,
        const QStringList& envs = QStringList());
    Q_INVOKABLE bool stop();
    Q_INVOKABLE void kill();
    Q_INVOKABLE void install(const QString& file,
                             const QString& target);
    Q_INVOKABLE void notify(const QString& title,
                            const QString& message);

private:
    void connectProcess();
    QProcess *m_process;
    QString m_line;
    QString wd;
    qint64 m_pid;
signals:
    void lineChanged();
};

#endif // QLOG_H
