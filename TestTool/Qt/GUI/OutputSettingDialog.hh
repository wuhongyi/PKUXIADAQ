// OutputSettingDialog.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 9月 10 14:34:32 2022 (+0800)
// Last-Updated: 日 9月 11 09:47:36 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 16
// URL: http://wuhongyi.cn 

#ifndef _OUTPUTSETTINGDIALOG_H_
#define _OUTPUTSETTINGDIALOG_H_

#include "QtWidgets"
#include "QtWidgets/QDialog"
#include "QtWidgets/QPushButton"
#include "QtWidgets/QGridLayout"
#include "QtWidgets/QComboBox"
#include "QtWidgets/QFrame"
#include "QtWidgets/QLabel"
#include "QStringList"
#include "QString"

// #include <QtCore/QVariant>
// #include <QtWidgets/QApplication>
// #include <QtWidgets/QGroupBox>
// #include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QLabel>
// #include <QtWidgets/QLineEdit>
// #include <QtWidgets/QPushButton>
// #include <QtWidgets/QRadioButton>
// #include <QtWidgets/QSpacerItem>


class MainWindow;

class OutputSettingDialog : public QDialog
{
  Q_OBJECT
  
public:
  OutputSettingDialog(MainWindow *parent = 0);
  virtual ~OutputSettingDialog();


private:
  MainWindow * mMainWindow;


  QGridLayout *gridLayout;
  QLabel *labelfilefolder;
  QLabel *labelfileprefix;
  QLabel *labelrunnumber;
  QLabel *labelexample;
  QLineEdit *leFolder;
  QLineEdit *lePrefix;
  QLineEdit *leRun;
  QPushButton *pbBrowse;

  // QHBoxLayout *hboxLayout;
  QPushButton *okButton;
  QPushButton *cancelButton;
  QSpacerItem *horizontalSpacer3;
  QSpacerItem *horizontalSpacer4;
  
  void LoadSettings();



		     
private slots:
  
  void SaveSettings();
  void EditFolder();
  // void EditingFinished();
  

  
};

#endif /* _OUTPUTSETTINGDIALOG_H_ */
