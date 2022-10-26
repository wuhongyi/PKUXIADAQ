QT += widgets
requires(qtConfig(filedialog))
INCLUDEPATH += ../pixiesdk/include
unix: LIBS += -L$$PWD/../pixiesdk/lib -lPixieSDK -lPixie16Api
QMAKE_LFLAGS += "-Wl,-rpath=../pixiesdk/lib"
HEADERS       = MainWindow.hh Global.hh About.hh ControlPanel.hh DeviceHandle.hh ReadoutThread.hh ConnectDialog.hh OutputSettingDialog.hh Login.hh BasicSettings.hh
SOURCES       = main.cc \
                MainWindow.cc Global.cc About.cc ControlPanel.cc DeviceHandle.cc ReadoutThread.cc ConnectDialog.cc OutputSettingDialog.cc Login.cc BasicSettings.cc BasicSettings_TabPolarityGainBaseline.cc BasicSettings_TabQDC.cc BasicSettings_TabCFD.cc BasicSettings_TabDataRecord.cc BasicSettings_TabTriggerFilter.cc BasicSettings_TabEnergyFilter.cc BasicSettings_TabWaveform.cc
#! [0]
RESOURCES     = gddaq.qrc 
#! [0]
