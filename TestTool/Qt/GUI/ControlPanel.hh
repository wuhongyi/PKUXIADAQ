// ControlPanel.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 4月 23 21:07:31 2022 (+0800)
// Last-Updated: 二 10月 25 20:35:22 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 5
// URL: http://wuhongyi.cn 

#ifndef _CONTROLPANEL_H_
#define _CONTROLPANEL_H_

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>

// #include <QtCore/QLocale>
// #include <QtCore/QVariant>
// #include <QtWidgets/QApplication>
// #include <QtWidgets/QComboBox>
// #include <QtWidgets/QDoubleSpinBox>
// #include <QtWidgets/QGroupBox>
// #include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QLabel>
// #include <QtWidgets/QLineEdit>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class MainWindow;

class ControlPanel : public QWidget
{
  Q_OBJECT

public:
  explicit ControlPanel(MainWindow *parent = nullptr);
  ~ControlPanel();

  QTimer *trigger_timer;

  void DisableAll(bool f);
  void ToogleStartStop();
  void SetStartStopStatus(int status);
  void UpdateStartableDevices();

  QGridLayout *gridlayout;
  QVBoxLayout *verticallayout;
  QPushButton *pbStartAcq;
  QPushButton *pbStopAcq;


private:

  MainWindow * const mMainWindow;


private slots:
  
  
};

#endif /* _CONTROLPANEL_H_ */

// 
// ControlPanel.hh ends here
