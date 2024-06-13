// Debug.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 2月 16 15:49:18 2023 (+0800)
// Last-Updated: 一 3月 20 21:40:23 2023 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
// URL: http://wuhongyi.cn 

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "Global.hh"
#include "DeviceHandle.hh"

#include <vector>

#include <QTime>
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

#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#define TABNUM_DEBUG 3  // 0 single, 1 ch0-15, 2 ch16-31, 3 ch32-47, 4 ch48-63

class MainWindow;

class Debug : public QMainWindow
{
  Q_OBJECT
  
public:
  Debug(MainWindow *parent, DeviceHandle *device);
  virtual ~Debug();

private:

  void TabChSingle(int n);
  void TabCh00to15(int n);
  void TabCh16to31(int n);

  void LoadLogicDebugSources();
  void ApplyLogicDebugSources();
  void UpdateTab(int index1, int index2);
  
private:

  MainWindow * const mMainWindow;
  DeviceHandle *mDevice = nullptr;

  QWidget *centralWidget;
  QSplitter *splitter;// 左右区域分割

  QSplitter *leftsplitter;
  
  QToolBox *toolBox;// 内部装 QWidget page
  QGroupBox *choosebutton;
  QGridLayout *gridgroupbox;
  QComboBox *combomonitorfront;//EnableDisableOfTestSignals 0 [15]
  QComboBox *combomonitorbackpanle;//DebugSignalsOfBackplane 3 [0]
  QComboBox *combomonitorgroup;//GroupOfTestSignals 0 [14-12]
  QComboBox *combomonitorch;//ChannelOfTestSignals 0 [19-16]
  QComboBox *combomonitortest;//TestSignals 0 [23-20]
  QPushButton *btnmonitorapply;

  QGroupBox *choosebutton1;
  QGridLayout *gridgroupbox1;
  QComboBox *combotracebaseline;
  QComboBox *combotracebaselinech;
  
  QTabWidget *tabWidget; // 内部装 QWidget tab
  
  QStatusBar *statusbar;
  QLabel *statusinfo;

  // QToolBox
  QWidget *pagemod[MAXMODULENUM];
  QGridLayout *gridpagemod[MAXMODULENUM];//

  QWidget *tab[TABNUM_DEBUG];
  //QVBoxLayout *verticalLayout[TABNUM_DEBUG];
  QGridLayout *verticalLayout[TABNUM_DEBUG];


  
  QChartView *chartViewSingle;
  QChart *chartSingle;
  QChartView *chartView[MAXCHANNELNUM];
  QChart *chart[MAXCHANNELNUM];

				      
private slots:

  void on_toolBox_currentChanged(int index);

  void on_btnmonitorapply_clicked();

  void on_combotracebaseline_activated(int index);
  void on_combotracebaselinech_activated(int index);
  
};

#endif /* _DEBUG_H_ */
// 
// Debug.hh ends here
