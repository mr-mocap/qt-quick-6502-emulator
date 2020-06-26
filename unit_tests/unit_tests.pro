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
    instruction_checks.hpp \
    instruction_definitions.hpp \
    instruction_helpers.hpp

SOURCES += \
        6502_tests.cpp \
        absolute_indexed_with_x_mode_ADC.cpp \
        absolute_indexed_with_x_mode_AND.cpp \
        absolute_indexed_with_x_mode_ASL.cpp \
        absolute_indexed_with_x_mode_LDA.cpp \
        absolute_indexed_with_x_mode_LDY.cpp \
        absolute_indexed_with_y_mode_ADC.cpp \
        absolute_indexed_with_y_mode_AND.cpp \
        absolute_indexed_with_y_mode_LDA.cpp \
        absolute_indexed_with_y_mode_LDX.cpp \
        absolute_mode_ADC.cpp \
        absolute_mode_AND.cpp \
        absolute_mode_ASL.cpp \
        absolute_mode_BIT.cpp \
        absolute_mode_LDA.cpp \
        absolute_mode_LDX.cpp \
        absolute_mode_LDY.cpp \
        accumulator_mode_ASL.cpp \
        immediate_mode_ADC.cpp \
        immediate_mode_AND.cpp \
        immediate_mode_LDA.cpp \
        immediate_mode_LDX.cpp \
        immediate_mode_LDY.cpp \
        implied_mode.cpp \
        indirect_y_indexed_ADC.cpp \
        indirect_y_indexed_AND.cpp \
        indirect_y_indexed_LDA.cpp \
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
        x_indexed_indirect_LDA.cpp \
        zero_page_indexed_with_x_mode_ADC.cpp \
        zero_page_indexed_with_x_mode_AND.cpp \
        zero_page_indexed_with_x_mode_ASL.cpp \
        zero_page_indexed_with_x_mode_LDA.cpp \
        main.cpp \
        zero_page_indexed_with_x_mode_LDY.cpp \
        zero_page_indexed_with_y_mode_LDX.cpp \
        zero_page_mode_ADC.cpp \
        zero_page_mode_AND.cpp \
        zero_page_mode_ASL.cpp \
        zero_page_mode_BIT.cpp \
        zero_page_mode_LDA.cpp \
        zero_page_mode_LDX.cpp \
        zero_page_mode_LDY.cpp

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
