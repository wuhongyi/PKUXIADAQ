// BasicSettings.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 21 20:26:00 2022 (+0800)
// Last-Updated: 一 4月 10 10:13:48 2023 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 64
// URL: http://wuhongyi.cn 

#ifndef _BASICSETTINGS_H_
#define _BASICSETTINGS_H_

#include "Global.hh"
#include "DeviceHandle.hh"

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QGridLayout>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define TABNUM_BASICSETTINGS 8


class MainWindow;

class BasicSettings : public QMainWindow
{
  Q_OBJECT

public:
  BasicSettings(MainWindow *parent, DeviceHandle *device);
  virtual ~BasicSettings();


private:
  void TabPolarityGainBaseline(int n);// +/-, Gain, baseline
  void TabDataRecord(int n);// GC, TC, EQS, ERB, ETS
  void TabCFD(int n);// ECT, DELAY, FRAC, THRE, delay filter
  void TabTriggerFilter(int n);// rise, top, thre
  void TabEnergyFilter(int n);// rise, top, tau
  void TabWaveform(int n);// delay, trace, dec
  void TabQDC(int n);// q0-q7
  void TabMCADEBUG(int n);// 
  
private:

  // int TESTCH[13] = {32, 16, 64, 16, 32, 32, 16, 64, 16, 16, 32, 64, 16};
  // mDevice->GetNumberOfChannels(i)
  
  // QGridLayout 网格化布局
  // QVBoxLayout 上下排列
  // QHBoxLayout 水平排列
  
  MainWindow * const mMainWindow;
  DeviceHandle *mDevice = nullptr;
  
  QWidget *centralWidget;
  QSplitter *splitter;// 左右区域分割
  QToolBox *toolBox;// 内部装 QWidget page
  QTabWidget *tabWidget; // 内部装 QWidget tab

  QStatusBar *statusbar;
  QLabel *statusinfo;
  
  // QToolBox
  QWidget *pagemod[MAXMODULENUM];
  QGridLayout *gridpagemod[MAXMODULENUM];//

  // QTabWidget
  QWidget *tab[TABNUM_BASICSETTINGS];
  QVBoxLayout *verticalLayout[TABNUM_BASICSETTINGS];


  //PolarityGainBaseline
  QTableWidget *tablepgb;					   
  
  QGroupBox *groupboxpgb0;
  QHBoxLayout *horizontalgbpgb0;
  QCheckBox *chkboxpgbenabled[3];

  QGroupBox *groupboxpgb1;
  QHBoxLayout *horizontalgbpgb1;
  QRadioButton *rbtablepgbrow;
  QRadioButton *rbtablepgbcol;
  QRadioButton *rbtablepgbitem;
  
  
  QGroupBox *groupboxpgb2;
  QHBoxLayout *horizontalgbpgb2;
  QPushButton *pbtablepgbloadselected;
  QPushButton *pbtablepgbloadall;
  QPushButton *pbtablepgbapplyselected;
  QPushButton *pbtablepgbapplyall;

  QGroupBox *groupboxpgb3;
  QHBoxLayout *horizontalgbpgb3;
  QPushButton *pbtablepgbadjustoffset;
  QPushButton *pbtablepgbblcutfinder;
  
  QComboBox *cbpgbcopych;
  QPushButton *pbtablepgbcopy;

  
  // data record
  QTableWidget *tabledatarecord;
  
  QGroupBox *groupboxrecord0;
  QHBoxLayout *horizontalgbrecord0;
  QCheckBox *chkboxrecordenabled[6];

  QGroupBox *groupboxrecord1;
  QHBoxLayout *horizontalgbrecord1;
  QRadioButton *rbtablerecordrow;
  QRadioButton *rbtablerecordcol;
  QRadioButton *rbtablerecorditem;

  QGroupBox *groupboxrecord2;
  QHBoxLayout *horizontalgbrecord2;
  QPushButton *pbtablerecordloadselected;
  QPushButton *pbtablerecordloadall;
  QPushButton *pbtablerecordapplyselected;
  QPushButton *pbtablerecordapplyall;

  QGroupBox *groupboxrecord3;
  QHBoxLayout *horizontalgbrecord3;
  QComboBox *cbrecordcopych;
  QPushButton *pbtablerecordcopy;

  // wave
  QTableWidget *tablewave;					   
  
  QGroupBox *groupboxwave0;
  QHBoxLayout *horizontalgbwave0;
  QCheckBox *chkboxwaveenabled[8];

