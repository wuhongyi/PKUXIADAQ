// ConnectDialog.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月  9 13:59:54 2022 (+0800)
// Last-Updated: 六 9月 10 12:10:54 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 29
// URL: http://wuhongyi.cn 

#ifndef _CONNECTDIALOG_H_
#define _CONNECTDIALOG_H_

#include "QtWidgets"
#include "QtWidgets/QDialog"
#include "QtWidgets/QPushButton"
#include "QtWidgets/QGridLayout"
#include "QtWidgets/QComboBox"
#include "QtWidgets/QFrame"
#include "QtWidgets/QLabel"
#include "QStringList"
#include "QString"

#include "nlohmann/json.hpp"
class MainWindow;

class ConnectDialog : public QDialog
{
  Q_OBJECT
  
public:
  ConnectDialog(MainWindow *parent = 0);
  virtual ~ConnectDialog();

  void IncreaseProgressBar(int i, int n);
  
private:
  MainWindow * const mMainWindow;
  bool mConnecting = false;


  void ShowProgress(bool flag);
  
  
  QGridLayout *gridLayout;
  QFrame *frame_conn;
  QPushButton *loadconfig;

  QVBoxLayout *verticalLayout0;
  QHBoxLayout *horizontalLayout0;			 
  QVBoxLayout *verticalLayout1;
  QVBoxLayout *verticalLayout2;
  QVBoxLayout *verticalLayout3;
  QVBoxLayout *verticalLayout4;
  QLabel *labelmod;
  QComboBox *modulenumber;
  QLabel *labelcrate;
  QComboBox *cratenumber;
  
  
  QLabel *label2;
  QLabel *label3;
  QProgressBar *progressBar;
  QButtonGroup *modulesel;
  QHBoxLayout *hboxLayout;
  QPushButton *connectButton;
  QPushButton *bootButton;
  QSpacerItem *verticalSpacer1;
  QSpacerItem *verticalSpacer2;
  QSpacerItem *horizontalSpacer1;
  QSpacerItem *horizontalSpacer2;
			      
private slots:
  void ConnectButtonClick();
  void BootButtonClick();

  void Load();
};

#endif /* _CONNECTDIALOG_H_ */