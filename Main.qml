import QtCore
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Terminal

ApplicationWindow{
    width: 640
    height: 480
    visible: true
    title: qsTr("Terminal")
    Settings {
        id: settings
        property list<string> savedCommands: ["/system/bin/ls -lh"]
    }

    QLog{
        id: ql
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent

        Commands{
            qlog: ql
            title: "Commands"
            commands: settings.savedCommands
        }

        QTerm{
            line: ql.line
            title: "Log"
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex
        Repeater {
            model: swipeView.count
            TabButton {
                text: swipeView.contentData[index].title
                onClicked: swipeView.currentIndex = index
            }
        }
    }
}