  QGroupBox *groupboxwave1;
  QHBoxLayout *horizontalgbwave1;
  QRadioButton *rbtablewaverow;
  QRadioButton *rbtablewavecol;
  QRadioButton *rbtablewaveitem;
  
  
  QGroupBox *groupboxwave2;
  QHBoxLayout *horizontalgbwave2;
  QPushButton *pbtablewaveloadselected;
  QPushButton *pbtablewaveloadall;
  QPushButton *pbtablewaveapplyselected;
  QPushButton *pbtablewaveapplyall;

  QGroupBox *groupboxwave3;
  QHBoxLayout *horizontalgbwave3;
  QComboBox *cbwavecopych;
  QPushButton *pbtablewavecopy;  

  // trigger
  QTableWidget *tabletrigger;					   
  
  QGroupBox *groupboxtrigger0;
  QHBoxLayout *horizontalgbtrigger0;
  QCheckBox *chkboxtriggerenabled[3];

  QGroupBox *groupboxtrigger1;
  QHBoxLayout *horizontalgbtrigger1;
  QRadioButton *rbtabletriggerrow;
  QRadioButton *rbtabletriggercol;
  QRadioButton *rbtabletriggeritem;
  
  QGroupBox *groupboxtrigger2;
  QHBoxLayout *horizontalgbtrigger2;
  QPushButton *pbtabletriggerloadselected;
  QPushButton *pbtabletriggerloadall;
  QPushButton *pbtabletriggerapplyselected;
  QPushButton *pbtabletriggerapplyall;

  QGroupBox *groupboxtrigger3;
  QHBoxLayout *horizontalgbtrigger3;
  QComboBox *cbtriggercopych;
  QPushButton *pbtabletriggercopy;

  // energy
  QTableWidget *tableenergy;					   
  
  QGroupBox *groupboxenergy0;
  QHBoxLayout *horizontalgbenergy0;
  QCheckBox *chkboxenergyenabled[3];

  QGroupBox *groupboxenergy1;
  QHBoxLayout *horizontalgbenergy1;
  QRadioButton *rbtableenergyrow;
  QRadioButton *rbtableenergycol;
  QRadioButton *rbtableenergyitem;
  
  QGroupBox *groupboxenergy2;
  QHBoxLayout *horizontalgbenergy2;
  QPushButton *pbtableenergyloadselected;
  QPushButton *pbtableenergyloadall;
  QPushButton *pbtableenergyapplyselected;
  QPushButton *pbtableenergyapplyall;

  QGroupBox *groupboxenergy3;
  QHBoxLayout *horizontalgbenergy3;

  QComboBox *cbenergyrange;
  QPushButton *pbtableenergyfindtau;

  
  QComboBox *cbenergycopych;
  QPushButton *pbtableenergycopy;

  // CFD
  QTableWidget *tablecfd;					   
  
  QGroupBox *groupboxcfd0;
  QHBoxLayout *horizontalgbcfd0;
  QCheckBox *chkboxcfdenabled[5];

  QGroupBox *groupboxcfd1;
  QHBoxLayout *horizontalgbcfd1;
  QRadioButton *rbtablecfdrow;
  QRadioButton *rbtablecfdcol;
  QRadioButton *rbtablecfditem;
  
  
  QGroupBox *groupboxcfd2;
  QHBoxLayout *horizontalgbcfd2;
  QPushButton *pbtablecfdloadselected;
  QPushButton *pbtablecfdloadall;
  QPushButton *pbtablecfdapplyselected;
  QPushButton *pbtablecfdapplyall;

  QGroupBox *groupboxcfd3;
  QHBoxLayout *horizontalgbcfd3;
  QComboBox *cbcfdcopych;
  QPushButton *pbtablecfdcopy;  

  // QDC
  QTableWidget *tableqdc;					   
  
  QGroupBox *groupboxqdc0;
  QHBoxLayout *horizontalgbqdc0;
  QCheckBox *chkboxqdcenabled[8];

  QGroupBox *groupboxqdc1;
  QHBoxLayout *horizontalgbqdc1;
  QRadioButton *rbtableqdcrow;
  QRadioButton *rbtableqdccol;
  QRadioButton *rbtableqdcitem;
  
  
  QGroupBox *groupboxqdc2;
  QHBoxLayout *horizontalgbqdc2;
  QPushButton *pbtableqdcloadselected;
  QPushButton *pbtableqdcloadall;
  QPushButton *pbtableqdcapplyselected;
  QPushButton *pbtableqdcapplyall;

  QGroupBox *groupboxqdc3;
  QHBoxLayout *horizontalgbqdc3;
  QComboBox *cbqdccopych;
  QPushButton *pbtableqdccopy;
  // QComboBox *cbqdccopymod;

  // MCA/Debug
  QTableWidget *tablemcadebug;

  QGroupBox *groupboxmcadebug0;
  QHBoxLayout *horizontalgbmcadebug0;
  QCheckBox *chkboxmcadebugenabled[3];

