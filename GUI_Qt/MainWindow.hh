// MainWindow.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 12月 17 15:18:35 2021 (+0800)
// Last-Updated: 一 11月 11 18:54:30 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 68
// URL: http://wuhongyi.cn 

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "ControlPanel.hh"

#include "DeviceHandle.hh"
#include "ReadoutThread.hh"
#include "RateThread.hh"
#include "RealDecoderThread.hh"
#include "BasicSettings.hh"
#include "LogicSettings.hh"
#include "OnlineRate.hh"
#include "Debug.hh"

#include <QtGui/QColor>
#include "QtWidgets/QMainWindow"
#include "QtWidgets"
#include "QStringList"
#include "QString"
#include "QtConcurrent/QtConcurrentRun"

#include "nlohmann/json.hpp"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
  MainWindow(QWidget *parent = Q_NULLPTR);

  void LoadConfigure();
  nlohmann::json GetJSON();

  void SetOutputSettings(QString folder, QString prefix, QString run, bool saved, bool reset);
  void GetOutputSettings(QString &folder, QString &prefix, QString &run, bool &saved, bool &reset);
  void ShowOutputSettings();
  void SaveOutputSettings();

  bool IsSavedFile() {return leSaved->isChecked();}
  
  bool DAQRunning() {return mReadoutThread->IsRunning();}

  void UpdateOnlineRateInput(unsigned short mod, unsigned short ch, double value, QColor color = QColor(Qt::black));
  void UpdateOnlineRateOutput(unsigned short mod, unsigned short ch, double value, QColor color = QColor(Qt::black));
  void UpdateOutputDataSize(unsigned short mod, unsigned long value);


  int GetCountRateUpper(unsigned short mod, unsigned short ch);
  int GetCountRateLower(unsigned short mod, unsigned short ch);
  void SetCountRateUpper(unsigned short mod, unsigned short ch, int value);
  void SetCountRateLower(unsigned short mod, unsigned short ch, int value);
  void SaveCountRateLimit();


  
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
  void ShowLogicSettings();
  void ShowOnlineRate();
  void ShowDebug();
  
  void StartAcquisition();
  void StopAcquisition();

  void SaveDefaultJSON();
  void SaveAsJSON();
  
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
  QAction *mLogicSettingsAct;
  QAction *mOnlineRateAct;
  QAction *mDebugAct;

  
  QAction *mConnectMenuAct;
  QAction *mLoadMenuAct;
  QAction *mSaveMenuAct;
  QAction *mInfoMenuAct;
  QAction *mSaveasMenuAct;
  QAction *mOpenSettingsMenuAct;
  QAction *mOutputConfigMenuAct;

  
  DeviceHandle *mDevice = nullptr;
  ReadoutThread *mReadoutThread = nullptr;
  RateThread *mRateThread = nullptr;
  RealDecoderThread *mRealDecoderThread = nullptr;
  BasicSettings *mBasicSettings = nullptr;
  LogicSettings *mLogicSettings = nullptr;
  OnlineRate *mOnlineRate = nullptr;
  Debug *mDebug = nullptr;

  
  
  nlohmann::json  json;

  // Data Save Global Variable
  QString mOutput_folder;
  QString mOutput_prefix;
  QString mOutput_run;

  QLineEdit *leFolder;
  QLineEdit *lePrefix;
  QLineEdit *leRun;
  QCheckBox *leSaved;
  QCheckBox *leClockReset;
  
}; 

#endif /* _MAINWINDOW_H_ */

// 
// MainWindow.hh ends here
