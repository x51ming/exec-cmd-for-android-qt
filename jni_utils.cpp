
#include "jni_utils.h"


#include <QtCore/private/qandroidextras_p.h>


static QJniObject activity = QtAndroidPrivate::activity();
static QJniObject context = QtAndroidPrivate::context();
static QString packageName = context.callObjectMethod(
       "getPackageName", "()Ljava/lang/String;").toString();
static QJniObject package = context.callObjectMethod(
        "getPackageName", "()Ljava/lang/String;");

QJniObject getSystemService(const QString &name) {
    auto jname = QJniObject::getStaticObjectField("android/content/Context",name.toStdString().c_str(),"Ljava/lang/String;");
    return activity.callObjectMethod(
        "getSystemService",
        "(Ljava/lang/String;)Ljava/lang/Object;",
        jname.object<jstring>());
}


QJniObject wakeLock;

void acquireWakeLock() {
    if (!wakeLock.isValid()) {
        auto powerManager = getSystemService("POWER_SERVICE");
        wakeLock = powerManager.callObjectMethod(
            "newWakeLock",
            "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;",
            1, // PowerManager.PARTIAL_WAKE_LOCK
            QJniObject::fromString(packageName+":wake").object());
    }
    if (wakeLock.isValid())
        wakeLock.callObjectMethod("acquire", "()V");
}

void releaseWakeLock(){
    if (wakeLock.isValid())
        wakeLock.callObjectMethod("release", "()V");
}

QString invoke(
    const QString &intent_qstr,
    const QString &package,
    const QString &activity,
    const QString &mime,
    const QString &url)
{
    //    adb shell am start
    //          -a $intent
    //          -n $package/$activity
    //          -d $url
    //    [-a <ACTION>] [-d <DATA_URI>]
    //    [-t <MIME_TYPE>] [-i <IDENTIFIER>]
    //    [-c <CATEGORY> [-c <CATEGORY>] ...]
    //    [-n <COMPONENT_NAME>]

    QJniObject jUrl(QJniObject::callStaticObjectMethod(
        "android/net/Uri", "parse",
        "(Ljava/lang/String;)Landroid/net/Uri;",
        QJniObject::fromString(url).object()));
    QJniObject intent("android/content/Intent", "(Ljava/lang/String;)V",
                      QJniObject::fromString(intent_qstr).object());
    intent.callObjectMethod("setData", "(Landroid/net/Uri;)Landroid/content/Intent;",
                            jUrl.object());
    if (mime != "")
        intent.callObjectMethod(
            "setType",
            "(Ljava/lang/String;)Landroid/content/Intent;",
            QJniObject::fromString(mime).object());
    intent.callObjectMethod(
        "setClassName",
        "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
        QJniObject::fromString(package).object(),
        QJniObject::fromString(activity).object());
    QtAndroidPrivate::startActivity(intent, 0);
    return "";
}
