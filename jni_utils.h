#ifndef JNI_UTILS_H
#define JNI_UTILS_H
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>


QJniObject getSystemService(const QString &name);
QJniObject startService(const QString& srvName);
QJniObject stopService(const QString& srvName);
QString invoke(
    const QString &intent_qstr,
    const QString &package,
    const QString &activity,
    const QString &mime,
    const QString &url);
void notify(const QString &title, const QString &message);
#endif // JNI_UTILS_H
