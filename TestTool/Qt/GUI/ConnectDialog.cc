// ConnectDialog.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 9月  9 14:00:18 2022 (+0800)
// Last-Updated: 六 10月 22 13:07:54 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 106
// URL: http://wuhongyi.cn 

#include "ConnectDialog.hh"
#include "MainWindow.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ConnectDialog::ConnectDialog(MainWindow *parent)
: QDialog(parent), mMainWindow(parent)
{
  if(this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("ConnectDialog"));

  this->setWindowModality(Qt::WindowModal);
  this->resize(550, 258);

  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
  this->setSizePolicy(sizePolicy);
  this->setMinimumSize(QSize(300, 100));
  this->setMaximumSize(QSize(65555, 55555));
  this->setModal(true);

  //
  gridLayout = new QGridLayout(this);
  gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

  // part1
  loadconfig = new QPushButton(this);
  gridLayout->addWidget(loadconfig, 0, 0, 1, 1, Qt::AlignHCenter);
  loadconfig->setObjectName(QString::fromUtf8("LoadConfig"));
  loadconfig->setMinimumSize(QSize(0, 35));
  loadconfig->setText(QApplication::translate("ConnectDialog", "Load configure file", nullptr));
  
  // part2
  frame_conn = new QFrame(this);
  gridLayout->addWidget(frame_conn, 1, 0, 1, 1);
  frame_conn->setObjectName(QString::fromUtf8("frame_conn"));
  frame_conn->setFrameShape(QFrame::StyledPanel);
  frame_conn->setFrameShadow(QFrame::Raised);
  
  QFont font1;
  font1.setBold(false);
  font1.setWeight(50);
  QFont font2;
  font2.setPointSize(8);

  verticalLayout0 = new QVBoxLayout(frame_conn);
  verticalLayout0->setObjectName(QString::fromUtf8("verticalLayout0"));
  
  horizontalLayout0 = new QHBoxLayout();
  horizontalLayout0->setObjectName(QString::fromUtf8("horizontalLayout0"));
  verticalLayout0->addLayout(horizontalLayout0);
  
  verticalLayout1 = new QVBoxLayout();
  verticalLayout1->setObjectName(QString::fromUtf8("verticalLayout1"));
  horizontalLayout0->addLayout(verticalLayout1);

  labelcrate = new QLabel(frame_conn);
  verticalLayout1->addWidget(labelcrate, 0 , Qt::AlignHCenter);
  labelcrate->setObjectName(QString::fromUtf8("labelcrate"));
  labelcrate->setFont(font1);
  labelcrate->setText(QApplication::translate("ConnectDialog", "<html><head/><body><p><span style=\" font-size:10pt;\">Crate ID.</span></p></body></html>", nullptr));


  cratenumber = new QComboBox(frame_conn);
  verticalLayout1->addWidget(cratenumber);
  for (int i = 0; i < 8; i++)
    {
      cratenumber->addItem(QString::number(i));
    }
  cratenumber->setObjectName(QString::fromUtf8("crateid"));
  cratenumber->setFont(font2);
  
  verticalLayout2 = new QVBoxLayout();
  verticalLayout2->setObjectName(QString::fromUtf8("verticalLayout2"));
  horizontalLayout0->addLayout(verticalLayout2);
  
  labelmod = new QLabel(frame_conn);
  verticalLayout2->addWidget(labelmod, 0 , Qt::AlignHCenter);
  labelmod->setObjectName(QString::fromUtf8("labelmod"));
  labelmod->setFont(font1);
  labelmod->setText(QApplication::translate("ConnectDialog", "<html><head/><body><p><span style=\" font-size:10pt;\">Module No.</span></p></body></html>", nullptr));


  modulenumber = new QComboBox(frame_conn);
  verticalLayout2->addWidget(modulenumber);
  for (int i = 1; i < 14; i++)
    {
      modulenumber->addItem(QString::number(i));
    }
  modulenumber->setObjectName(QString::fromUtf8("moduleNum"));
  modulenumber->setFont(font2);

  

  
  verticalLayout4 = new QVBoxLayout();
  verticalLayout4->setObjectName(QString::fromUtf8("verticalLayout4"));
  verticalLayout0->addLayout(verticalLayout4);
  
  label2 = new QLabel(frame_conn);
  verticalLayout4->addWidget(label2, 0, Qt::AlignHCenter);
  label2->setObjectName(QString::fromUtf8("label2"));
  label2->setFont(font1);
  label2->setText(QApplication::translate("ConnectDialog", "<html><head/><body><p><span style=\" font-size:10pt;\">Slot No.</span></p></body></html>", nullptr));

  QHBoxLayout *horizontalLayout21 = new QHBoxLayout();
  horizontalLayout21->setObjectName(QString::fromUtf8("horizontalLayout21"));
  verticalLayout4->addLayout(horizontalLayout21);
  
  modulesel = new QButtonGroup(frame_conn);
  modulesel->setExclusive(false);//取消互斥
  // verticalLayout2->addWidget(modulesel);
  modulesel->setObjectName(QString::fromUtf8("modulesel"));
  // modulesel->setFont(font2);
  horizontalLayout21->setAlignment(Qt::AlignRight|Qt::AlignTrailing);
  for (int i = 2; i <= 14; i++)
    {
      QRadioButton *temp = new QRadioButton(QString::number(i));
      modulesel->addButton(temp,i); 
    }	
  for (QWidget * item : modulesel->buttons())
    {
      item->setMinimumSize(400,0);
      horizontalLayout21->addWidget(item,0);
      // item->installEventFilter(this);
    }

  verticalSpacer1 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  verticalLayout0->addItem(verticalSpacer1);
  

  hboxLayout = new QHBoxLayout();
  verticalLayout0->addLayout(hboxLayout);
  hboxLayout->setSpacing(5);
  hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
  hboxLayout->setContentsMargins(0, 0, 0, 0);
  horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hboxLayout->addItem(horizontalSpacer1);
  connectButton = new QPushButton(frame_conn);
  connectButton->setObjectName(QString::fromUtf8("connectButton"));
  connectButton->setFont(font2);
  connectButton->setCheckable(false);
  connectButton->setText(QApplication::translate("ConnectDialog", "Connect", nullptr));
  hboxLayout->addWidget(connectButton);
  bootButton = new QPushButton(frame_conn);
  bootButton->setObjectName(QString::fromUtf8("BootButton"));
  bootButton->setText(QApplication::translate("ConnectDialog", "Boot", nullptr));
  bootButton->setFont(font2);
  hboxLayout->addWidget(bootButton);
  horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  hboxLayout->addItem(horizontalSpacer2);

  
  verticalSpacer2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
  verticalLayout0->addItem(verticalSpacer2);
  



  verticalLayout3 = new QVBoxLayout();
  verticalLayout3->setObjectName(QString::fromUtf8("verticalLayout3"));
  verticalLayout0->addLayout(verticalLayout3);


  label3 = new QLabel(frame_conn);
  verticalLayout3->addWidget(label3, 0, Qt::AlignHCenter);
  label3->setObjectName(QString::fromUtf8("lProgressBar"));
  label3->setFont(font1);
  label3->setText(QApplication::translate("openDialog", "<html><head/><body><p><span style=\" font-size:10pt;\">Please Wait init.</span></p></body></html>", nullptr));
  progressBar = new QProgressBar(frame_conn);
  verticalLayout3->addWidget(progressBar);
  progressBar->setObjectName(QString::fromUtf8("progressBar"));
  progressBar->setValue(0);
  progressBar->setTextVisible(false);
  progressBar->setFormat(QString());
        


  connect(loadconfig, SIGNAL(clicked()), this, SLOT(Load()));
  connect(connectButton, SIGNAL(clicked()), this, SLOT(ConnectButtonClick()));
  connect(bootButton, SIGNAL(clicked()), this, SLOT(BootButtonClick()));



  
  QMetaObject::connectSlotsByName(this);
  this->setWindowTitle(QApplication::translate("ConnectDialog", "Connect to a crate", nullptr));


  ShowProgress(false);
}
ConnectDialog::~ConnectDialog()
{

}


