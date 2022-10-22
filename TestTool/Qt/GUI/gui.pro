QT += widgets
requires(qtConfig(filedialog))

HEADERS       = MainWindow.hh About.hh ControlPanel.hh DeviceHandle.hh ReadoutThread.hh ConnectDialog.hh OutputSettingDialog.hh Login.hh BasicSettings.hh
SOURCES       = main.cc \
                MainWindow.cc About.cc ControlPanel.cc DeviceHandle.cc ReadoutThread.cc ConnectDialog.cc OutputSettingDialog.cc Login.cc BasicSettings.cc
#! [0]
RESOURCES     = gui.qrc 
#! [0]
