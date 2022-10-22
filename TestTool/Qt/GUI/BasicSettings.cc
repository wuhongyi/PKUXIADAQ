// BasicSettings.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 21 20:26:55 2022 (+0800)
// Last-Updated: 六 10月 22 14:19:16 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include "MainWindow.hh"
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
  
  page = new QWidget();
  toolBox->addItem(page, QString::fromUtf8("QListWidget\346\223\215\344\275\234"));
  page->setObjectName(QString::fromUtf8("page"));
  page->setGeometry(QRect(0, 0, 150, 167));


  gridLayout = new QGridLayout(page);
  gridLayout->setSpacing(6);
  gridLayout->setContentsMargins(11, 11, 11, 11);
  gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
  
  tBtnListIni = new QToolButton(page);
  gridLayout->addWidget(tBtnListIni, 0, 0, 1, 1);
  tBtnListIni->setObjectName(QString::fromUtf8("tBtnListIni"));
  tBtnListIni->setMinimumSize(QSize(120, 25));
  tBtnListIni->setPopupMode(QToolButton::DelayedPopup);
  tBtnListIni->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  tBtnListIni->setAutoRaise(false);
  tBtnListIni->setArrowType(Qt::NoArrow);

  tBtnListClear = new QToolButton(page);
  gridLayout->addWidget(tBtnListClear, 1, 0, 1, 1);
  tBtnListClear->setObjectName(QString::fromUtf8("tBtnListClear"));
  tBtnListClear->setMinimumSize(QSize(120, 25));
  tBtnListClear->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  tBtnListInsert = new QToolButton(page);
  gridLayout->addWidget(tBtnListInsert, 2, 0, 1, 1);
  tBtnListInsert->setObjectName(QString::fromUtf8("tBtnListInsert"));
  tBtnListInsert->setMinimumSize(QSize(120, 25));
  tBtnListInsert->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  tBtnListAppend = new QToolButton(page);
  gridLayout->addWidget(tBtnListAppend, 3, 0, 1, 1);
  tBtnListAppend->setObjectName(QString::fromUtf8("tBtnListAppend"));
  tBtnListAppend->setMinimumSize(QSize(120, 25));
  tBtnListAppend->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  tBtnListDelete = new QToolButton(page);
  gridLayout->addWidget(tBtnListDelete, 4, 0, 1, 1);
  tBtnListDelete->setObjectName(QString::fromUtf8("tBtnListDelete"));
  tBtnListDelete->setMinimumSize(QSize(120, 25));
  tBtnListDelete->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);



  

  page_2 = new QWidget();
  toolBox->addItem(page_2, QString::fromUtf8("QTreeWidget\346\223\215\344\275\234"));
  page_2->setObjectName(QString::fromUtf8("page_2"));
  page_2->setGeometry(QRect(0, 0, 150, 167));


  verticalLayout_4 = new QVBoxLayout(page_2);
  verticalLayout_4->setSpacing(6);
  verticalLayout_4->setContentsMargins(11, 11, 11, 11);
  verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
  
  groupBox_4 = new QGroupBox(page_2);
  groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
  
  verticalLayout_3 = new QVBoxLayout(groupBox_4);
  verticalLayout_3->setSpacing(6);
  verticalLayout_3->setContentsMargins(11, 11, 11, 11);
  verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
  
  label_2 = new QLabel(groupBox_4);
  verticalLayout_3->addWidget(label_2);
  label_2->setObjectName(QString::fromUtf8("label_2"));

  lineEdit = new QLineEdit(groupBox_4);
  verticalLayout_3->addWidget(lineEdit);
  lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

  spinBox = new QSpinBox(groupBox_4);
  verticalLayout_3->addWidget(spinBox);
  spinBox->setObjectName(QString::fromUtf8("spinBox"));


  verticalLayout_4->addWidget(groupBox_4);

  verticalSpacer = new QSpacerItem(20, 119, QSizePolicy::Minimum, QSizePolicy::Expanding);
  verticalLayout_4->addItem(verticalSpacer);



  page_3 = new QWidget();
  toolBox->addItem(page_3, QString::fromUtf8("QTableWidget\346\223\215\344\275\234"));
  page_3->setObjectName(QString::fromUtf8("page_3"));
  page_3->setGeometry(QRect(0, 0, 150, 167));
 
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
        
  	
  tabWidget = new QTabWidget(splitter);
  splitter->addWidget(tabWidget);
  tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
  tabWidget->setTabPosition(QTabWidget::North);
  tabWidget->setTabShape(QTabWidget::Rounded);
  tabWidget->setElideMode(Qt::ElideLeft);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  


  tab_3 = new QWidget();
  tabWidget->addTab(tab_3, QString());
  tab_3->setObjectName(QString::fromUtf8("tab_3"));


  verticalLayout = new QVBoxLayout(tab_3);
  verticalLayout->setSpacing(6);
  verticalLayout->setContentsMargins(11, 11, 11, 11);
  verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
  verticalLayout->setContentsMargins(4, 4, 4, 4);
  
  groupBox = new QGroupBox(tab_3);
  verticalLayout->addWidget(groupBox);
  groupBox->setObjectName(QString::fromUtf8("groupBox"));
  
  horizontalLayout = new QHBoxLayout(groupBox);
  horizontalLayout->setSpacing(10);
  horizontalLayout->setContentsMargins(11, 11, 11, 11);
  horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
  horizontalLayout->setContentsMargins(-1, 2, -1, 2);
  
  label = new QLabel(groupBox);
  horizontalLayout->addWidget(label);
  label->setObjectName(QString::fromUtf8("label"));

  editCutItemText = new QLineEdit(groupBox);
  horizontalLayout->addWidget(editCutItemText);
  editCutItemText->setObjectName(QString::fromUtf8("editCutItemText"));
  editCutItemText->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
  editCutItemText->setClearButtonEnabled(false);

  chkBoxListEditable = new QCheckBox(groupBox);
  horizontalLayout->addWidget(chkBoxListEditable);
  chkBoxListEditable->setObjectName(QString::fromUtf8("chkBoxListEditable"));



  groupBox_2 = new QGroupBox(tab_3);
  verticalLayout->addWidget(groupBox_2);
  groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));

  
  horizontalLayout_2 = new QHBoxLayout(groupBox_2);
  horizontalLayout_2->setSpacing(6);
  horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
  horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
  horizontalLayout_2->setContentsMargins(-1, 2, -1, 2);
  
  tBtnSelectItem = new QToolButton(groupBox_2);
  horizontalLayout_2->addWidget(tBtnSelectItem);
  tBtnSelectItem->setObjectName(QString::fromUtf8("tBtnSelectItem"));
  tBtnSelectItem->setMinimumSize(QSize(100, 25));
  tBtnSelectItem->setPopupMode(QToolButton::MenuButtonPopup);
  tBtnSelectItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  tBtnSelALL = new QToolButton(groupBox_2);
  horizontalLayout_2->addWidget(tBtnSelALL);
  tBtnSelALL->setObjectName(QString::fromUtf8("tBtnSelALL"));
  tBtnSelALL->setMinimumSize(QSize(70, 25));

  tBtnSelNone = new QToolButton(groupBox_2);
  horizontalLayout_2->addWidget(tBtnSelNone);
  tBtnSelNone->setObjectName(QString::fromUtf8("tBtnSelNone"));
  tBtnSelNone->setMinimumSize(QSize(70, 25));

  tBtnSelInvs = new QToolButton(groupBox_2);
  horizontalLayout_2->addWidget(tBtnSelInvs);
  tBtnSelInvs->setObjectName(QString::fromUtf8("tBtnSelInvs"));
  tBtnSelInvs->setMinimumSize(QSize(70, 25));

  

  listWidget = new QListWidget(tab_3);
  verticalLayout->addWidget(listWidget);

  // QIcon icon10;
  // icon10.addFile(QString::fromUtf8(":/images/icons/check2.ico"), QSize(), QIcon::Normal, QIcon::Off);
  QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listWidget);
  __qlistwidgetitem->setCheckState(Qt::Checked);
  // __qlistwidgetitem->setIcon(icon10);
  __qlistwidgetitem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
  QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listWidget);
  __qlistwidgetitem1->setCheckState(Qt::Unchecked);
  // __qlistwidgetitem1->setIcon(icon10);
  __qlistwidgetitem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
  listWidget->setObjectName(QString::fromUtf8("listWidget"));
  listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  listWidget->setLayoutMode(QListView::SinglePass);
  listWidget->setViewMode(QListView::ListMode);
  listWidget->setSelectionRectVisible(false);


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  tab = new QWidget();
  tabWidget->addTab(tab, QString());
  tab->setObjectName(QString::fromUtf8("tab"));
	
  verticalLayout_2 = new QVBoxLayout(tab);
  verticalLayout_2->setSpacing(6);
  verticalLayout_2->setContentsMargins(11, 11, 11, 11);
  verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
	
  groupBox_3 = new QGroupBox(tab);
  verticalLayout_2->addWidget(groupBox_3);
  groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
	
  horizontalLayout_3 = new QHBoxLayout(groupBox_3);
  horizontalLayout_3->setSpacing(6);
  horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
  horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

       

        
  tab_2 = new QWidget();
  tabWidget->addTab(tab_2, QString());
  tab_2->setObjectName(QString::fromUtf8("tab_2"));
        

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  






  tBtnListIni->setText(QApplication::translate("BasicSettings", "tBtnListIni", nullptr));
  tBtnListClear->setText(QApplication::translate("BasicSettings", "tBtnListClear", nullptr));
  tBtnListInsert->setText(QApplication::translate("BasicSettings", "tBtnListInsert", nullptr));
  tBtnListAppend->setText(QApplication::translate("BasicSettings", "tBtnListAppend", nullptr));
  tBtnListDelete->setText(QApplication::translate("BasicSettings", "tBtnListDelete", nullptr));


  toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("BasicSettings", "QListWidget\346\223\215\344\275\234", nullptr));
  groupBox_4->setTitle(QApplication::translate("BasicSettings", "GroupBox", nullptr));
  label_2->setText(QApplication::translate("BasicSettings", "TextLabel", nullptr));
  toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("BasicSettings", "QTreeWidget\346\223\215\344\275\234", nullptr));
  toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("BasicSettings", "QTableWidget\346\223\215\344\275\234", nullptr));
  groupBox->setTitle(QString());
  label->setText(QApplication::translate("BasicSettings", "\345\275\223\345\211\215\351\241\271\345\217\230\345\214\226", nullptr));
  chkBoxListEditable->setText(QApplication::translate("BasicSettings", "\345\217\257\347\274\226\350\276\221", nullptr));
  groupBox_2->setTitle(QString());
  tBtnSelectItem->setText(QApplication::translate("BasicSettings", "tBtnSelectItem", nullptr));
  tBtnSelALL->setText(QApplication::translate("BasicSettings", "tBtnSelALL", nullptr));
  tBtnSelNone->setText(QApplication::translate("BasicSettings", "tBtnSelNone", nullptr));
  tBtnSelInvs->setText(QApplication::translate("BasicSettings", "tBtnSelInvs", nullptr));

  

  const bool __sortingEnabled = listWidget->isSortingEnabled();
  listWidget->setSortingEnabled(false);
  QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
  ___qlistwidgetitem->setText(QApplication::translate("BasicSettings", "New Item", nullptr));
  QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
  ___qlistwidgetitem1->setText(QApplication::translate("BasicSettings", "New Item2", nullptr));
  listWidget->setSortingEnabled(__sortingEnabled);

  tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("BasicSettings", "QListWidget", nullptr));
  groupBox_3->setTitle(QString());
  tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("BasicSettings", "QTreeWidget", nullptr));
  tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("BasicSettings", "QTableWidget", nullptr));

  
  toolBox->setCurrentIndex(0);
  tabWidget->setCurrentIndex(0);


  setCentralWidget(splitter); //设置中心组件
  
  QMetaObject::connectSlotsByName(this);
}

BasicSettings::~BasicSettings()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




// 
// BasicSettings.cc ends here
