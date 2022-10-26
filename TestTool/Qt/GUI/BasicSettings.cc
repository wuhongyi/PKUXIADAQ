// BasicSettings.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 21 20:26:55 2022 (+0800)
// Last-Updated: 二 10月 25 22:19:08 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 125
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include "MainWindow.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BasicSettings::BasicSettings(MainWindow *parent, DeviceHandle *device)
: QMainWindow(parent), mMainWindow(parent), mDevice(device)
{
  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("BasicSettings"));

  this->setWindowTitle(QApplication::translate("BasicSettings", "Basic Settings", nullptr));

  const QSize availableSize = qApp->desktop()->availableGeometry().size();
  int width = availableSize.width();
  int height = availableSize.height();
  width *= 0.55; // 90% of the screen size
  height *= 0.8; // 90% of the screen size
  QSize newSize(width, height);
  // int widthPlot = width *= 0.8; // TODO prossible bug...
  setGeometry( QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, newSize, qApp->desktop()->availableGeometry()) );
  
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
  
  splitter = new QSplitter(centralWidget);
  splitter->setObjectName(QString::fromUtf8("splitter"));
  splitter->setGeometry(QRect(5, 5, 556, 271));
  splitter->setFrameShape(QFrame::Box);
  splitter->setFrameShadow(QFrame::Plain);
  splitter->setLineWidth(1);
  splitter->setMidLineWidth(3);
  splitter->setOrientation(Qt::Horizontal);
  splitter->setOpaqueResize(true);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  toolBox = new QToolBox(splitter);
  splitter->addWidget(toolBox);
  toolBox->setObjectName(QString::fromUtf8("toolBox"));
  toolBox->setMinimumSize(QSize(150, 0));
  toolBox->setMaximumSize(QSize(300, 16777215));


  for (int i = 0; i < TESTMOD; ++i)
    {
      pagemod[i] = new QWidget();
      toolBox->addItem(pagemod[i], QString::fromUtf8("Mod")+QString::number(i));
     
      QByteArray ba = QString(QString::fromUtf8("Mod ")+QString::number(i)).toLatin1();
      toolBox->setItemText(toolBox->indexOf(pagemod[i]), QApplication::translate("BasicSettings", ba.data(), nullptr));
      pagemod[i]->setObjectName(QString::fromUtf8("pagemod")+QString::number(i));
      pagemod[i]->setGeometry(QRect(0, 0, 150, 167));

      gridpagemod[i] = new QGridLayout(pagemod[i]);
      gridpagemod[i]->setSpacing(3);
      //gridpagemod[i]->setContentsMargins(11, 11, 11, 11);//左侧、顶部、右侧和底部边距
      gridpagemod[i]->setObjectName(QString::fromUtf8("gridpagemod")+QString::number(i));


      QLabel *lab00 = new QLabel("Version");
      lab00->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab00, 0, 0, 1, 1);

      QLabel *lab01 = new QLabel("F");
      lab01->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab01, 0, 1, 1, 1);
      
      QLabel *lab10 = new QLabel("Sample");
      lab10->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab10, 1, 0, 1, 1);

      QLabel *lab11 = new QLabel("100");
      lab11->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab11, 1, 1, 1, 1);
      
      QLabel *lab20 = new QLabel("bit");
      lab20->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab20, 2, 0, 1, 1);

      QLabel *lab21 = new QLabel("14");
      lab21->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab21, 2, 1, 1, 1);
      
      QLabel *lab30 = new QLabel("Channel");
      lab30->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab30, 3, 0, 1, 1);

      QLabel *lab31 = new QLabel("16");
      lab31->setAlignment(Qt::AlignCenter);
      gridpagemod[i]->addWidget(lab31, 3, 1, 1, 1);
      
    }


 
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        
  	
  tabWidget = new QTabWidget(splitter);
  splitter->addWidget(tabWidget);
  tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
  tabWidget->setTabPosition(QTabWidget::North);
  tabWidget->setTabShape(QTabWidget::Rounded);
  tabWidget->setElideMode(Qt::ElideLeft);


  for (int i = 0; i < TABNUM_BASICSETTINGS; ++i)
    {
      tab[i] = new QWidget();
      tabWidget->addTab(tab[i], QString());
      tab[i]->setObjectName(QString::fromUtf8("tab")+QString::number(i));

      verticalLayout[i] = new QVBoxLayout(tab[i]);
      verticalLayout[i]->setSpacing(6);
      verticalLayout[i]->setContentsMargins(11, 11, 11, 11);
      verticalLayout[i]->setObjectName(QString::fromUtf8("verticallayout")+QString::number(i));
      verticalLayout[i]->setContentsMargins(4, 4, 4, 4);

      switch(i)
	{
	case 0:
	  TabPolarityGainBaseline(0);
	  break;
	case 1:
	  TabDataRecord(1);
	  break;
	case 2:
	  TabTriggerFilter(2);
	  break;
	case 3:
	  TabEnergyFilter(3);
	  break;
	case 4:
	  TabWaveform(4);
	  break;
	case 5:
	  TabCFD(5);
	  break;
	case 6:
	  TabQDC(6);
	  break;
	default:
	  break;
	}
      
    }




  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  statusbar = new QStatusBar(this);
  statusbar->setObjectName(QString::fromUtf8("basicsettingsstatusbar"));
  this->setStatusBar(statusbar);

  statusinfo = new QLabel("Info: xxxxxxxxxxxxxxxxxxxxxxxxxxxx", this);
  statusbar->addWidget(statusinfo);
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  


  toolBox->setCurrentIndex(0);
  tabWidget->setCurrentIndex(0);


  setCentralWidget(splitter); //设置中心组件
  
  QMetaObject::connectSlotsByName(this);
}

