// MainWindow.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 12月 17 15:18:54 2021 (+0800)
// Last-Updated: 六 10月 22 15:14:03 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 73
// URL: http://wuhongyi.cn 

#include "MainWindow.hh"
#include "About.hh"
#include "ConnectDialog.hh"
#include "OutputSettingDialog.hh"



#include "QtWidgets"

#include <iostream>
#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
{

  mCenterWidget = new QWidget;
  mCenterWidget->setStyleSheet(" background-image: url(:/images/logo2.png); background-repeat: no-repeat; background-position: center;");
  setCentralWidget(mCenterWidget);


  QTemporaryFile tmpFile;
  tmpFile.open();
  tmpFile.setAutoRemove(true);
  mTmpConfigFileName = tmpFile.fileName();
  std::cout<<mTmpConfigFileName.toStdString().c_str()<<std::endl;

  this->setWindowIcon(QIcon(":/images/logo2.png"));//图标
  this->setWindowTitle("GDDAQ");

  const QSize availableSize = qApp->desktop()->availableGeometry().size();
  int width = availableSize.width();
  int height = availableSize.height();
  std::cout<<width<<"  "<<height<<std::endl;

  mConfigFile = new QSettings(mTmpConfigFileName, QSettings::IniFormat, this);
  mConfigFileName = QDir::homePath() + DIRSEP + CONFIG_DIR + DIRSEP + DEFAUL_CONFIG_FILENAME;
  // if (!QDir(QDir::homePath() + DIRSEP + WAVEDUMP2_DIR).exists())
  //   QDir().mkdir(QDir::homePath() + DIRSEP + WAVEDUMP2_DIR);
  std::cout<<mConfigFileName.toStdString().c_str()<<std::endl;

  width *= 0.2; // 90% of the screen size
  height *= 0.8; // 90% of the screen size
  QSize newSize(width, height);
  // int widthPlot = width *= 0.8; // TODO prossible bug...
  setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, newSize, qApp->desktop()->availableGeometry()) );


  mMainToolBar = new QToolBar(this);
  this->addToolBar(mMainToolBar);
  
  mMainStatusBar = new QStatusBar(this);
  this->setStatusBar(mMainStatusBar);

  mControlPanel = new ControlPanel(this);
  // mControlPanel->disableAll(false);
  // mControlPanel->setVisible(false);
  
  
  mMainStatusBarText = new QLabel(this);
  mMainStatusBarText->setFixedWidth(110);
  mMainStatusBarText->setText("<html> <b> Disconnected </b></html>");
  mMainProgressBar = new QProgressBar(mMainStatusBar);
  mMainProgressBar->setTextVisible(false);
  mMainProgressBar->setFixedWidth(70);
  mMainProgressBar->setMaximum(100);
  mMainProgressBar->setMinimum(0);
  mMainProgressBar->setValue(0);
  mStatusLED = new QPushButton(this);
  mStatusLED->setEnabled(false);
  mStatusLED->setText(" No Errors ");
  mStatusLED->setStyleSheet("border-radius:6px; font: bold; color: black; background-color: rgb(48, 243, 18)"); //green

  
  mMainStatusBar->addPermanentWidget(new QLabel("<html> <b> Status:&nbsp;&nbsp;</b></html>"));
  mMainStatusBar->addPermanentWidget(mMainStatusBarText);
  mMainStatusBar->addPermanentWidget(mStatusLED);
  mMainStatusBar->addPermanentWidget(mMainProgressBar);


  

  PopulateToolBar();
  PopulateMenu();
  CreateDockWindows(width*0.8);
}

void MainWindow::PopulateToolBar()
{
  const QIcon exitIcon = QIcon(":/images/exit.png");
  
  mExitAct = new QAction(exitIcon, "Exit", this);
  mExitAct->setStatusTip(tr("Exit GDDAQ"));
  mExitAct->setToolTip("Exit GDDAQ");

  
  mBasicSettingsAct = new QAction("Basic", this);
  mBasicSettingsAct->setEnabled(false);
  
  mAboutAct = new QAction("About", this);
  mAboutAct->setStatusTip(tr("Show About"));
  mAboutAct->setToolTip("Show About");
  mAboutAct->setCheckable(false);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  connect(mAboutAct, SIGNAL(triggered()), this, SLOT(AboutSoftware()));
  connect(mBasicSettingsAct, SIGNAL(triggered()), this, SLOT(ShowBasicSettings()));
  connect(mExitAct, &QAction::triggered, qApp, &QApplication::closeAllWindows);
  

  mMainToolBar->addAction(mExitAct);
  mMainToolBar->addAction(mBasicSettingsAct);
  mMainToolBar->addAction(mAboutAct);
  mMainToolBar->addSeparator();

  mMainToolBar->setMovable(false);
}