  QGroupBox *groupboxmcadebug1;
  QHBoxLayout *horizontalgbmcadebug1;
  QRadioButton *rbtablemcadebugrow;
  QRadioButton *rbtablemcadebugcol;
  QRadioButton *rbtablemcadebugitem;
  
  QGroupBox *groupboxmcadebug2;
  QHBoxLayout *horizontalgbmcadebug2;
  QPushButton *pbtablemcadebugloadselected;
  QPushButton *pbtablemcadebugloadall;
  QPushButton *pbtablemcadebugapplyselected;
  QPushButton *pbtablemcadebugapplyall;

  QGroupBox *groupboxmcadebug3;
  QHBoxLayout *horizontalgbmcadebug3;
  QComboBox *cbmcadebugcopych;
  QPushButton *pbtablemcadebugcopy;



			     

			     
private slots:
  void on_toolBox_currentChanged(int index); //ToolBox当前组变化时，显示TabWidget相应的页面
  void on_tabWidget_currentChanged(int index);

  //PolarityGainBaseline
  void on_rbtablepgbrow_clicked(); 
  void on_rbtablepgbitem_clicked();
  void on_rbtablepgbcol_clicked();

  void on_pbtablepgbloadselected_clicked();
  void on_pbtablepgbloadall_clicked();
  void on_pbtablepgbapplyselected_clicked();
  void on_pbtablepgbapplyall_clicked();

  void on_pbtablepgbcopy_clicked();

  void on_pbtablepgbadjustoffset_clicked();
  void on_pbtablepgbblcutfinder_clicked();
  
  void on_chkboxpgbenabled0_clicked(bool checked);
  void on_chkboxpgbenabled1_clicked(bool checked);
  void on_chkboxpgbenabled2_clicked(bool checked);

  void TablePolarityGainBaselineCheckState(int row, int column);
  
  // record
  void on_rbtablerecordrow_clicked(); 
  void on_rbtablerecorditem_clicked();
  void on_rbtablerecordcol_clicked();

  void on_pbtablerecordloadselected_clicked();
  void on_pbtablerecordloadall_clicked();
  void on_pbtablerecordapplyselected_clicked();
  void on_pbtablerecordapplyall_clicked();

  void on_pbtablerecordcopy_clicked();

  void on_chkboxrecordenabled0_clicked(bool checked);
  void on_chkboxrecordenabled1_clicked(bool checked);
  void on_chkboxrecordenabled2_clicked(bool checked);
  void on_chkboxrecordenabled3_clicked(bool checked);
  void on_chkboxrecordenabled4_clicked(bool checked);
  void on_chkboxrecordenabled5_clicked(bool checked);
  
  void TableDataRecordCheckState(int row, int column);
  


  // wave
  void on_rbtablewaverow_clicked(); 
  void on_rbtablewaveitem_clicked();
  void on_rbtablewavecol_clicked();

  void on_pbtablewaveloadselected_clicked();
  void on_pbtablewaveloadall_clicked();
  void on_pbtablewaveapplyselected_clicked();
  void on_pbtablewaveapplyall_clicked();

  void on_pbtablewavecopy_clicked();

  void on_chkboxwaveenabled0_clicked(bool checked);
  void on_chkboxwaveenabled1_clicked(bool checked);
  void on_chkboxwaveenabled2_clicked(bool checked);


  // trigger
  void on_rbtabletriggerrow_clicked(); 
  void on_rbtabletriggeritem_clicked();
  void on_rbtabletriggercol_clicked();

  void on_pbtabletriggerloadselected_clicked();
  void on_pbtabletriggerloadall_clicked();
  void on_pbtabletriggerapplyselected_clicked();
  void on_pbtabletriggerapplyall_clicked();

  void on_pbtabletriggercopy_clicked();

  void on_chkboxtriggerenabled0_clicked(bool checked);
  void on_chkboxtriggerenabled1_clicked(bool checked);
  void on_chkboxtriggerenabled2_clicked(bool checked);

  // energy
  void on_rbtableenergyrow_clicked(); 
  void on_rbtableenergyitem_clicked();
  void on_rbtableenergycol_clicked();

  void on_pbtableenergyloadselected_clicked();
  void on_pbtableenergyloadall_clicked();
  void on_pbtableenergyapplyselected_clicked();
  void on_pbtableenergyapplyall_clicked();

  void on_pbtableenergyfindtau_clicked();
  
  void on_pbtableenergycopy_clicked();

  void on_chkboxenergyenabled0_clicked(bool checked);
  void on_chkboxenergyenabled1_clicked(bool checked);
  void on_chkboxenergyenabled2_clicked(bool checked);

  void on_cbenergyrange_currentIndexChanged(int index);
  
