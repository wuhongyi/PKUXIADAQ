// LogicSettings.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 1月 25 18:44:14 2023 (+0800)
// Last-Updated: 四 9月 28 03:40:41 2023 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 30
// URL: http://wuhongyi.cn 

#ifndef _LOGICSETTINGS_H_
#define _LOGICSETTINGS_H_

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

#define TABNUM_LOGICSETTINGS 5
#define BITUSED 14


class MainWindow;

class LogicSettings : public QMainWindow
{
  Q_OBJECT
  
public:
  LogicSettings(MainWindow *parent, DeviceHandle *device);
  virtual ~LogicSettings();

private:
  void TabModuleCSRB(int n);
  void TabChannelCSRA(int n);
  void TabDelayStretch(int n);
  void TabCoinMulti(int n);
  void TabTriggerSource(int n);
  
private:

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
  QWidget *tab[TABNUM_LOGICSETTINGS];
  QVBoxLayout *verticalLayout[TABNUM_LOGICSETTINGS];				


  // ModuleCSRB
  const static unsigned short MODULECSRBUSED = 9;
  QTableWidget *tablemodulecsrb;

  QGroupBox *groupboxmodulecsrb0;
  QHBoxLayout *horizontalgbmodulecsrb0;
  QCheckBox *chkboxmodulecsrbenabled[MODULECSRBUSED+2];

  QGroupBox *groupboxmodulecsrb1;
  QHBoxLayout *horizontalgbmodulecsrb1;
  QRadioButton *rbtablemodulecsrbrow;
  QRadioButton *rbtablemodulecsrbcol;
  QRadioButton *rbtablemodulecsrbitem;

  QGroupBox *groupboxmodulecsrb2;
  QHBoxLayout *horizontalgbmodulecsrb2;
  QPushButton *pbtablemodulecsrbloadall;
  QPushButton *pbtablemodulecsrbcheck;
  QPushButton *pbtablemodulecsrbapplyall;

  QGroupBox *groupboxmodulecsrb3;
  QHBoxLayout *horizontalgbmodulecsrb3;
  QComboBox *cbmodulecsrbcopych;
  QPushButton *pbtablemodulecsrbcopy;

  
  
  // ChannelCSRA
  QTableWidget *tablechannelcsra;
  QGroupBox *groupboxchannelcsra0;
  QHBoxLayout *horizontalgbchannelcsra0;
  QCheckBox *chkboxchannelcsraenabled[BITUSED];

  QGroupBox *groupboxchannelcsra1;
  QHBoxLayout *horizontalgbchannelcsra1;
  QRadioButton *rbtablechannelcsrarow;
  QRadioButton *rbtablechannelcsracol;
  QRadioButton *rbtablechannelcsraitem;
  
  QGroupBox *groupboxchannelcsra2;
  QHBoxLayout *horizontalgbchannelcsra2;
  QPushButton *pbtablechannelcsraloadselected;
  QPushButton *pbtablechannelcsraloadall;
  QPushButton *pbtablechannelcsraapplyselected;
  QPushButton *pbtablechannelcsraapplyall;

  QGroupBox *groupboxchannelcsra3;
  QHBoxLayout *horizontalgbchannelcsra3;
  QComboBox *cbchannelcsracopych;
  QPushButton *pbtablechannelcsracopy;  

  
  // DelayStretch
  QTableWidget *tabledelaystretch;
  QGroupBox *groupboxdelaystretch0;
  QHBoxLayout *horizontalgbdelaystretch0;
  QCheckBox *chkboxdelaystretchenabled[6];

  QGroupBox *groupboxdelaystretch1;
  QHBoxLayout *horizontalgbdelaystretch1;
  QRadioButton *rbtabledelaystretchrow;
  QRadioButton *rbtabledelaystretchcol;
  QRadioButton *rbtabledelaystretchitem;
  
  QGroupBox *groupboxdelaystretch2;
  QHBoxLayout *horizontalgbdelaystretch2;
  QPushButton *pbtabledelaystretchloadselected;
  QPushButton *pbtabledelaystretchloadall;
  QPushButton *pbtabledelaystretchapplyselected;
  QPushButton *pbtabledelaystretchapplyall;

  QGroupBox *groupboxdelaystretch3;
  QHBoxLayout *horizontalgbdelaystretch3;
  QComboBox *cbdelaystretchcopych;
  QPushButton *pbtabledelaystretchcopy;  

  
  // CoinMulti
  QTableWidget *tablecoinmulti;					   
  