void MainWindow::PopulateMenu()
{

  // file
  mFileMenu = new QMenu();
  this->menuBar()->addMenu(mFileMenu)->setText("&File");
  mFileMenu->addSeparator();


  mConnectMenuAct = new QAction("Connect to device", this);
  mFileMenu->addAction(mConnectMenuAct);

  mInfoMenuAct = new QAction("Show active device info", this);
  mFileMenu->addAction(mInfoMenuAct);
  mInfoMenuAct->setEnabled(false);

  mOutputConfigMenuAct = new QAction("Output Configure", this);
  mFileMenu->addAction(mOutputConfigMenuAct);

  mLoadMenuAct = new QAction("Load", this);
  mFileMenu->addAction(mLoadMenuAct);

  mOpenSettingsMenuAct = new QAction("Load", this);
  mFileMenu->addAction(mOpenSettingsMenuAct);
  
  mSaveMenuAct = new QAction("Save", this);
  mFileMenu->addAction(mSaveMenuAct);

  mSaveasMenuAct = new QAction("Save As ...", this);
  mFileMenu->addAction(mSaveasMenuAct);
  
  mExitMenuAct = new QAction("Exit", this);
  mFileMenu->addAction(mExitMenuAct);
  

  // configure
  mConfigMenu = new QMenu();
  this->menuBar()->addMenu(mConfigMenu)->setText("&Config");
  // mConfigMenu->setEnabled(false);


  // Expert
  mExpertLogicMenu = new QMenu();
  this->menuBar()->addMenu(mExpertLogicMenu)->setText("&Expert");

  // Debug
  mDebugMenu = new QMenu();
  this->menuBar()->addMenu(mDebugMenu)->setText("&Debug");

  
  // help
  mHelpMenu = new QMenu();
  this->menuBar()->addMenu(mHelpMenu)->setText("&Help");
  
  mHelpMenu->addAction(mAboutAct);
  mHelpMenu->setEnabled(true);

  //
  connect(mConnectMenuAct, SIGNAL(triggered()), this, SLOT(ConnectModule()));
  connect(mOutputConfigMenuAct, SIGNAL(triggered()), this, SLOT(OpenOutputSettings()));
  // connect(mInfoMenuAct, SIGNAL(triggered()), this, SLOT(DeviceInfo()));
  // connect(mLoadMenuAct, SIGNAL(triggered()), this, SLOT(load()));

  // connect(mSaveMenuAct, SIGNAL(triggered()), this, SLOT(save()));
  // connect(mSaveasMenuAct, SIGNAL(triggered()), this, SLOT(saveAs()));
  // connect(mOpenSettingsMenuAct, SIGNAL(triggered()), this, SLOT(openSettings()));
  
  connect(mExitMenuAct, &QAction::triggered, qApp, &QApplication::closeAllWindows);
}


void MainWindow::AboutSoftware()
{
  About *winfo = new About(this);
  winfo->exec();
}

void MainWindow::ConnectModule()
{
  ConnectDialog dialog(this);
  dialog.setModal(true);
  // dialog.exec();

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  if(dialog.exec() == QDialog::Accepted)
    {
      if(mBasicSettings != nullptr) delete mBasicSettings;
      mBasicSettings = new BasicSettings(this);
      mBasicSettings->show();
      mBasicSettings->setVisible(false);

      mBasicSettingsAct->setEnabled(true);
    }
}

void MainWindow::OpenOutputSettings()
{
  OutputSettingDialog dialog(this);
  dialog.setModal(true);
  dialog.exec();
}

void MainWindow::ShowBasicSettings()
{
  if(mBasicSettings == nullptr) return;

  if(mBasicSettings->isVisible())
    mBasicSettings->setVisible(false);
  else
    mBasicSettings->setVisible(true);
  
}



void MainWindow::CreateDockWindows(int width)
{
  QDockWidget *dock = new QDockWidget("<html><b>Control</b></html>", this);
  dock->setAllowedAreas(Qt::TopDockWidgetArea);
  dock->setTitleBarWidget(new QWidget());
  dock->setMaximumHeight(95);
  dock->setMinimumHeight(85);
  dock->setWidget(this->mControlPanel);
  addDockWidget(Qt::TopDockWidgetArea, dock);
}

void MainWindow::LoadConfigure()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open config file"), "", tr("MainWindow conf file (*.dat);;All Files (*)"));
  if (fileName.isEmpty())
    {
      // if (!Check_connection_Timer->isActive())
      // 	Check_connection_Timer->start(3000);
      return;
    }

  std::cout<<fileName.toStdString()<<std::endl;
  LoadConfigureFile(fileName);
}

void MainWindow::LoadConfigureFile(QString filename)
{
  std::ifstream ifs(filename.toStdString().c_str());
  json = nlohmann::json::parse(ifs);
  

  // std::cout<<json.dump()<<std::endl;

  // int modules = json["modules"];
  // std::cout<<"modules: "<<modules<<std::endl;

  // std::cout<<json["slotid"]<<"  "<<json["slotid"].size()<<std::endl;
  // std::cout<<json["firmware"]<<"  "<<json["firmware"].size()<<std::endl;
  
  // for (int i = 0; i < json["slotid"].size(); ++i)
  //   {
  //     std::cout<<json["slotid"][i].get<short>()<<std::endl;
  //     std::cout<<json["firmware"][i].get<std::string>()<<std::endl;
  //   }

  // std::cout<<json["dspfpga_100_12_std"]<<std::endl;
  // std::cout<<json["dspfpga_100_12_std"]["ldr"]<<std::endl;
  // std::cout<<json["dspfpga_100_12_std"]["var"]<<std::endl;
  // std::cout<<json["dspfpga_100_12_std"]["fippi"]<<std::endl;
  // std::cout<<json["dspfpga_100_12_std"]["trig"]<<std::endl;

  // std::cout<<json["firmware"].is_array()<<std::endl;
  // std::cout<<json["dspfpga_100_12_std"].empty()<<std::endl;
  // std::cout<<json["dspfpga_100_16_std"].empty()<<std::endl;

  // TODO
}

nlohmann::json MainWindow::GetJSON()
{
  return json;
}

void MainWindow::SetOutputSettings(QString folder, QString prefix, QString run)
{
  if (folder == "") folder = QDir::homePath();
  if (prefix == "") prefix = "data";
  if (run == "") run = "0";

  mOutput_folder = folder;
  mOutput_prefix = prefix;
  mOutput_run = run;
}

void MainWindow::GetOutputSettings(QString &folder, QString &prefix, QString &run)
{
  folder = mOutput_folder;
  prefix = mOutput_prefix;
  run = mOutput_run;
}


// 
// MainWindow.cc ends here
