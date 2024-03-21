import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Terminal
ScrollablePage {
    id: scrollView
    required property list<string> commands
    required property list<string> environments
    required property QLog qlog
    property bool envVisible: false
    Column{
        anchors.centerIn: parent
        spacing: 10
        width: parent.width

        Repeater{
            id: listView
            model: commands.length
            width: parent.width

            delegate: Item{
                height: textArea.height
                width: parent.width
                TextArea {
                    id: textArea
                    text: commands[index]
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    enabled: false
                }
                Row{
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    spacing: 5
                    RoundButton{
                        text: "\u2713"
                        radius: height/2
                        onClicked: {
                            qlog.execute(commands[index], settings.envs)
                            qlog.line = "Execute: " + commands[index]
                        }
                    }
                    RoundButton {
                        text: "-"
                        radius: height/2
                        onClicked: {
                            textField.text = commands[index]
                            settings.savedCommands.splice(index, 1)
                        }
                    }
                }
            }
        }


        TextField{
            id: textField
            width: parent.width
            wrapMode: Text.WrapAnywhere
            placeholderText: "Command"
        }

        Row{
            anchors.right: parent.right
            spacing: 10
            Button {
                text: "Add"
                onClicked: {
                    settings.savedCommands.push(textField.text)
                }
            }

            Button {
                text: "Stop"
                onClicked: {
                    qlog.stop()
                }
            }

            Button{
                text: "Debug"
                onClicked: {
                    qlog.debug()
                }
            }
        }

        TextField{
            id: tsel
            placeholderText: "Selected File"
            readOnly: true
            width: parent.width
            wrapMode: Text.WrapAnywhere
        }

        FileDialog{
            id: fileDialog
            onAccepted: {
                tsel.text = this.currentFile
                // split %2F
                let s = tsel.text.split("%2F")
                tcmd.text = s[s.length - 1]
            }
        }

        RowLayout{
            spacing: 10
            width: parent.width

            TextField{
                id: tcmd
                placeholderText: "Name"
                wrapMode: Text.WrapAnywhere
                Layout.fillWidth: true
            }

            Button {
                text: "Select"
                onClicked: {
                    fileDialog.open()
                }
            }

            Button {
                text: "Install"
                onClicked: {
                    if (tsel.text === "" || tcmd.text === "")   {
                        return
                    }
                    qlog.install(tsel.text, tcmd.text)
                }
            }
        }

        RowLayout{
            width: parent.width
            Switch {
                id: editEnv
                text: "EditEnv"
                checked: envVisible
                Layout.alignment: Qt.AlignRight
                onToggled: {
                    envVisible = editEnv.checked
                }
            }
        }

        // environments
        Repeater{
            id: envView
            model: environments.length
            width: parent.width

            delegate: Item{
                visible: envVisible
                height: tKey.height
                width: parent.width
                TextArea {
                    id: tKey
                    text: environments[index]
                    wrapMode: Text.WrapAnywhere
                    width: parent.width
                    enabled: false
                }
                Row{
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    spacing: 5
                    RoundButton {
                        text: "-"
                        radius: height/2
                        onClicked: {
                            textField.text = environments[index]
                            settings.envs.splice(index, 1)
                        }
                    }
                }
            }
        }

        TextField{
            id: tenv
            visible: envVisible
            width: parent.width
            wrapMode: Text.WrapAnywhere
            placeholderText: "Environment"
        }

        Row{
            id: redit
            visible: envVisible
            anchors.right: parent.right
            spacing: 10
            Button {
                text: "Add"
                onClicked: {
                    settings.envs.push(tenv.text)
                }
            }
        }
    }
}
