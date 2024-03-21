#ifndef QLOG_H
#define QLOG_H

#include <QObject>
#include <QQuickItem>
#include <QProcess>
#include <QJniObject>
#include <QFile>
#include <QTimer>

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
    Q_INVOKABLE void debug();

private:
    QString m_line;
    QString wd;
    QFile logfile;
    QFile errfile;
    QTimer timer;

signals:
    void lineChanged();
};

#endif // QLOG_H
