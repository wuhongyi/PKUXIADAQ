// MainWindow.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 12月 17 15:18:35 2021 (+0800)
// Last-Updated: 六 10月 22 14:44:13 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 35
// URL: http://wuhongyi.cn 

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ControlPanel.hh"

#include "DeviceHandle.hh"
#include "ReadoutThread.hh"
#include "BasicSettings.hh"

#include "QtWidgets/QMainWindow"
#include "QtWidgets"
#include "QStringList"
#include "QString"
#include "QtConcurrent/QtConcurrentRun"

#include "nlohmann/json.hpp"

#ifdef WIN32
	#define ENDL  "\r\n"
	#define DIRSEP "/"
#else
	#define ENDL  "\n"
	#define DIRSEP "/"
#endif

#define CONFIG_DIR "gddaqtest"
#define DEFAUL_CONFIG_FILENAME "gddaq_Config.wconf"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  MainWindow(QWidget *parent = Q_NULLPTR);

  void LoadConfigure();
  nlohmann::json GetJSON();

  void SetOutputSettings(QString folder, QString prefix, QString run);
  void GetOutputSettings(QString &folder, QString &prefix, QString &run);
  

  
private:

  void PopulateToolBar();
  void PopulateMenu();
    
  void LoadConfigureFile(QString filename);

		       
private slots:


public slots:
  void AboutSoftware();
  void ConnectModule();
  void OpenOutputSettings();

  void ShowBasicSettings();
  
private:

  QWidget *mCenterWidget;

  QToolBar *mMainToolBar;
  QStatusBar *mMainStatusBar;
  QPushButton* mStatusLED;
  QProgressBar *mMainProgressBar;
  QLabel *mMainStatusBarText;
  
  QString mTmpConfigFileName;
  QSettings *mConfigFile;
  QString mConfigFileName;


  ControlPanel *mControlPanel;
  void CreateDockWindows(int width);

  QMenu *mFileMenu;
  QMenu *mConfigMenu;
  QMenu *mExpertLogicMenu;
  QMenu *mDebugMenu;
  QMenu *mHelpMenu;
  
  QAction *mExitAct;
  QAction *mExitMenuAct;
  QAction *mAboutAct;
  QAction *mBasicSettingsAct;

  
  QAction *mConnectMenuAct;
  QAction *mLoadMenuAct;
  QAction *mSaveMenuAct;
  QAction *mInfoMenuAct;
  QAction *mSaveasMenuAct;
  QAction *mOpenSettingsMenuAct;
  QAction *mOutputConfigMenuAct;

  
  DeviceHandle *mDevice = nullptr;
  ReadoutThread *mReadoutThread = nullptr;
  BasicSettings *mBasicSettings = nullptr;

  
  nlohmann::json  json;

  // Data Save Global Variable
  QString mOutput_folder;
  QString mOutput_prefix;
  QString mOutput_run;
  
}; 

#endif /* _MAINWINDOW_H_ */

// 
// MainWindow.hh ends here