  // cfd
  void on_rbtablecfdrow_clicked(); 
  void on_rbtablecfditem_clicked();
  void on_rbtablecfdcol_clicked();

  void on_pbtablecfdloadselected_clicked();
  void on_pbtablecfdloadall_clicked();
  void on_pbtablecfdapplyselected_clicked();
  void on_pbtablecfdapplyall_clicked();

  void on_pbtablecfdcopy_clicked();

  void on_chkboxcfdenabled0_clicked(bool checked);
  void on_chkboxcfdenabled1_clicked(bool checked);
  void on_chkboxcfdenabled2_clicked(bool checked);
  void on_chkboxcfdenabled3_clicked(bool checked);
  void on_chkboxcfdenabled4_clicked(bool checked);

  void TableCFDCheckState(int row, int column);
  
  // qdc
  void on_rbtableqdcrow_clicked(); 
  void on_rbtableqdcitem_clicked();
  void on_rbtableqdccol_clicked();

  void on_pbtableqdcloadselected_clicked();
  void on_pbtableqdcloadall_clicked();
  void on_pbtableqdcapplyselected_clicked();
  void on_pbtableqdcapplyall_clicked();

  void on_pbtableqdccopy_clicked();

  void on_chkboxqdcenabled0_clicked(bool checked);
  void on_chkboxqdcenabled1_clicked(bool checked);
  void on_chkboxqdcenabled2_clicked(bool checked);
  void on_chkboxqdcenabled3_clicked(bool checked);
  void on_chkboxqdcenabled4_clicked(bool checked);
  void on_chkboxqdcenabled5_clicked(bool checked);
  void on_chkboxqdcenabled6_clicked(bool checked);
  void on_chkboxqdcenabled7_clicked(bool checked);

  // void on_cbqdccopymod_currentIndexChanged(int index);

  // mca/debug
  void on_rbtablemcadebugrow_clicked(); 
  void on_rbtablemcadebugitem_clicked();
  void on_rbtablemcadebugcol_clicked();

  void on_pbtablemcadebugloadselected_clicked();
  void on_pbtablemcadebugloadall_clicked();
  void on_pbtablemcadebugapplyselected_clicked();
  void on_pbtablemcadebugapplyall_clicked();

  void on_pbtablemcadebugcopy_clicked();

  void on_chkboxmcadebugenabled0_clicked(bool checked);
  void on_chkboxmcadebugenabled1_clicked(bool checked);
  void on_chkboxmcadebugenabled2_clicked(bool checked);


  
};


#endif /* _BASICSETTINGS_H_ */