  QGroupBox *groupboxcoinmulti0;
  QHBoxLayout *horizontalgbcoinmulti0;
  QCheckBox *chkboxcoinmultienabled[9];

  QGroupBox *groupboxcoinmulti1;
  QHBoxLayout *horizontalgbcoinmulti1;
  QRadioButton *rbtablecoinmultirow;
  QRadioButton *rbtablecoinmulticol;
  QRadioButton *rbtablecoinmultiitem;
  
  QGroupBox *groupboxcoinmulti2;
  QHBoxLayout *horizontalgbcoinmulti2;
  QPushButton *pbtablecoinmultiloadselected;
  QPushButton *pbtablecoinmultiloadall;
  QPushButton *pbtablecoinmultiapplyselected;
  QPushButton *pbtablecoinmultiapplyall;

  QGroupBox *groupboxcoinmulti3;
  QHBoxLayout *horizontalgbcoinmulti3;
  QComboBox *cbcoinmulticopych;
  QPushButton *pbtablecoinmulticopy;

  QLineEdit *fasttrigbpleftcoinmulti;
  QLineEdit *fasttrigbprightcoinmulti;

  // TriggerSource
  QGroupBox *groupboxtriggersource0;
  QGridLayout *gridlayoutgbtriggersource0;
  
  QComboBox *ChTri_ch0_3;//2 [24]
  QComboBox *GroupTri0;//2 [17:16]
  QComboBox *GroupTri0_0;//1 [3:0]
  QComboBox *GroupTri0_1;//1 [7:4]
  QComboBox *GroupTri0_2;//1 [11:8]
  
  QComboBox *ChTri_ch4_7;//2 [25]
  QComboBox *GroupTri1;//2 [19:18]
  QComboBox *GroupTri1_0;//1 [15:12]
  QComboBox *GroupTri1_1;//1 [19:16]
  QComboBox *GroupTri1_2;//1 [23:20]
  
  QComboBox *ChTri_ch8_11;//2 [26]
  QComboBox *GroupTri2;//2 [21:20]
  QComboBox *GroupTri2_0;//1 [27:24]
  QComboBox *GroupTri2_1;//1 [31:28]
  QComboBox *GroupTri2_2;//2 [3:0]

  QComboBox *ChTri_ch12_15;//2 [27]
  QComboBox *GroupTri3;//2 [23:22]
  QComboBox *GroupTri3_0;//2 [7:4]
  QComboBox *GroupTri3_1;//2 [11:8]
  QComboBox *GroupTri3_2;//2 [15:12]


  
  QGroupBox *groupboxtriggersource1;
  QGridLayout *gridlayoutgbtriggersource1;
  QGroupBox *groupboxtriggersource2;
  QGridLayout *gridlayoutgbtriggersource2;
  QGroupBox *groupboxtriggersource3;
  QGridLayout *gridlayoutgbtriggersource3;

  QComboBox *ChannelTrigger;//2 [31:28]
  QComboBox *InternalFastTrigger;//0 [3:0]
  QComboBox *ExternalFastTrigger;//0 [7:4]
  QComboBox *InternalValidationTrigger;//0 [11:8]
  QComboBox *GlobalFastTrigger;//0 [25:24]
  QComboBox *GlobalValidationTrigger;//0 [27:26]
  QComboBox *ExternalValidationTrigger;//0 [31:28]

  QGroupBox *groupboxtriggersource4;
  QGridLayout *gridlayoutgbtriggersource4;

  QGroupBox *groupboxtriggersource5;
  QHBoxLayout *horizontalgbtriggersource5;

  
  QPushButton *pbtabletriggersourceloadall;
  QPushButton *pbtabletriggersourceapplyall;

					 
private slots:
  
  void on_toolBox_currentChanged(int index); //ToolBox当前组变化时，显示TabWidget相应的页面
  void on_tabWidget_currentChanged(int index);  

  // ModuleCSRB
  void on_rbtablemodulecsrbrow_clicked(); 
  void on_rbtablemodulecsrbitem_clicked();
  void on_rbtablemodulecsrbcol_clicked();


  void on_pbtablemodulecsrbloadall_clicked();
  void on_pbtablemodulecsrbcheck_clicked();
  void on_pbtablemodulecsrbapplyall_clicked();

  void on_pbtablemodulecsrbcopy_clicked();

