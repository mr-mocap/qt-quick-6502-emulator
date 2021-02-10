import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.2
import Qt.example.computer 1.0

Rectangle {
    id: main_item

    property font  labelFont
    property color labelColor: "white"
    property color flagSetColor: "white"
    property color flagClearColor: "red"
    property bool  hexDisplay: true

    function calculateValueStringFromInt(input_value) {
        return hexDisplay ? "$" + input_value.toString(16) : input_value.toString(10)
    }

    ColumnLayout {
        id: registers_column
        Layout.fillHeight: true

        RowLayout {
            id: status_row
            spacing: 10
            Layout.alignment: Qt.AlignLeft | Qt.AlignRight
            Layout.fillWidth: true
            Layout.margins: 0

            Label {
                id: status_label
                text: "STATUS:"
                font: main_item.labelFont
                color: main_item.labelColor
            }
            Label {
                text: "N"
                font: main_item.labelFont
                color: (Computer.cpu.status & 0x80) ? flagSetColor : flagClearColor
            }
            Label {
                text: "V"
                font: main_item.labelFont
                color: (Computer.cpu.status & 0x40) ? flagSetColor : flagClearColor
            }
            Label {
                text: "-"
                font: main_item.labelFont
                color: main_item.labelColor
            }
            Label {
                text: "B"
                font: main_item.labelFont
                color: (Computer.cpu.status & 0x10) ? flagSetColor : flagClearColor
            }
            Label {
                text: "D"
                font: main_item.labelFont
                color: (Computer.cpu.status & 0x08) ? flagSetColor : flagClearColor
            }
            Label {
                text: "I"
                font: main_item.labelFont
                color: (Computer.cpu.status & 0x04) ? flagSetColor : flagClearColor
            }
            Label {
                text: "Z"
                font: main_item.labelFont
                color: (Computer.cpu.status & 0x02) ? flagSetColor : flagClearColor
            }
            Label {
                text: "C"
                font: main_item.labelFont
                color: (Computer.cpu.status & 0x01) ? flagSetColor : flagClearColor
            }
        }
        RowLayout {
            id: pc_row
            Layout.alignment: Qt.AlignLeft | Qt.AlignRight
            Layout.fillWidth: true
            Layout.margins: 0

            Label {
                id: pc_label
                text: "PC:"
                font: main_item.labelFont
                color: main_item.labelColor
            }
            Label {
                text: calculateValueStringFromInt(Computer.cpu.pc)
                font: main_item.labelFont
                color: main_item.labelColor
            }
        }
        RowLayout {
            id: accumulator_row
            Layout.alignment: Qt.AlignLeft | Qt.AlignRight
            Layout.fillWidth: true
            Layout.margins: 0

            Label {
                id: accumulator_label
                text: "A:"
                font: main_item.labelFont
                color: main_item.labelColor
            }
            Label {
                text: calculateValueStringFromInt(Computer.cpu.a)
                font: main_item.labelFont
                color: main_item.labelColor
            }
        }
        RowLayout {
            id: x_register_row
            Layout.alignment: Qt.AlignLeft | Qt.AlignRight
            Layout.fillWidth: true
            Layout.margins: 0

            Label {
                id: x_label
                text: "X:"
                font: main_item.labelFont
                color: main_item.labelColor
            }
            Label {
                text: calculateValueStringFromInt(Computer.cpu.x)
                font: main_item.labelFont
                color: main_item.labelColor
            }
        }
        RowLayout {
            id: y_register_row
            Layout.alignment: Qt.AlignLeft | Qt.AlignRight
            Layout.fillWidth: true
            Layout.margins: 0

            Label {
                id: y_label
                text: "Y:"
                font: main_item.labelFont
                color: main_item.labelColor
            }
            Label {
                text: calculateValueStringFromInt(Computer.cpu.y)
                font: main_item.labelFont
                color: main_item.labelColor
            }
        }
        RowLayout {
            id: stack_pointer_row
            Layout.alignment: Qt.AlignLeft | Qt.AlignRight
            Layout.fillWidth: true
            Layout.margins: 0

            Label {
                id: stackpointer_label
                text: "STACK P:"
                font: main_item.labelFont
                color: main_item.labelColor
            }
            Label {
                text: calculateValueStringFromInt(Computer.cpu.stackPointer)
                font: main_item.labelFont
                color: main_item.labelColor
            }
        }
    }
}