// 
// BasicSettings.hh ends here


  // QWidget *page;
  // QGridLayout *gridLayout;
  // QToolButton *tBtnListIni;
  // QToolButton *tBtnListClear;
  // QToolButton *tBtnListInsert;
  // QToolButton *tBtnListAppend;
  // QToolButton *tBtnListDelete;

  // QWidget *page_2;
  // QVBoxLayout *verticalLayout_4;
  // QGroupBox *groupBox_4;
  // QVBoxLayout *verticalLayout_3;
  // QLabel *label_2;
  // QLineEdit *lineEdit;
  // QSpinBox *spinBox;
  // QSpacerItem *verticalSpacer;

  // QWidget *page_3;

  // QListWidget *listWidget;
  
  // QWidget *tab_3;
  // QVBoxLayout *verticalLayout;
  // QGroupBox *groupBox;
  // QHBoxLayout *horizontalLayout;
  // QLabel *label;
  // QLineEdit *editCutItemText;
  // QCheckBox *chkBoxListEditable;
  
  // QGroupBox *groupBox_2;
  // QHBoxLayout *horizontalLayout_2;
  // QToolButton *tBtnSelectItem;
  // QToolButton *tBtnSelALL;
  // QToolButton *tBtnSelNone;
  // QToolButton *tBtnSelInvs;

  // QWidget *tab;
  // QVBoxLayout *verticalLayout_2;
  // QGroupBox *groupBox_3;
  // QHBoxLayout *horizontalLayout_3;
  
  // QWidget *tab_2;







  // page = new QWidget();
  // toolBox->addItem(page, QString::fromUtf8("QListWidget\346\223\215\344\275\234"));
  // page->setObjectName(QString::fromUtf8("page"));
  // page->setGeometry(QRect(0, 0, 150, 167));


  // gridLayout = new QGridLayout(page);
  // gridLayout->setSpacing(6);
  // gridLayout->setContentsMargins(11, 11, 11, 11);
  // gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
  
  // tBtnListIni = new QToolButton(page);
  // gridLayout->addWidget(tBtnListIni, 0, 0, 1, 1);
  // tBtnListIni->setObjectName(QString::fromUtf8("tBtnListIni"));
  // tBtnListIni->setMinimumSize(QSize(120, 25));
  // tBtnListIni->setPopupMode(QToolButton::DelayedPopup);
  // tBtnListIni->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  // tBtnListIni->setAutoRaise(false);
  // tBtnListIni->setArrowType(Qt::NoArrow);

  // tBtnListClear = new QToolButton(page);
  // gridLayout->addWidget(tBtnListClear, 1, 0, 1, 1);
  // tBtnListClear->setObjectName(QString::fromUtf8("tBtnListClear"));
  // tBtnListClear->setMinimumSize(QSize(120, 25));
  // tBtnListClear->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  // tBtnListInsert = new QToolButton(page);
  // gridLayout->addWidget(tBtnListInsert, 2, 0, 1, 1);
  // tBtnListInsert->setObjectName(QString::fromUtf8("tBtnListInsert"));
  // tBtnListInsert->setMinimumSize(QSize(120, 25));
  // tBtnListInsert->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  // tBtnListAppend = new QToolButton(page);
  // gridLayout->addWidget(tBtnListAppend, 3, 0, 1, 1);
  // tBtnListAppend->setObjectName(QString::fromUtf8("tBtnListAppend"));
  // tBtnListAppend->setMinimumSize(QSize(120, 25));
  // tBtnListAppend->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  // tBtnListDelete = new QToolButton(page);
  // gridLayout->addWidget(tBtnListDelete, 4, 0, 1, 1);
  // tBtnListDelete->setObjectName(QString::fromUtf8("tBtnListDelete"));
  // tBtnListDelete->setMinimumSize(QSize(120, 25));
  // tBtnListDelete->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);



  

  // page_2 = new QWidget();
  // toolBox->addItem(page_2, QString::fromUtf8("QTreeWidget\346\223\215\344\275\234"));
  // page_2->setObjectName(QString::fromUtf8("page_2"));
  // page_2->setGeometry(QRect(0, 0, 150, 167));


  // verticalLayout_4 = new QVBoxLayout(page_2);
  // verticalLayout_4->setSpacing(6);
  // verticalLayout_4->setContentsMargins(11, 11, 11, 11);
  // verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
  
  // groupBox_4 = new QGroupBox(page_2);
  // groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
  
  // verticalLayout_3 = new QVBoxLayout(groupBox_4);
  // verticalLayout_3->setSpacing(6);
  // verticalLayout_3->setContentsMargins(11, 11, 11, 11);
  // verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
  
  // label_2 = new QLabel(groupBox_4);
  // verticalLayout_3->addWidget(label_2);
  // label_2->setObjectName(QString::fromUtf8("label_2"));

  // lineEdit = new QLineEdit(groupBox_4);
  // verticalLayout_3->addWidget(lineEdit);
  // lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

  // spinBox = new QSpinBox(groupBox_4);
  // verticalLayout_3->addWidget(spinBox);
  // spinBox->setObjectName(QString::fromUtf8("spinBox"));


  // verticalLayout_4->addWidget(groupBox_4);

  // verticalSpacer = new QSpacerItem(20, 119, QSizePolicy::Minimum, QSizePolicy::Expanding);
  // verticalLayout_4->addItem(verticalSpacer);



  // page_3 = new QWidget();
  // toolBox->addItem(page_3, QString::fromUtf8("QTableWidget\346\223\215\344\275\234"));
  // page_3->setObjectName(QString::fromUtf8("page_3"));
  // page_3->setGeometry(QRect(0, 0, 150, 167));
















  // tab_3 = new QWidget();
  // tabWidget->addTab(tab_3, QString());
  // tab_3->setObjectName(QString::fromUtf8("tab_3"));


  // verticalLayout = new QVBoxLayout(tab_3);
  // verticalLayout->setSpacing(6);
  // verticalLayout->setContentsMargins(11, 11, 11, 11);
  // verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
  // verticalLayout->setContentsMargins(4, 4, 4, 4);
  
  // groupBox = new QGroupBox(tab_3);
  // verticalLayout->addWidget(groupBox);
  // groupBox->setObjectName(QString::fromUtf8("groupBox"));
  
  // horizontalLayout = new QHBoxLayout(groupBox);
  // horizontalLayout->setSpacing(10);
  // horizontalLayout->setContentsMargins(11, 11, 11, 11);
  // horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
  // horizontalLayout->setContentsMargins(-1, 2, -1, 2);
  
  // label = new QLabel(groupBox);
  // horizontalLayout->addWidget(label);
  // label->setObjectName(QString::fromUtf8("label"));

  // editCutItemText = new QLineEdit(groupBox);
  // horizontalLayout->addWidget(editCutItemText);
  // editCutItemText->setObjectName(QString::fromUtf8("editCutItemText"));
  // editCutItemText->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
  // editCutItemText->setClearButtonEnabled(false);

  // chkBoxListEditable = new QCheckBox(groupBox);
  // horizontalLayout->addWidget(chkBoxListEditable);
  // chkBoxListEditable->setObjectName(QString::fromUtf8("chkBoxListEditable"));



  // groupBox_2 = new QGroupBox(tab_3);
  // verticalLayout->addWidget(groupBox_2);
  // groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));

  
  // horizontalLayout_2 = new QHBoxLayout(groupBox_2);
  // horizontalLayout_2->setSpacing(6);
  // horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
  // horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
  // horizontalLayout_2->setContentsMargins(-1, 2, -1, 2);
  
  // tBtnSelectItem = new QToolButton(groupBox_2);
  // horizontalLayout_2->addWidget(tBtnSelectItem);
  // tBtnSelectItem->setObjectName(QString::fromUtf8("tBtnSelectItem"));
  // tBtnSelectItem->setMinimumSize(QSize(100, 25));
  // tBtnSelectItem->setPopupMode(QToolButton::MenuButtonPopup);
  // tBtnSelectItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  // tBtnSelALL = new QToolButton(groupBox_2);
  // horizontalLayout_2->addWidget(tBtnSelALL);
  // tBtnSelALL->setObjectName(QString::fromUtf8("tBtnSelALL"));
  // tBtnSelALL->setMinimumSize(QSize(70, 25));

  // tBtnSelNone = new QToolButton(groupBox_2);
  // horizontalLayout_2->addWidget(tBtnSelNone);
  // tBtnSelNone->setObjectName(QString::fromUtf8("tBtnSelNone"));
  // tBtnSelNone->setMinimumSize(QSize(70, 25));

  // tBtnSelInvs = new QToolButton(groupBox_2);
  // horizontalLayout_2->addWidget(tBtnSelInvs);
  // tBtnSelInvs->setObjectName(QString::fromUtf8("tBtnSelInvs"));
  // tBtnSelInvs->setMinimumSize(QSize(70, 25));

  

  // listWidget = new QListWidget(tab_3);
  // verticalLayout->addWidget(listWidget);

  // // QIcon icon10;
  // // icon10.addFile(QString::fromUtf8(":/images/icons/check2.ico"), QSize(), QIcon::Normal, QIcon::Off);
  // QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listWidget);
  // __qlistwidgetitem->setCheckState(Qt::Checked);
  // // __qlistwidgetitem->setIcon(icon10);
  // __qlistwidgetitem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
  // QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(listWidget);
  // __qlistwidgetitem1->setCheckState(Qt::Unchecked);
  // // __qlistwidgetitem1->setIcon(icon10);
  // __qlistwidgetitem1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
  // listWidget->setObjectName(QString::fromUtf8("listWidget"));
  // listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  // listWidget->setLayoutMode(QListView::SinglePass);
  // listWidget->setViewMode(QListView::ListMode);
  // listWidget->setSelectionRectVisible(false);


  // tab = new QWidget();
  // tabWidget->addTab(tab, QString());
  // tab->setObjectName(QString::fromUtf8("tab"));
	
  // verticalLayout_2 = new QVBoxLayout(tab);
  // verticalLayout_2->setSpacing(6);
  // verticalLayout_2->setContentsMargins(11, 11, 11, 11);
  // verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
	
  // groupBox_3 = new QGroupBox(tab);
  // verticalLayout_2->addWidget(groupBox_3);
  // groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
	
  // horizontalLayout_3 = new QHBoxLayout(groupBox_3);
  // horizontalLayout_3->setSpacing(6);
  // horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
  // horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

       

        
  // tab_2 = new QWidget();
  // tabWidget->addTab(tab_2, QString());
  // tab_2->setObjectName(QString::fromUtf8("tab_2"));









  // tBtnListIni->setText(QApplication::translate("BasicSettings", "tBtnListIni", nullptr));
  // tBtnListClear->setText(QApplication::translate("BasicSettings", "tBtnListClear", nullptr));
  // tBtnListInsert->setText(QApplication::translate("BasicSettings", "tBtnListInsert", nullptr));
  // tBtnListAppend->setText(QApplication::translate("BasicSettings", "tBtnListAppend", nullptr));
  // tBtnListDelete->setText(QApplication::translate("BasicSettings", "tBtnListDelete", nullptr));


  // toolBox->setItemText(toolBox->indexOf(page), QApplication::translate("BasicSettings", "QListWidget\346\223\215\344\275\234", nullptr));
  // groupBox_4->setTitle(QApplication::translate("BasicSettings", "GroupBox", nullptr));
  // label_2->setText(QApplication::translate("BasicSettings", "TextLabel", nullptr));
  // toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("BasicSettings", "QTreeWidget\346\223\215\344\275\234", nullptr));
  // toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("BasicSettings", "QTableWidget\346\223\215\344\275\234", nullptr));
  // groupBox->setTitle(QString());




  // label->setText(QApplication::translate("BasicSettings", "\345\275\223\345\211\215\351\241\271\345\217\230\345\214\226", nullptr));
  // chkBoxListEditable->setText(QApplication::translate("BasicSettings", "\345\217\257\347\274\226\350\276\221", nullptr));
  // groupBox_2->setTitle(QString());
  // tBtnSelectItem->setText(QApplication::translate("BasicSettings", "tBtnSelectItem", nullptr));
  // tBtnSelALL->setText(QApplication::translate("BasicSettings", "tBtnSelALL", nullptr));
  // tBtnSelNone->setText(QApplication::translate("BasicSettings", "tBtnSelNone", nullptr));
  // tBtnSelInvs->setText(QApplication::translate("BasicSettings", "tBtnSelInvs", nullptr));

  

  // const bool __sortingEnabled = listWidget->isSortingEnabled();
  // listWidget->setSortingEnabled(false);
  // QListWidgetItem *___qlistwidgetitem = listWidget->item(0);
  // ___qlistwidgetitem->setText(QApplication::translate("BasicSettings", "New Item", nullptr));
  // QListWidgetItem *___qlistwidgetitem1 = listWidget->item(1);
  // ___qlistwidgetitem1->setText(QApplication::translate("BasicSettings", "New Item2", nullptr));
  // listWidget->setSortingEnabled(__sortingEnabled);

  // tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("BasicSettings", "QListWidget", nullptr));
  // groupBox_3->setTitle(QString());
  // tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("BasicSettings", "QTreeWidget", nullptr));
  // tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("BasicSettings", "QTableWidget", nullptr));





