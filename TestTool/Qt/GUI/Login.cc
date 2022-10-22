// Login.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 10月 20 21:11:53 2022 (+0800)
// Last-Updated: 五 10月 21 14:50:34 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.cn 

#include "Login.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Login::Login(QWidget *parent)
: QDialog(parent)
{
  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("Login"));
  this->resize(520, 360);
  this->setMaximumSize(QSize(520, 360));

  QFont font;
  font.setPointSize(11);
  this->setFont(font);


  verticalLayout = new QVBoxLayout(this);
  verticalLayout->setSpacing(0);
  verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
  verticalLayout->setContentsMargins(0, 0, 0, 0);

  label = new QLabel(this);
  verticalLayout->addWidget(label);
  label->setObjectName(QString::fromUtf8("label"));
  label->setMaximumSize(QSize(110, 110));
  label->setPixmap(QPixmap(QString::fromUtf8(":/images/logo2.png")));
  label->setScaledContents(true);


  
  
  horizontalLayout = new QHBoxLayout();
  verticalLayout->addLayout(horizontalLayout);
  horizontalLayout->setSpacing(5);
  horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));  


  label_2 = new QLabel(this);
  label_2->setObjectName(QString::fromUtf8("label_2"));
  horizontalLayout->addWidget(label_2);

  editUser = new QLineEdit(this);
  editUser->setObjectName(QString::fromUtf8("editUser"));
  horizontalLayout->addWidget(editUser);

  label_3 = new QLabel(this);
  label_3->setObjectName(QString::fromUtf8("label_3"));
  horizontalLayout->addWidget(label_3);

  editPSWD = new QLineEdit(this);
  editPSWD->setObjectName(QString::fromUtf8("editPSWD"));
  editPSWD->setEchoMode(QLineEdit::Password);
  horizontalLayout->addWidget(editPSWD);

  chkBoxSave = new QCheckBox(this);
  chkBoxSave->setObjectName(QString::fromUtf8("chkBoxSave"));
  horizontalLayout->addWidget(chkBoxSave);

  horizontalSpacer_3 = new QSpacerItem(17, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  horizontalLayout->addItem(horizontalSpacer_3);
  
  btnOK = new QPushButton(this);
  horizontalLayout->addWidget(btnOK);
  btnOK->setObjectName(QString::fromUtf8("btnOK"));
  // QIcon icon;
  // icon.addFile(QString::fromUtf8(":/images/images/704.bmp"), QSize(), QIcon::Normal, QIcon::Off);
  // btnOK->setIcon(icon);


  btnCancel = new QPushButton(this);
  horizontalLayout->addWidget(btnCancel);
  btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
  // QIcon icon1;
  // icon1.addFile(QString::fromUtf8(":/images/images/706.bmp"), QSize(), QIcon::Normal, QIcon::Off);
  // btnCancel->setIcon(icon1);


  horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  horizontalLayout->addItem(horizontalSpacer_6);


  this->setWindowTitle(QApplication::translate("Login", "Dialog", nullptr));
  label->setText(QString());
  label_2->setText(QApplication::translate("Login", "\347\224\250\346\210\267\345\220\215", nullptr));
  label_3->setText(QApplication::translate("Login", "\345\257\206 \347\240\201", nullptr));
  chkBoxSave->setText(QApplication::translate("Login", "\344\277\235\345\255\230\347\224\250\346\210\267\345\220\215", nullptr));
  btnOK->setText(QApplication::translate("Login", "\347\241\256\345\256\232", nullptr));
  btnCancel->setText(QApplication::translate("Login", "\345\217\226\346\266\210", nullptr));

  
  QObject::connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
  QObject::connect(btnOK, SIGNAL(clicked()), this, SLOT(Button_OK_Clicked()));

  
  QMetaObject::connectSlotsByName(this);

  editPSWD->setEchoMode(QLineEdit::Password); //密码输入编辑框设置为密码输入模式
  this->setAttribute(Qt::WA_DeleteOnClose);//设置为关闭时删除
  this->setWindowFlags(Qt::SplashScreen); //设置为SplashScreen, 窗口无边框,不在任务栏显示
  
}

