// OutputSettingDialog.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 9月 10 14:34:50 2022 (+0800)
// Last-Updated: 日 9月 11 09:19:40 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 88
// URL: http://wuhongyi.cn 

#include "OutputSettingDialog.hh"
#include "MainWindow.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


OutputSettingDialog::OutputSettingDialog(MainWindow *parent)
: QDialog(parent)
{
  mMainWindow = parent;

  
  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("OutputSettingDialog"));
  this->resize(427, 200);


  
  QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
  sizePolicy1.setHorizontalStretch(0);
  sizePolicy1.setVerticalStretch(0);

  QFont font1;
  font1.setBold(true);
  font1.setWeight(75);
  QFont font2;
  font2.setItalic(true);
  font2.setUnderline(false);
  font2.setStrikeOut(false);
  font2.setKerning(true);

  




  labelfilefolder = new QLabel(this);
  labelfilefolder->setObjectName(QString::fromUtf8("labelfilefolder"));
  labelfilefolder->setFont(font1);
  labelfilefolder->setText(QApplication::translate("OutputSettingDialog", "File Folder", nullptr));
        
  leFolder = new QLineEdit(this);
  leFolder->setObjectName(QString::fromUtf8("leFolder"));
  sizePolicy1.setHeightForWidth(leFolder->sizePolicy().hasHeightForWidth());
  leFolder->setSizePolicy(sizePolicy1);
  leFolder->setText("/home/wuhongyi/");

  pbBrowse = new QPushButton(this);
  pbBrowse->setText(QApplication::translate("OutputSettingDialog", "Browse", nullptr));
  pbBrowse->setObjectName(QString::fromUtf8("pbBrowse"));
  sizePolicy1.setHeightForWidth(pbBrowse->sizePolicy().hasHeightForWidth());
  pbBrowse->setSizePolicy(sizePolicy1);
  pbBrowse->setMinimumSize(QSize(0, 22));
  pbBrowse->setMaximumSize(QSize(16777215, 22));

        
  labelfileprefix = new QLabel(this);
  labelfileprefix->setText(QApplication::translate("OutputSettingDialog", "File Prefix", nullptr));
  labelfileprefix->setObjectName(QString::fromUtf8("labelfileprefix"));
  labelfileprefix->setFont(font1);
  

  lePrefix = new QLineEdit(this);
  lePrefix->setObjectName(QString::fromUtf8("lePrefix"));
  sizePolicy1.setHeightForWidth(lePrefix->sizePolicy().hasHeightForWidth());
  lePrefix->setSizePolicy(sizePolicy1);
  lePrefix->setMinimumSize(QSize(0, 22));
  lePrefix->setMaximumSize(QSize(16777215, 22));
  lePrefix->setText("data");


  labelrunnumber= new QLabel(this);
  labelrunnumber->setText(QApplication::translate("OutputSettingDialog", "Run Number", nullptr));
  labelrunnumber->setObjectName(QString::fromUtf8("labelrunnumber"));
  labelrunnumber->setFont(font1);


  leRun = new QLineEdit(this);
  leRun->setObjectName(QString::fromUtf8("leRun"));
  sizePolicy1.setHeightForWidth(leRun->sizePolicy().hasHeightForWidth());
  leRun->setSizePolicy(sizePolicy1);
  leRun->setMinimumSize(QSize(0, 22));
  leRun->setMaximumSize(QSize(16777215, 22));
  leRun->setText("0");


  
  
  labelexample = new QLabel(this);
  labelexample->setObjectName(QString::fromUtf8("labelexample"));
  labelexample->setFont(font2);
  labelexample->setText(QApplication::translate("OutputSettingDialog", "i.e. [run_id]/[file_prefix]_R[run_id]_M[mod_id].bin", nullptr));





  okButton = new QPushButton(this);
  okButton->setObjectName(QString::fromUtf8("okButton"));
  okButton->setMinimumSize(QSize(0, 25));
  okButton->setText(QApplication::translate("OutputSettingDialog", "OK", nullptr));


  cancelButton = new QPushButton(this);
  cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
  cancelButton->setMinimumSize(QSize(0, 25));
  cancelButton->setText(QApplication::translate("OutputSettingDialog", "Cancel", nullptr));

  
  gridLayout = new QGridLayout(this);
  this->setLayout(gridLayout);
  gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
  gridLayout->addWidget(labelfilefolder, 0, 0);
  gridLayout->addWidget(leFolder, 0, 1);
  gridLayout->addWidget(pbBrowse, 0, 2);
  gridLayout->addWidget(labelfileprefix, 1, 0);
  gridLayout->addWidget(lePrefix, 1, 1);
  gridLayout->addWidget(labelrunnumber, 2, 0);
  gridLayout->addWidget(leRun, 2, 1);
  gridLayout->addWidget(labelexample, 4, 1);
  gridLayout->addWidget(okButton, 6, 0);
  gridLayout->addWidget(cancelButton, 6, 2);
  
  connect(pbBrowse, SIGNAL(clicked()), this, SLOT(EditFolder()));
  connect(okButton, SIGNAL(clicked()), this, SLOT(SaveSettings()));
  
  // connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  LoadSettings();

  this->setWindowTitle(QApplication::translate("OutputSettingDialog", "Output Settings", nullptr));
  
  QMetaObject::connectSlotsByName(this);
}

OutputSettingDialog::~OutputSettingDialog()
{

}

void OutputSettingDialog::LoadSettings()
{

}


void OutputSettingDialog::SaveSettings()
{
  // mWaveDump->setOutputSettings(this->ui->leFolder->text(), 
  // 			       this->ui->lePrefix->text(), 
  // 			       (ui->rbSingle->isChecked()) ? "SINGLE" : "MULTIPLE",
  // 			       (ui->rbYes->isChecked()) ? "YES" : "NO",
  // 			       (ui->rbAscii->isChecked()) ? "ASCII" : "BINARY",
  // 			       true);

}

void OutputSettingDialog::EditFolder()
{
  QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), leFolder->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (dir != "") leFolder->setText(dir);

}


// 
// OutputSettingDialog.cc ends here