// public:
//     QAction *actListIni;
//     QAction *actListClear;
//     QAction *actListInsert;
//     QAction *actListAppend;
//     QAction *actListDelete;
//     QAction *actSelALL;
//     QAction *actSelNone;
//     QAction *actSelInvs;
//     QAction *actQuit;
//     QAction *actSelPopMenu;
    

//     QToolBar *mainToolBar;


//         MainWindow->resize(565, 332);
//         QFont font;
//         font.setPointSize(10);
//         MainWindow->setFont(font);
//         actListIni = new QAction(MainWindow);
//         actListIni->setObjectName(QString::fromUtf8("actListIni"));
//         QIcon icon;
//         icon.addFile(QString::fromUtf8(":/images/icons/128.bmp"), QSize(), QIcon::Normal, QIcon::Off);
//         actListIni->setIcon(icon);
//         actListClear = new QAction(MainWindow);
//         actListClear->setObjectName(QString::fromUtf8("actListClear"));
//         QIcon icon1;
//         icon1.addFile(QString::fromUtf8(":/images/icons/delete1.bmp"), QSize(), QIcon::Normal, QIcon::Off);
//         actListClear->setIcon(icon1);
//         actListInsert = new QAction(MainWindow);
//         actListInsert->setObjectName(QString::fromUtf8("actListInsert"));
//         QIcon icon2;
//         icon2.addFile(QString::fromUtf8(":/images/icons/424.bmp"), QSize(), QIcon::Normal, QIcon::Off);
//         actListInsert->setIcon(icon2);
//         actListAppend = new QAction(MainWindow);
//         actListAppend->setObjectName(QString::fromUtf8("actListAppend"));
//         QIcon icon3;
//         icon3.addFile(QString::fromUtf8(":/images/icons/316.bmp"), QSize(), QIcon::Normal, QIcon::Off);
//         actListAppend->setIcon(icon3);
//         actListDelete = new QAction(MainWindow);
//         actListDelete->setObjectName(QString::fromUtf8("actListDelete"));
//         QIcon icon4;
//         icon4.addFile(QString::fromUtf8(":/images/icons/324.bmp"), QSize(), QIcon::Normal, QIcon::Off);
//         actListDelete->setIcon(icon4);
//         actSelALL = new QAction(MainWindow);
//         actSelALL->setObjectName(QString::fromUtf8("actSelALL"));
//         actSelNone = new QAction(MainWindow);
//         actSelNone->setObjectName(QString::fromUtf8("actSelNone"));
//         actSelNone->setMenuRole(QAction::AboutRole);
//         actSelInvs = new QAction(MainWindow);
//         actSelInvs->setObjectName(QString::fromUtf8("actSelInvs"));
//         actQuit = new QAction(MainWindow);
//         actQuit->setObjectName(QString::fromUtf8("actQuit"));
//         QIcon icon5;
//         icon5.addFile(QString::fromUtf8(":/images/icons/exit.bmp"), QSize(), QIcon::Normal, QIcon::Off);
//         actQuit->setIcon(icon5);
//         actSelPopMenu = new QAction(MainWindow);
//         actSelPopMenu->setObjectName(QString::fromUtf8("actSelPopMenu"));
//         QIcon icon6;
//         icon6.addFile(QString::fromUtf8(":/images/icons/124.bmp"), QSize(), QIcon::Normal, QIcon::Off);
//         actSelPopMenu->setIcon(icon6);


	
//         mainToolBar = new QToolBar(MainWindow);
//         mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
//         mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//         MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

