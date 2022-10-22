// BasicSettings.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 21 20:26:55 2022 (+0800)
// Last-Updated: 六 10月 22 22:37:31 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 48
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include "MainWindow.hh"
#include "iostream"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

BasicSettings::BasicSettings(MainWindow *parent)
: QMainWindow(parent), mMainWindow(parent)
{
  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("BasicSettings"));

  this->setWindowTitle(QApplication::translate("BasicSettings", "Basic Settings", nullptr));
  
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
      gridpagemod[i]->setContentsMargins(11, 11, 11, 11);//左侧、顶部、右侧和底部边距
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
	  TabFastFilter(2);
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

void BasicSettings::TabPolarityGainBaseline(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Base", nullptr));

  
}

void BasicSettings::TabDataRecord(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Record", nullptr));


  
}

void BasicSettings::TabCFD(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "CFD", nullptr));
  
}

void BasicSettings::TabFastFilter(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Trigger", nullptr));
}

void BasicSettings::TabEnergyFilter(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Energy", nullptr));
  
}

void BasicSettings::TabWaveform(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Wave", nullptr));
}

void BasicSettings::TabQDC(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "QDC", nullptr));

  tableqdc = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tableqdc);
  // tableqdc->setSortingEnabled(false);
  tableqdc->setRowCount(MAXCHANNELNUM);
  tableqdc->setColumnCount(8);
  tableqdc->setSelectionBehavior(QAbstractItemView::SelectRows); // SelectItems

  
  QStringList header;
  header << "QDC0(us)" << "QDC1(us)" << "QDC2(us)" << "QDC3(us)" << "QDC4(us)" << "QDC5(us)" << "QDC6(us)" << "QDC7(us)";
  tableqdc->setHorizontalHeaderLabels(header);

  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 8; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);
	item->setText(QString::asprintf("%f", i*MAXCHANNELNUM+j));
        tableqdc->setItem(i, j, item);

	
      }

  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH)
	tableqdc->setRowHidden(i, false);
      else
	tableqdc->setRowHidden(i, true);
    }


  
  {
    QTableWidgetItem *item = tableqdc->item(0, 0);
    std::cout << item->text().toStdString().data()  << std::endl;
  }
  
}

void BasicSettings::on_toolBox_currentChanged(int index)
{
  std::cout << "toolBox" << index << std::endl;

  QTableWidgetItem *item = tableqdc->item(0, 0);
  item->setText(QString::asprintf("%d", index));
}


void BasicSettings::on_tabWidget_currentChanged(int index)
{

  std::cout << "tab: "<< index << std::endl;
  
}


// 
// BasicSettings.cc ends here