BasicSettings::~BasicSettings()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void BasicSettings::on_toolBox_currentChanged(int index)
{
  std::cout << "toolBox: " << index << std::endl;

  //pgb
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[index])
	tablepgb->setRowHidden(i, false);
      else
	tablepgb->setRowHidden(i, true);
    }

  cbpgbcopych->clear();
  for (int i = 0; i < TESTCH[index]; ++i)
    {
      cbpgbcopych->addItem(QString::asprintf("ch - %02d", i));
    }

  on_chkboxpgbenabled0_clicked(chkboxpgbenabled[0]->isChecked());
  on_chkboxpgbenabled1_clicked(chkboxpgbenabled[1]->isChecked());
  on_chkboxpgbenabled2_clicked(chkboxpgbenabled[2]->isChecked());
  
  // record
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[index])
	tabledatarecord->setRowHidden(i, false);
      else
	tabledatarecord->setRowHidden(i, true);
    }

  cbrecordcopych->clear();
  for (int i = 0; i < TESTCH[index]; ++i)
    {
      cbrecordcopych->addItem(QString::asprintf("ch - %02d", i));
    }
  
  on_chkboxrecordenabled0_clicked(chkboxrecordenabled[0]->isChecked());
  on_chkboxrecordenabled1_clicked(chkboxrecordenabled[1]->isChecked());
  on_chkboxrecordenabled2_clicked(chkboxrecordenabled[2]->isChecked());
  on_chkboxrecordenabled3_clicked(chkboxrecordenabled[3]->isChecked());
  on_chkboxrecordenabled4_clicked(chkboxrecordenabled[4]->isChecked());


  // wave
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[index])
	tablewave->setRowHidden(i, false);
      else
	tablewave->setRowHidden(i, true);
    }

  cbwavecopych->clear();
  for (int i = 0; i < TESTCH[index]; ++i)
    {
      cbwavecopych->addItem(QString::asprintf("ch - %02d", i));
    }

  on_chkboxwaveenabled0_clicked(chkboxwaveenabled[0]->isChecked());
  on_chkboxwaveenabled1_clicked(chkboxwaveenabled[1]->isChecked());
  on_chkboxwaveenabled2_clicked(chkboxwaveenabled[2]->isChecked());

  // trigger
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[index])
	tabletrigger->setRowHidden(i, false);
      else
	tabletrigger->setRowHidden(i, true);
    }

  cbtriggercopych->clear();
  for (int i = 0; i < TESTCH[index]; ++i)
    {
      cbtriggercopych->addItem(QString::asprintf("ch - %02d", i));
    }

  on_chkboxtriggerenabled0_clicked(chkboxtriggerenabled[0]->isChecked());
  on_chkboxtriggerenabled1_clicked(chkboxtriggerenabled[1]->isChecked());
  on_chkboxtriggerenabled2_clicked(chkboxtriggerenabled[2]->isChecked());


  // energy
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[index])
	tableenergy->setRowHidden(i, false);
      else
	tableenergy->setRowHidden(i, true);
    }

  cbenergycopych->clear();
  for (int i = 0; i < TESTCH[index]; ++i)
    {
      cbenergycopych->addItem(QString::asprintf("ch - %02d", i));
    }

  on_chkboxenergyenabled0_clicked(chkboxenergyenabled[0]->isChecked());
  on_chkboxenergyenabled1_clicked(chkboxenergyenabled[1]->isChecked());
  on_chkboxenergyenabled2_clicked(chkboxenergyenabled[2]->isChecked());	 

  // cfd
    for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[index])
	tablecfd->setRowHidden(i, false);
      else
	tablecfd->setRowHidden(i, true);
    }

  cbcfdcopych->clear();
  for (int i = 0; i < TESTCH[index]; ++i)
    {
      cbcfdcopych->addItem(QString::asprintf("ch - %02d", i));
    }

  on_chkboxcfdenabled0_clicked(chkboxcfdenabled[0]->isChecked());
  on_chkboxcfdenabled1_clicked(chkboxcfdenabled[1]->isChecked());
  on_chkboxcfdenabled2_clicked(chkboxcfdenabled[2]->isChecked());
  on_chkboxcfdenabled3_clicked(chkboxcfdenabled[3]->isChecked());
  on_chkboxcfdenabled4_clicked(chkboxcfdenabled[4]->isChecked());

 
  // qdc
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[index])
	tableqdc->setRowHidden(i, false);
      else
	tableqdc->setRowHidden(i, true);
    }

  QTableWidgetItem *item = tableqdc->item(0, 0);
  item->setText(QString::asprintf("%d", index));
  cbqdccopych->clear();
  for (int i = 0; i < TESTCH[index]; ++i)
    {
      cbqdccopych->addItem(QString::asprintf("ch - %02d", i));
    }

  on_chkboxqdcenabled0_clicked(chkboxqdcenabled[0]->isChecked());
  on_chkboxqdcenabled1_clicked(chkboxqdcenabled[1]->isChecked());
  on_chkboxqdcenabled2_clicked(chkboxqdcenabled[2]->isChecked());
  on_chkboxqdcenabled3_clicked(chkboxqdcenabled[3]->isChecked());
  on_chkboxqdcenabled4_clicked(chkboxqdcenabled[4]->isChecked());
  on_chkboxqdcenabled5_clicked(chkboxqdcenabled[5]->isChecked());
  on_chkboxqdcenabled6_clicked(chkboxqdcenabled[6]->isChecked());
  on_chkboxqdcenabled7_clicked(chkboxqdcenabled[7]->isChecked());


  statusinfo->setText(QString::asprintf("Change module %d done", index));
  
}


void BasicSettings::on_tabWidget_currentChanged(int index)
{
  std::cout << "tab: "<< index << std::endl;

  statusinfo->setText(QString::asprintf("Change tab %d done", index));
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




// 
// BasicSettings.cc ends here
