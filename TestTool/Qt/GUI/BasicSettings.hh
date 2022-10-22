// BasicSettings.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 10月 21 20:26:00 2022 (+0800)
// Last-Updated: 六 10月 22 14:05:33 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _BASICSETTINGS_H_
#define _BASICSETTINGS_H_

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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#define TABNUM_BASICSETTINGS 7

class MainWindow;

class BasicSettings : public QMainWindow
{
  Q_OBJECT

public:
  BasicSettings(MainWindow *parent = nullptr);
  virtual ~BasicSettings();


private:

  MainWindow * const mMainWindow;


  QListWidget *listWidget;
  QTabWidget *tabWidget;

  QWidget *centralWidget;
  QSplitter *splitter;
  
  QToolBox *toolBox;
  QWidget *page;
  QGridLayout *gridLayout;
  QToolButton *tBtnListIni;
  QToolButton *tBtnListClear;
  QToolButton *tBtnListInsert;
  QToolButton *tBtnListAppend;
  QToolButton *tBtnListDelete;

  QWidget *page_2;
  QVBoxLayout *verticalLayout_4;
  QGroupBox *groupBox_4;
  QVBoxLayout *verticalLayout_3;
  QLabel *label_2;
  QLineEdit *lineEdit;
  QSpinBox *spinBox;
  QSpacerItem *verticalSpacer;

  QWidget *page_3;

  QWidget *tab_3;
  QVBoxLayout *verticalLayout;
  QGroupBox *groupBox;
  QHBoxLayout *horizontalLayout;
  QLabel *label;
  QLineEdit *editCutItemText;
  QCheckBox *chkBoxListEditable;
  
  QGroupBox *groupBox_2;
  QHBoxLayout *horizontalLayout_2;
  QToolButton *tBtnSelectItem;
  QToolButton *tBtnSelALL;
  QToolButton *tBtnSelNone;
  QToolButton *tBtnSelInvs;

  QWidget *tab;
  QVBoxLayout *verticalLayout_2;
  QGroupBox *groupBox_3;
  QHBoxLayout *horizontalLayout_3;
  
  QWidget *tab_2;
  
  // toolBox
  //         page
  //               gridLayout
  //                       tBtnListIni
  //                       tBtnListClear
  //                       tBtnListInsert
  //                       tBtnListAppend
  //                       tBtnListDelete
  //         page_2
  //               verticalLayout_4
  //
			     
		     
private slots:
  
};


#endif /* _BASICSETTINGS_H_ */

// 
// BasicSettings.hh ends here






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


