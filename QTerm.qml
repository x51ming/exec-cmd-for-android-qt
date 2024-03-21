import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Terminal

ScrollablePage{
    // this is the terminal page
    property list<string> lines: ["Log"]
    required property string line
    onLineChanged: {
        lines.push(line)
        if(lines.length > 100){
            lines.shift()
        }
    }
    Column{
        spacing: 10
        Repeater{
            model: lines.length
            delegate: Label{
                width: parent.width
                text: lines[index]
                wrapMode: Text.WrapAnywhere
            }
        }
    }
}