  void on_chkboxmodulecsrbenabled0_clicked(bool checked);
  void on_chkboxmodulecsrbenabled1_clicked(bool checked);
  void on_chkboxmodulecsrbenabled2_clicked(bool checked);
  void on_chkboxmodulecsrbenabled3_clicked(bool checked);
  void on_chkboxmodulecsrbenabled4_clicked(bool checked);
  void on_chkboxmodulecsrbenabled5_clicked(bool checked);
  void on_chkboxmodulecsrbenabled6_clicked(bool checked);
  void on_chkboxmodulecsrbenabled7_clicked(bool checked);
  void on_chkboxmodulecsrbenabled8_clicked(bool checked);
  void on_chkboxmodulecsrbenabled9_clicked(bool checked);
  void on_chkboxmodulecsrbenabled10_clicked(bool checked);
  

  void TableModuleCSRBCheckState(int row, int column);
  
  // ChannelCSRA
  void on_rbtablechannelcsrarow_clicked(); 
  void on_rbtablechannelcsraitem_clicked();
  void on_rbtablechannelcsracol_clicked();

  void on_pbtablechannelcsraloadselected_clicked();
  void on_pbtablechannelcsraloadall_clicked();
  void on_pbtablechannelcsraapplyselected_clicked();
  void on_pbtablechannelcsraapplyall_clicked();

  void on_pbtablechannelcsracopy_clicked();

  void on_chkboxchannelcsraenabled0_clicked(bool checked);
  void on_chkboxchannelcsraenabled1_clicked(bool checked);
  void on_chkboxchannelcsraenabled2_clicked(bool checked);
  void on_chkboxchannelcsraenabled3_clicked(bool checked);
  void on_chkboxchannelcsraenabled4_clicked(bool checked);
  void on_chkboxchannelcsraenabled5_clicked(bool checked);
  void on_chkboxchannelcsraenabled6_clicked(bool checked);
  void on_chkboxchannelcsraenabled7_clicked(bool checked);
  void on_chkboxchannelcsraenabled8_clicked(bool checked);
  void on_chkboxchannelcsraenabled9_clicked(bool checked);
  void on_chkboxchannelcsraenabled10_clicked(bool checked);
  void on_chkboxchannelcsraenabled11_clicked(bool checked);
  void on_chkboxchannelcsraenabled12_clicked(bool checked);
  void on_chkboxchannelcsraenabled13_clicked(bool checked);
  // add
  
  void TableChannelCSRACheckState(int row, int column);
  
  // DelayStretch
  void on_rbtabledelaystretchrow_clicked(); 
  void on_rbtabledelaystretchitem_clicked();
  void on_rbtabledelaystretchcol_clicked();

  void on_pbtabledelaystretchloadselected_clicked();
  void on_pbtabledelaystretchloadall_clicked();
  void on_pbtabledelaystretchapplyselected_clicked();
  void on_pbtabledelaystretchapplyall_clicked();

  void on_pbtabledelaystretchcopy_clicked();

  void on_chkboxdelaystretchenabled0_clicked(bool checked);
  void on_chkboxdelaystretchenabled1_clicked(bool checked);
  void on_chkboxdelaystretchenabled2_clicked(bool checked);
  void on_chkboxdelaystretchenabled3_clicked(bool checked);
  void on_chkboxdelaystretchenabled4_clicked(bool checked);
  void on_chkboxdelaystretchenabled5_clicked(bool checked);
  
  // CoinMulti
  void on_rbtablecoinmultirow_clicked(); 
  void on_rbtablecoinmultiitem_clicked();
  void on_rbtablecoinmulticol_clicked();

  void on_pbtablecoinmultiloadselected_clicked();
  void on_pbtablecoinmultiloadall_clicked();
  void on_pbtablecoinmultiapplyselected_clicked();
  void on_pbtablecoinmultiapplyall_clicked();

  void on_pbtablecoinmulticopy_clicked();

  void on_chkboxcoinmultienabled0_clicked(bool checked);
  void on_chkboxcoinmultienabled1_clicked(bool checked);
  void on_chkboxcoinmultienabled2_clicked(bool checked);
  void on_chkboxcoinmultienabled3_clicked(bool checked);
  void on_chkboxcoinmultienabled4_clicked(bool checked);
  void on_chkboxcoinmultienabled5_clicked(bool checked);
  void on_chkboxcoinmultienabled6_clicked(bool checked);
  void on_chkboxcoinmultienabled7_clicked(bool checked);
  void on_chkboxcoinmultienabled8_clicked(bool checked);
  
  void TableCoinMultiCheckState(int row, int column);
  
  // TriggerSource
  void on_pbtabletriggersourceloadall_clicked();
  void on_pbtabletriggersourceapplyall_clicked();


};

#endif /* _LOGICSETTINGS_H_ */
// 
// LogicSettings.hh ends here
