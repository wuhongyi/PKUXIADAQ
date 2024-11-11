// OnlineRate.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 3月 11 16:34:28 2023 (+0800)
// Last-Updated: 日 11月  3 20:49:34 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 26
// URL: http://wuhongyi.cn 

#ifndef _ONLINERATE_H_
#define _ONLINERATE_H_


#include "Global.hh"
#include "DeviceHandle.hh"

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QtCore/QVariant>
#include <QtGui/QColor>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MainWindow;
class ModuleRate;
class ModuleAlert;


class OnlineRate : public QMainWindow
{
  Q_OBJECT
  
public:
  OnlineRate(MainWindow *parent, DeviceHandle *device);
  virtual ~OnlineRate();


  void UpdateOnlineRateInput(unsigned short mod, unsigned short ch, double value, QColor color = QColor(Qt::black));
  void UpdateOnlineRateOutput(unsigned short mod, unsigned short ch, double value, QColor color = QColor(Qt::black));
  void UpdateOutputDataSize(unsigned short mod, unsigned long value);




  
private:

  MainWindow * const mMainWindow;
  DeviceHandle *mDevice = nullptr;

  QWidget *centralWidget;

  ModuleRate *mModuleRate[13];
  QPushButton *ratebt[13];

  ModuleAlert *mModuleAlert[13];
  QPushButton *alertbt[13];
private:

  QTableWidget *tableonline;
  QVBoxLayout *verticalLayout;

			     
private slots:
  void VisibleModuleRate();
  void VisibleModuleAlert();
  void SaveCountRateLimit();
};

class ModuleRate : public QWidget
{
  Q_OBJECT
  
public:
  ModuleRate(QWidget *parent, DeviceHandle *device, unsigned short mod);
  virtual ~ModuleRate();

  void UpdateInput(unsigned short ch, double value, QColor color = QColor(Qt::black));
  void UpdateOutput(unsigned short ch, double value, QColor color = QColor(Qt::black));
  
private:
  DeviceHandle *mDevice = nullptr;
  unsigned short mMod;
  unsigned short mGroup;
  QTableWidget *tablerate;

private slots:
  void VisibleRate();
  
};


class ModuleAlert : public QWidget
{
 Q_OBJECT
  
public:
  ModuleAlert(QWidget *parent, MainWindow *w, DeviceHandle *device, unsigned short mod);
  virtual ~ModuleAlert();


  void ReadLowerUpper();
  void WriteLowerUpper();
  

private:
  MainWindow * const mMainWindow;
  DeviceHandle *mDevice = nullptr;
  unsigned short mMod;
  unsigned short mGroup;
  QTableWidget *tablealert;

private slots:
  void VisibleAlert();

};


#endif /* _ONLINERATE_H_ */

// 
// OnlineRate.hh ends here
