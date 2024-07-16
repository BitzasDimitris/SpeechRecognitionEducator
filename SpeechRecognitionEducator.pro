#-------------------------------------------------
#
# Project created by QtCreator 2016-02-27T03:46:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets printsupport

TARGET = SpeechRecognitionEducator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    SoundDataOperators.cpp \
    framesizedialog.cpp \
    qcustomplot.cpp \
    Wavfile.cpp \
    waveform.cpp \
    frameswindow.cpp \
    audiosettingsdialog.cpp \
    directoryrunner.cpp \
    threadhandler.cpp \
    wavfilesholder.cpp \
    mfccholder.cpp \
    phonemeparser.cpp \
    phonemesLibrary.cpp \
    phoneme.cpp \
    hmmtrainer.cpp \
    mog.cpp \
    matrixcalcs.cpp \
    openclinterface.cpp \
    mfccdialog.cpp \
    openclsettings.cpp \
    hmmsettings.cpp

QMAKE_LFLAGS += /LARGEADDRESSAWARE

HEADERS  += mainwindow.h \
    SoundDataOperators.h \
    framesizedialog.h \
    qcustomplot.h \
    wavestructs.h \
    waveform.h \
    frameswindow.h \
    audiosettingsdialog.h \
    directoryrunner.h \
    threadhandler.h \
    wavfilesholder.h \
    WavFile.h \
    mfccholder.h \
    phonemeparser.h \
    phonemesLibrary.h \
    phoneme.h \
    hmmtrainer.h \
    mog.h \
    matrixcalcs.h \
    openclinterface.h \
    mfccdialog.h \
    openclsettings.h \
    hmmsettings.h

FORMS    += mainwindow.ui \
    framesizedialog.ui \
    waveform.ui \
    frameswindow.ui \
    audiosettingsdialog.ui \
    mfccdialog.ui \
    openclsettings.ui \
    hmmsettings.ui


INCLUDEPATH += $$PWD\\include\
DEPENDPATH += $$PWD\\include\

INCLUDEPATH += $$(AMDAPPSDKROOT)\\include\
DEPENDPATH += $$(AMDAPPSDKROOT)\\include\


LIBS += -L$$(AMDAPPSDKROOT)\\lib\\x86\
-lOpenCL \
-lamdocl \
-lamdocl12cl \
-lglew32 \
-lglut32


RESOURCES     = Resources.qrc

