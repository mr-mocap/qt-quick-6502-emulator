# Use a relative path so we can just use it as a submodule from git.
GOOGLETEST_DIR = ../googletest

include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG += qt

QT += quick

HEADERS += \
    InstructionExecutorTestFixture.hpp \
    addressing_mode_helpers.hpp \
    instruction_checks.hpp \
    instruction_definitions.hpp \
    instruction_helpers.hpp

SOURCES += \
        6502_tests.cpp \
        absolute_indexed_with_x_mode_ADC.cpp \
        absolute_indexed_with_x_mode_AND.cpp \
        absolute_indexed_with_x_mode_ASL.cpp \
        absolute_indexed_with_x_mode_CMP.cpp \
        absolute_indexed_with_x_mode_DEC.cpp \
        absolute_indexed_with_x_mode_EOR.cpp \
        absolute_indexed_with_x_mode_INC.cpp \
        absolute_indexed_with_x_mode_LDA.cpp \
        absolute_indexed_with_x_mode_LDY.cpp \
        absolute_indexed_with_x_mode_LSR.cpp \
        absolute_indexed_with_x_mode_ROL.cpp \
        absolute_indexed_with_x_mode_ROR.cpp \
        absolute_indexed_with_x_mode_SBC.cpp \
        absolute_indexed_with_x_mode_STA.cpp \
        absolute_indexed_with_y_mode_ADC.cpp \
        absolute_indexed_with_y_mode_AND.cpp \
        absolute_indexed_with_y_mode_CMP.cpp \
        absolute_indexed_with_y_mode_EOR.cpp \
        absolute_indexed_with_y_mode_LDA.cpp \
        absolute_indexed_with_y_mode_LDX.cpp \
        absolute_indexed_with_y_mode_SBC.cpp \
        absolute_indexed_with_y_mode_STA.cpp \
        absolute_mode_ADC.cpp \
        absolute_mode_AND.cpp \
        absolute_mode_ASL.cpp \
        absolute_mode_BIT.cpp \
        absolute_mode_CMP.cpp \
        absolute_mode_CPX.cpp \
        absolute_mode_CPY.cpp \
        absolute_mode_DEC.cpp \
        absolute_mode_EOR.cpp \
        absolute_mode_INC.cpp \
        absolute_mode_JMP.cpp \
        absolute_mode_JSR.cpp \
        absolute_mode_LDA.cpp \
        absolute_mode_LDX.cpp \
        absolute_mode_LDY.cpp \
        absolute_mode_LSR.cpp \
        absolute_mode_ROL.cpp \
        absolute_mode_ROR.cpp \
        absolute_mode_SBC.cpp \
        absolute_mode_STA.cpp \
        absolute_mode_STX.cpp \
        absolute_mode_STY.cpp \
        accumulator_mode_ASL.cpp \
        accumulator_mode_LSR.cpp \
        accumulator_mode_ROL.cpp \
        accumulator_mode_ROR.cpp \
        addressing_mode_helpers.cpp \
        immediate_mode_ADC.cpp \
        immediate_mode_AND.cpp \
        immediate_mode_CMP.cpp \
        immediate_mode_CPX.cpp \
        immediate_mode_CPY.cpp \
        immediate_mode_EOR.cpp \
        immediate_mode_LDA.cpp \
        immediate_mode_LDX.cpp \
        immediate_mode_LDY.cpp \
        immediate_mode_SBC.cpp \
        implied_mode_CLC.cpp \
        implied_mode_CLD.cpp \
        implied_mode_CLI.cpp \
        implied_mode_CLV.cpp \
        implied_mode_DEX.cpp \
        implied_mode_DEY.cpp \
        implied_mode_INX.cpp \
        implied_mode_INY.cpp \
        implied_mode_NOP.cpp \
        implied_mode_PHA.cpp \
        implied_mode_PHP.cpp \
        implied_mode_PLA.cpp \
        implied_mode_PLP.cpp \
        implied_mode_RTI.cpp \
        implied_mode_RTS.cpp \
        implied_mode_SEC.cpp \
        implied_mode_SED.cpp \
        implied_mode_SEI.cpp \
        implied_mode_TAX.cpp \
        implied_mode_TAY.cpp \
        implied_mode_TSX.cpp \
        implied_mode_TXA.cpp \
        implied_mode_TXS.cpp \
        implied_mode_TYA.cpp \
        indirect_JMP.cpp \
        indirect_y_indexed_ADC.cpp \
        indirect_y_indexed_AND.cpp \
        indirect_y_indexed_CMP.cpp \
        indirect_y_indexed_EOR.cpp \
        indirect_y_indexed_LDA.cpp \
        indirect_y_indexed_SBC.cpp \
        indirect_y_indexed_STA.cpp \
        instruction_executor_tests.cpp \
        registers_tests.cpp \
        relative_mode_BCC.cpp \
        relative_mode_BCS.cpp \
        relative_mode_BEQ.cpp \
        relative_mode_BMI.cpp \
        relative_mode_BNE.cpp \
        relative_mode_BPL.cpp \
        relative_mode_BVC.cpp \
        relative_mode_BVS.cpp \
        x_indexed_indirect_ADC.cpp \
        x_indexed_indirect_AND.cpp \
        x_indexed_indirect_CMP.cpp \
        x_indexed_indirect_EOR.cpp \
        x_indexed_indirect_LDA.cpp \
        x_indexed_indirect_SBC.cpp \
        x_indexed_indirect_STA.cpp \
        zero_page_indexed_with_x_mode_ADC.cpp \
        zero_page_indexed_with_x_mode_AND.cpp \
        zero_page_indexed_with_x_mode_ASL.cpp \
        zero_page_indexed_with_x_mode_CMP.cpp \
        zero_page_indexed_with_x_mode_DEC.cpp \
        zero_page_indexed_with_x_mode_EOR.cpp \
        zero_page_indexed_with_x_mode_INC.cpp \
        zero_page_indexed_with_x_mode_LDA.cpp \
        main.cpp \
        zero_page_indexed_with_x_mode_LDY.cpp \
        zero_page_indexed_with_x_mode_LSR.cpp \
        zero_page_indexed_with_x_mode_ROL.cpp \
        zero_page_indexed_with_x_mode_ROR.cpp \
        zero_page_indexed_with_x_mode_SBC.cpp \
        zero_page_indexed_with_x_mode_STA.cpp \
        zero_page_indexed_with_x_mode_STY.cpp \
        zero_page_indexed_with_y_mode_LDX.cpp \
        zero_page_indexed_with_y_mode_STX.cpp \
        zero_page_mode_ADC.cpp \
        zero_page_mode_AND.cpp \
        zero_page_mode_ASL.cpp \
        zero_page_mode_BIT.cpp \
        zero_page_mode_CMP.cpp \
        zero_page_mode_CPX.cpp \
        zero_page_mode_CPY.cpp \
        zero_page_mode_DEC.cpp \
        zero_page_mode_EOR.cpp \
        zero_page_mode_INC.cpp \
        zero_page_mode_LDA.cpp \
        zero_page_mode_LDX.cpp \
        zero_page_mode_LDY.cpp \
        zero_page_mode_LSR.cpp \
        zero_page_mode_ROL.cpp \
        zero_page_mode_ROR.cpp \
        zero_page_mode_SBC.cpp \
        zero_page_mode_STA.cpp \
        zero_page_mode_STX.cpp \
        zero_page_mode_STY.cpp

# Generated by the "Add Library..." right mouse menu option.
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../emulator/release/ -lemulator
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../emulator/debug/ -lemulator
else:unix: LIBS += -L$$OUT_PWD/../emulator/ -lemulator

INCLUDEPATH += $$PWD/../emulator
DEPENDPATH += $$PWD/../emulator

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../emulator/release/libemulator.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../emulator/debug/libemulator.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../emulator/release/emulator.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../emulator/debug/emulator.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../emulator/libemulator.a

DISTFILES += \
    PLAN.md