//         mainToolBar->addAction(actListIni);
//         mainToolBar->addAction(actListClear);
//         mainToolBar->addAction(actListInsert);
//         mainToolBar->addAction(actListAppend);
//         mainToolBar->addAction(actListDelete);
//         mainToolBar->addSeparator();


//         QObject::connect(actQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
//         QObject::connect(actSelPopMenu, SIGNAL(triggered()), actSelInvs, SLOT(trigger()));




        

//         actListIni->setText(QApplication::translate("MainWindow", "\345\210\235\345\247\213\345\214\226\345\210\227\350\241\250", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actListIni->setToolTip(QApplication::translate("MainWindow", "\345\210\235\345\247\213\345\214\226\345\210\227\350\241\250", nullptr));
// #endif // QT_NO_TOOLTIP
// #ifndef QT_NO_SHORTCUT
//         actListIni->setShortcut(QApplication::translate("MainWindow", "Ctrl+I", nullptr));
// #endif // QT_NO_SHORTCUT
//         actListClear->setText(QApplication::translate("MainWindow", "\346\270\205\351\231\244\345\210\227\350\241\250", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actListClear->setToolTip(QApplication::translate("MainWindow", "\346\270\205\351\231\244\345\210\227\350\241\250", nullptr));
// #endif // QT_NO_TOOLTIP
//         actListInsert->setText(QApplication::translate("MainWindow", "\346\217\222\345\205\245\351\241\271", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actListInsert->setToolTip(QApplication::translate("MainWindow", "\346\217\222\345\205\245\351\241\271", nullptr));
// #endif // QT_NO_TOOLTIP
// #ifndef QT_NO_SHORTCUT
//         actListInsert->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
// #endif // QT_NO_SHORTCUT
//         actListAppend->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\351\241\271", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actListAppend->setToolTip(QApplication::translate("MainWindow", "\346\267\273\345\212\240\351\241\271", nullptr));
// #endif // QT_NO_TOOLTIP
// #ifndef QT_NO_SHORTCUT
//         actListAppend->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
// #endif // QT_NO_SHORTCUT
//         actListDelete->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\345\275\223\345\211\215\351\241\271", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actListDelete->setToolTip(QApplication::translate("MainWindow", "\345\210\240\351\231\244\345\275\223\345\211\215\351\241\271", nullptr));
// #endif // QT_NO_TOOLTIP
// #ifndef QT_NO_SHORTCUT
//         actListDelete->setShortcut(QApplication::translate("MainWindow", "Del", nullptr));
// #endif // QT_NO_SHORTCUT
//         actSelALL->setText(QApplication::translate("MainWindow", "\345\205\250\351\200\211", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actSelALL->setToolTip(QApplication::translate("MainWindow", "\345\205\250\351\200\211", nullptr));
// #endif // QT_NO_TOOLTIP
//         actSelNone->setText(QApplication::translate("MainWindow", "\345\205\250\344\270\215\351\200\211", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actSelNone->setToolTip(QApplication::translate("MainWindow", "\345\205\250\344\270\215\351\200\211", nullptr));
// #endif // QT_NO_TOOLTIP
//         actSelInvs->setText(QApplication::translate("MainWindow", "\345\217\215\351\200\211", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actSelInvs->setToolTip(QApplication::translate("MainWindow", "\345\217\215\351\200\211", nullptr));
// #endif // QT_NO_TOOLTIP
//         actQuit->setText(QApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actQuit->setToolTip(QApplication::translate("MainWindow", "\351\200\200\345\207\272\347\250\213\345\272\217", nullptr));
// #endif // QT_NO_TOOLTIP
//         actSelPopMenu->setText(QApplication::translate("MainWindow", "\351\241\271\351\200\211\346\213\251", nullptr));
// #ifndef QT_NO_TOOLTIP
//         actSelPopMenu->setToolTip(QApplication::translate("MainWindow", "\351\241\271\351\200\211\346\213\251", nullptr));
// #endif // QT_NO_TOOLTIP