Login::~Login()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Login::Button_OK_Clicked()
{
  //"确定"按钮相应
  QString user = editUser->text().trimmed();//输入用户名
  QString pswd = editPSWD->text().trimmed(); //输入密码

  QString encrptPSWD = Encrypt(pswd); //对输入密码进行加密

  if ((user == m_user) && (pswd == m_pswd)) //如果用户名和密码正确
    {
      WriteSettings();//保存设置
      this->accept(); //对话框 accept()，关闭对话框
    }
  else
    {
      m_tryCount++; //错误次数
      if (m_tryCount > 3)
        {
	  QMessageBox::critical(this, "错误", "输入错误次数太多，强行退出");
	  this->reject(); //退出
        }
      else
	QMessageBox::warning(this, "错误提示", "用户名或密码错误");
    }

}


void Login::mousePressEvent(QMouseEvent *event)
{
  //鼠标按键被按下
  if (event->button() == Qt::LeftButton)
    {
      m_moving = true;
      //记录下鼠标相对于窗口的位置
      //event->globalPos()鼠标按下时，鼠标相对于整个屏幕位置
      //pos() this->pos()鼠标按下时，窗口相对于整个屏幕位置
      m_lastPos = event->globalPos() - pos();
    }
  return QDialog::mousePressEvent(event);  //
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
  //鼠标按下左键移动
  //(event->buttons() && Qt::LeftButton)按下是左键
  //鼠标移动事件需要移动窗口，窗口移动到哪里呢？就是要获取鼠标移动中，窗口在整个屏幕的坐标，然后move到这个坐标，怎么获取坐标？
  //通过事件event->globalPos()知道鼠标坐标，鼠标坐标减去鼠标相对于窗口位置，就是窗口在整个屏幕的坐标
  if (m_moving && (event->buttons() && Qt::LeftButton)
      && (event->globalPos()-m_lastPos).manhattanLength() > QApplication::startDragDistance())
    {
      move(event->globalPos() - m_lastPos);
      m_lastPos = event->globalPos() - pos();
    }
  return QDialog::mouseMoveEvent(event);
}

void Login::mouseReleaseEvent(QMouseEvent *event)
{
  //鼠标按键释放
  m_moving = false; //停止移动
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void  Login::ReadSettings()
{
  //读取存储的用户名和密码, 密码是经过加密的
  QString organization = "GDDAQ";//用于注册表，
  QString appName = "GDDAQ-Qt"; //HKEY_CURRENT_USER/Software/WWB-Qt/amp6_5

  QSettings settings(organization, appName);//创建

  bool saved = settings.value("saved",false).toBool();//读取 saved键的值
  m_user = settings.value("Username", "admin").toString();//读取 Username 键的值，缺省为“admin”

  QString defaultPSWD = Encrypt("admin"); //缺省密码"12345"加密后的数据

  m_pswd = settings.value("PSWD", defaultPSWD).toString();//读取PSWD 键的值，

  if (saved)
    editUser->setText(m_user);

  chkBoxSave->setChecked(saved);
}

void Login::WriteSettings()
{
  //保存用户名，密码等设置
  QSettings settings("GDDAQ", "GDDAQ-Qt"); //注册表键组
  settings.setValue("Username", m_user); //用户名
  settings.setValue("PSWD", m_pswd);   //密码，经过加密的
  settings.setValue("saved", chkBoxSave->isChecked());
}

QString Login::Encrypt(const QString& str)
{
  //字符串MD5算法加密
  QByteArray btArray;
  btArray.append(str);//加入原始字符串

  QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法
  hash.addData(btArray);  //添加数据到加密哈希值

  QByteArray resultArray = hash.result();  //返回最终的哈希值
  QString md5 = resultArray.toHex();//转换为16进制字符串

  return  md5;
}



// 
// Login.cc ends here
