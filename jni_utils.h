#ifndef JNI_UTILS_H
#define JNI_UTILS_H
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>


QJniObject getSystemService(
    const QJniObject &activity,
    const QString &name);

void acquireWakeLock();
void releaseWakeLock();
#endif // JNI_UTILS_H