void ConnectDialog::ShowProgress(bool flag)
{
  label3->setVisible(flag);
  progressBar->setVisible(flag);
}

void ConnectDialog::ConnectButtonClick()
{
  if (mConnecting)
    {
      this->reject();
      return;
    }
  ShowProgress(true);
  mConnecting = true;

  // API for connect
  if(false)
    {
      ShowProgress(false);
      mConnecting = false;
      return;
    }

  // this->close();
}

void ConnectDialog::BootButtonClick()
{
  // this->reject();
  if (mConnecting)
    {
      // mWaveDump->abortConnection();
      // mConnecting = false;
    }


  this->accept(); //对话框 accept()，关闭对话框
}


void ConnectDialog::IncreaseProgressBar(int i, int n)
{
  // int v = ui->progressBar->value();
  // if (v == ui->progressBar->maximum()) v = 0; else v+=5;
  progressBar->setValue(100.0*i/n);
}

void ConnectDialog::Load()
{
  mMainWindow->LoadConfigure();


  // check json
  
  nlohmann::json json = mMainWindow->GetJSON();
  // std::cout<<"wuhongyi"<<std::endl;
  // std::cout<<json["slotid"]<<"  "<<json["slotid"].size()<<std::endl;
  // std::cout<<"mod index:  "<<modulenumber->currentIndex()<<std::endl;
  
  cratenumber->setCurrentIndex(json["crateid"].get<int>());
  modulenumber->setCurrentIndex(json["slotid"].size()-1);


  QList<QAbstractButton*> ins_list = modulesel->buttons();
  for(int i = 0; i < ins_list.length(); i++)
    {
      ins_list.at(i)->setChecked(false);
    }


  for (int i = 0; i < json["slotid"].size(); ++i)
    {
      ins_list.at(json["slotid"][i].get<int>()-2)->setChecked(true);
    }

}

// 
// ConnectDialog.cc ends here
