// Login.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 10月 20 21:11:36 2022 (+0800)
// Last-Updated: 四 11月 14 18:41:59 2024 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 15
// URL: http://wuhongyi.cn 

#ifndef _LOGIN_H_
#define _LOGIN_H_

#include "Global.hh"

#include "QDialog"

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QSettings>
#include <QCryptographicHash>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Login : public QDialog
{
  Q_OBJECT

  
public:
  explicit Login(QWidget *parent = nullptr);
  virtual ~Login();


private:

  void    ReadSettings(); //读取设置,从注册表
  void    WriteSettings();//写入设置，从注册表
  QString Encrypt(const QString& str);//字符串加密  


  private:
  QString mUser;
  
  bool    m_moving = false;//表示窗口是否在鼠标操作下移动
  QPoint  m_lastPos;  //上一次的鼠标位置

  QString m_user = "admin"; //初始化用户名
  QString m_pswd = "admin";//初始化密码，未加密的

  int m_tryCount = 0; //试错次数



  QVBoxLayout *verticalLayout;
  QLabel *label;
 
  QHBoxLayout *horizontalLayout;
  QLabel *label_2;
  QLineEdit *editUser;
  QLabel *label_3;
  QLineEdit *editPSWD;
  QCheckBox *chkBoxSave;
  QSpacerItem *horizontalSpacer_3;
  QPushButton *btnOK;
  QPushButton *btnCancel;
  QSpacerItem *horizontalSpacer_6;

  QHBoxLayout *horizontalLayoutinfo;
  QLabel *labeled;
  
protected:
  
  //用于鼠标拖动窗口的鼠标事件操作
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void Button_OK_Clicked(); //OK按键
  
};

#endif /* _LOGIN_H_ */

// 
// Login.hh ends here
