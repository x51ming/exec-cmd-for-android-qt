
#include "jni_utils.h"


#include <QtCore/private/qandroidextras_p.h>


QJniObject activity = QtAndroidPrivate::activity();
QJniObject context = QtAndroidPrivate::context();
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


QJniObject startService(const QString& srvName){
    QAndroidIntent serviceIntent(
        activity.object(),
        srvName.toStdString().c_str());
    return activity.callObjectMethod(
        "startService",
        "(Landroid/content/Intent;)Landroid/content/ComponentName;",
        serviceIntent.handle().object());
}

QJniObject stopService(const QString& srvName){
    QAndroidIntent serviceIntent(
        activity.object(),
        srvName.toStdString().c_str());
    // return bool
    activity.callMethod<jboolean>(
        "stopService",
        "(Landroid/content/Intent;)Z",
        serviceIntent.handle().object());
    return QJniObject();
}

void notify(const QString &title, const QString &message)
{
    QJniObject javaNotification = QJniObject::fromString(message);
    QJniObject javaTitle = QJniObject::fromString(title);
    QJniObject::callStaticMethod<void>(
        "org/qtproject/example/QNotification",
        "notify",
        "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)V",
        context,
        // QNativeInterface::QAndroidApplication::context(),
        javaTitle.object<jstring>(),
        javaNotification.object<jstring>());
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
