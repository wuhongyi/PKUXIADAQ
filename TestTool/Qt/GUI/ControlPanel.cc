// ControlPanel.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 4月 23 21:07:44 2022 (+0800)
// Last-Updated: 二 10月 25 20:35:22 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 13
// URL: http://wuhongyi.cn 

#include "ControlPanel.hh"
#include "MainWindow.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ControlPanel::ControlPanel(MainWindow *parent)
	: QWidget(parent),
	  mMainWindow(parent)
{
  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("ControlPanel"));
  this->resize(200, 99);




  
  pbStartAcq = new QPushButton(this);
  pbStartAcq->setObjectName(QString::fromUtf8("pbStartAcq"));
  pbStartAcq->setMinimumSize(QSize(60, 22));
  pbStartAcq->setMaximumSize(QSize(100, 16777215));

  pbStopAcq = new QPushButton(this);
  pbStopAcq->setObjectName(QString::fromUtf8("pbStopAcq"));
  pbStopAcq->setMinimumSize(QSize(60, 22));
  pbStopAcq->setMaximumSize(QSize(100, 16777215));

  
  verticallayout = new QVBoxLayout();
  verticallayout->setObjectName(QString::fromUtf8("verticallayout"));
  verticallayout->addWidget(pbStartAcq);
  verticallayout->addWidget(pbStopAcq);


  gridlayout = new QGridLayout(this);
  gridlayout->setObjectName(QString::fromUtf8("gridlayout"));
  gridlayout->setContentsMargins(-1, 0, -1, 0);
  gridlayout->addLayout(verticallayout, 0, 0, 1, 1);
  
  this->setWindowTitle(QApplication::translate("ControlPanel", "Form", nullptr));
  pbStartAcq->setText(QApplication::translate("ControlPanel", "Start", nullptr));
  pbStopAcq->setText(QApplication::translate("ControlPanel", "Stop", nullptr));
  
  trigger_timer = new QTimer(this);
  // QObject::connect(trigger_timer, SIGNAL(timeout()), mMainWindow, SLOT(SWTrg()));

  connect(pbStartAcq, SIGNAL(clicked()), mMainWindow, SLOT(StartAcquisition()));
  connect(pbStopAcq, SIGNAL(clicked()), mMainWindow, SLOT(StopAcquisition()));

  QMetaObject::connectSlotsByName(this);
}

ControlPanel::~ControlPanel()
{

}

void ControlPanel::DisableAll(bool f)
{
  pbStartAcq->setEnabled(!f);
  pbStopAcq->setEnabled(!f);
	// ui->pB_Trigger->setEnabled(!f);
}


void ControlPanel::ToogleStartStop()
{
  if(pbStartAcq->isEnabled())
    {
      pbStartAcq->setEnabled(false);
      pbStopAcq->setEnabled(true);
    }
  else
    {
      pbStartAcq->setEnabled(true);
      pbStopAcq->setEnabled(false);
    }
  
	// if (ui->pbStartAcq->isEnabled()) { //start acq 
	// 	ui->pbStartAcq->setEnabled(false);
	// 	ui->pbStopAcq->setEnabled(true);
	// 	if (ui->cBox_trigger_mode->currentIndex() == TRG_MODE_NORMAL)
	// 		ui->pB_Trigger->setEnabled(true);			
		
	// 	if(!mWaveDump->NAutoTrg)
	// 		trigger_timer->stop();
	// 	else {
	// 		if (!trigger_timer->isActive())
	// 			trigger_timer->start(500);
	// 	}
	// 	ui->dBox_reclen->setEnabled(false);
	// 	ui->dBox_pretrg->setEnabled(false);
	// 	ui->pB_trg_mask->setEnabled(false);
	// }
	// else { //stop acq 
	// 	ui->pbStartAcq->setEnabled(true);
	// 	ui->pbStopAcq->setEnabled(false);
	// 	ui->pB_Trigger->setEnabled(false);
	// 	ui->dBox_reclen->setEnabled(true);
	// 	ui->dBox_pretrg->setEnabled(true);
	// 	ui->pB_trg_mask->setEnabled(true);
	// 	if (trigger_timer->isActive() && !mWaveDump->NDevStarted)
	// 		trigger_timer->stop();
	// }

}

void ControlPanel::SetStartStopStatus(int status)
{
	// if ((status == 1 && ui->pbStartAcq->isEnabled()) || (status == 0 && ui->pbStopAcq->isEnabled())) { 
	// 	toogleStartStop();
	// }
}



void ControlPanel::UpdateStartableDevices()
{
  pbStartAcq->setEnabled(true);
  
	// int idx;
	// ui->cbDev->blockSignals(true);
	// switch (cmd) {
	// case 0: // remove
	// 	idx = this->ui->cbDev->findText(name);
	// 	if (idx != -1) {
	// 		this->ui->cbDev->removeItem(idx);
	// 	}
	// 	if ((this->ui->cbDev->count() == 1) && (this->ui->cbDev->currentText() == "ALL")) {
	// 		this->ui->cbDev->clear();
	// 		this->ui->cbDev->setEnabled(false);
	// 		this->ui->pbStartAcq->setEnabled(false);
	// 	}
	// 	break;
	// case 1: // add
	// 	this->ui->cbDev->addItem(name);
	// 	if ((this->ui->cbDev->count() > 1) && (this->ui->cbDev->findText("ALL") == -1)) {
	// 		this->ui->cbDev->addItem("ALL");
	// 		this->ui->cbDev->model()->sort(0);
	// 	}
	// 	this->ui->cbDev->setEnabled(true);
	// 	this->ui->pbStartAcq->setEnabled(true);
	// 	break;
	// }
	// ui->cbDev->blockSignals(false);

}


    

//     QComboBox *cbDev;
//     QGroupBox *groupBox_3;
//     QGridLayout *gridLayout_3;
//     QHBoxLayout *horizontalLayout_10;
//     QLabel *label_6;
//     QComboBox *cBox_trigger_mode;
//     QHBoxLayout *horizontalLayout_11;
//     QLabel *label_7;
//     QComboBox *cBox_trigger_source;
//     QPushButton *pB_Trigger;
//     QPushButton *pB_trg_mask;
//     QGroupBox *groupBox;
//     QGridLayout *gridLayout_2;
//     QHBoxLayout *horizontalLayout_7;
//     QLabel *label_reclen;
//     QDoubleSpinBox *dBox_reclen;
//     QHBoxLayout *horizontalLayout_12;
//     QLabel *label_pretrg;
//     QDoubleSpinBox *dBox_pretrg;
//     QHBoxLayout *horizontalLayout_8;
//     QLabel *label_4;
//     QComboBox *cBox_x_unit;
//     QHBoxLayout *horizontalLayout_9;
//     QLabel *label_5;
//     QComboBox *cBox_y_unit;
//     QGroupBox *gBox_cursors;
//     QGridLayout *gridLayout;
//     QHBoxLayout *horizontalLayout_6;
//     QLabel *label_13;
//     QLineEdit *lineEdit_Dy;
//     QHBoxLayout *horizontalLayout_5;
//     QLabel *label_3;
//     QLineEdit *lineEdit_Dx;
//     QHBoxLayout *horizontalLayout_2;
//     QLabel *label_32;
//     QLineEdit *lineEdit_x2;
//     QLabel *label_33;
//     QLineEdit *lineEdit_y2;
//     QHBoxLayout *horizontalLayout;
//     QLabel *label_30;
//     QLineEdit *lineEdit_x1;
//     QLabel *label_31;
//     QLineEdit *lineEdit_y1;
//     QHBoxLayout *horizontalLayout_3;
//     QLabel *label;
//     QComboBox *cBox_C_trace;

// void setupUi(QWidget *controlPanel)
// {



  



  


  

//   cbDev = new QComboBox(controlPanel);
//   cbDev->setObjectName(QString::fromUtf8("cbDev"));
//   cbDev->setMinimumSize(QSize(200, 22));
//   cbDev->setMaximumSize(QSize(150, 16777215));

//   gridLayout_4->addWidget(cbDev, 0, 1, 1, 1);

//   groupBox_3 = new QGroupBox(controlPanel);
//   groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
//   gridLayout_3 = new QGridLayout(groupBox_3);
//   gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
//   gridLayout_3->setVerticalSpacing(3);
//   gridLayout_3->setContentsMargins(-1, 2, -1, 2);
//   horizontalLayout_10 = new QHBoxLayout();
//   horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
//   label_6 = new QLabel(groupBox_3);
//   label_6->setObjectName(QString::fromUtf8("label_6"));

//   horizontalLayout_10->addWidget(label_6);

//   cBox_trigger_mode = new QComboBox(groupBox_3);
//   cBox_trigger_mode->addItem(QString());
//   cBox_trigger_mode->addItem(QString());
//   cBox_trigger_mode->setObjectName(QString::fromUtf8("cBox_trigger_mode"));
//   cBox_trigger_mode->setMinimumSize(QSize(0, 20));

//   horizontalLayout_10->addWidget(cBox_trigger_mode);


//   gridLayout_3->addLayout(horizontalLayout_10, 0, 0, 1, 1);

//   horizontalLayout_11 = new QHBoxLayout();
//   horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
//   label_7 = new QLabel(groupBox_3);
//   label_7->setObjectName(QString::fromUtf8("label_7"));

//   horizontalLayout_11->addWidget(label_7);

//   cBox_trigger_source = new QComboBox(groupBox_3);
//   cBox_trigger_source->addItem(QString());
//   cBox_trigger_source->addItem(QString());
//   cBox_trigger_source->addItem(QString());
//   cBox_trigger_source->addItem(QString());
//   cBox_trigger_source->addItem(QString());
//   cBox_trigger_source->setObjectName(QString::fromUtf8("cBox_trigger_source"));
//   cBox_trigger_source->setMinimumSize(QSize(0, 20));

//   horizontalLayout_11->addWidget(cBox_trigger_source);


//   gridLayout_3->addLayout(horizontalLayout_11, 1, 0, 1, 1);

//   pB_Trigger = new QPushButton(groupBox_3);
//   pB_Trigger->setObjectName(QString::fromUtf8("pB_Trigger"));
//   pB_Trigger->setEnabled(false);
//   pB_Trigger->setMinimumSize(QSize(0, 20));

//   gridLayout_3->addWidget(pB_Trigger, 0, 1, 1, 1);

//   pB_trg_mask = new QPushButton(groupBox_3);
//   pB_trg_mask->setObjectName(QString::fromUtf8("pB_trg_mask"));

//   gridLayout_3->addWidget(pB_trg_mask, 1, 1, 1, 1);


//   gridLayout_4->addWidget(groupBox_3, 0, 2, 1, 1);

//   groupBox = new QGroupBox(controlPanel);
//   groupBox->setObjectName(QString::fromUtf8("groupBox"));
//   groupBox->setMinimumSize(QSize(0, 70));
//   gridLayout_2 = new QGridLayout(groupBox);
//   gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
//   gridLayout_2->setVerticalSpacing(3);
//   gridLayout_2->setContentsMargins(-1, 2, -1, 2);
//   horizontalLayout_7 = new QHBoxLayout();
//   horizontalLayout_7->setSpacing(2);
//   horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
//   label_reclen = new QLabel(groupBox);
//   label_reclen->setObjectName(QString::fromUtf8("label_reclen"));

//   horizontalLayout_7->addWidget(label_reclen);

//   dBox_reclen = new QDoubleSpinBox(groupBox);
//   dBox_reclen->setObjectName(QString::fromUtf8("dBox_reclen"));
//   dBox_reclen->setMinimumSize(QSize(0, 20));
//   dBox_reclen->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
//   dBox_reclen->setKeyboardTracking(false);

//   horizontalLayout_7->addWidget(dBox_reclen);


//   gridLayout_2->addLayout(horizontalLayout_7, 0, 0, 1, 2);

//   horizontalLayout_12 = new QHBoxLayout();
//   horizontalLayout_12->setSpacing(2);
//   horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
//   label_pretrg = new QLabel(groupBox);
//   label_pretrg->setObjectName(QString::fromUtf8("label_pretrg"));

//   horizontalLayout_12->addWidget(label_pretrg);

//   dBox_pretrg = new QDoubleSpinBox(groupBox);
//   dBox_pretrg->setObjectName(QString::fromUtf8("dBox_pretrg"));
//   dBox_pretrg->setMinimumSize(QSize(0, 20));
//   dBox_pretrg->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
//   dBox_pretrg->setKeyboardTracking(false);

//   horizontalLayout_12->addWidget(dBox_pretrg);


//   gridLayout_2->addLayout(horizontalLayout_12, 0, 2, 1, 1);

//   horizontalLayout_8 = new QHBoxLayout();
//   horizontalLayout_8->setSpacing(2);
//   horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
//   label_4 = new QLabel(groupBox);
//   label_4->setObjectName(QString::fromUtf8("label_4"));

//   horizontalLayout_8->addWidget(label_4, 0, Qt::AlignRight);

//   cBox_x_unit = new QComboBox(groupBox);
//   cBox_x_unit->addItem(QString());
//   cBox_x_unit->addItem(QString());
//   cBox_x_unit->setObjectName(QString::fromUtf8("cBox_x_unit"));
//   cBox_x_unit->setMinimumSize(QSize(0, 20));
//   cBox_x_unit->setCurrentText(QString::fromUtf8("time"));
//   cBox_x_unit->setSizeAdjustPolicy(QComboBox::AdjustToContents);

//   horizontalLayout_8->addWidget(cBox_x_unit);


//   gridLayout_2->addLayout(horizontalLayout_8, 1, 0, 1, 1);

//   horizontalLayout_9 = new QHBoxLayout();
//   horizontalLayout_9->setSpacing(2);
//   horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
//   label_5 = new QLabel(groupBox);
//   label_5->setObjectName(QString::fromUtf8("label_5"));

//   horizontalLayout_9->addWidget(label_5, 0, Qt::AlignRight);

//   cBox_y_unit = new QComboBox(groupBox);
//   cBox_y_unit->addItem(QString());
//   cBox_y_unit->addItem(QString());
//   cBox_y_unit->setObjectName(QString::fromUtf8("cBox_y_unit"));
//   cBox_y_unit->setMinimumSize(QSize(0, 20));
//   cBox_y_unit->setSizeAdjustPolicy(QComboBox::AdjustToContents);

//   horizontalLayout_9->addWidget(cBox_y_unit);


//   gridLayout_2->addLayout(horizontalLayout_9, 1, 1, 1, 2);


//   gridLayout_4->addWidget(groupBox, 0, 3, 1, 1);

//   gBox_cursors = new QGroupBox(controlPanel);
//   gBox_cursors->setObjectName(QString::fromUtf8("gBox_cursors"));
//   gBox_cursors->setMinimumSize(QSize(400, 70));
//   gBox_cursors->setCheckable(true);
//   gBox_cursors->setChecked(false);
//   gridLayout = new QGridLayout(gBox_cursors);
//   gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
//   gridLayout->setContentsMargins(-1, 3, -1, 3);
//   horizontalLayout_6 = new QHBoxLayout();
//   horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
//   label_13 = new QLabel(gBox_cursors);
//   label_13->setObjectName(QString::fromUtf8("label_13"));
//   QFont font;
//   font.setPointSize(8);
//   font.setBold(false);
//   font.setWeight(50);
//   label_13->setFont(font);

//   horizontalLayout_6->addWidget(label_13);

//   lineEdit_Dy = new QLineEdit(gBox_cursors);
//   lineEdit_Dy->setObjectName(QString::fromUtf8("lineEdit_Dy"));
//   lineEdit_Dy->setEnabled(false);
//   QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//   sizePolicy.setHorizontalStretch(0);
//   sizePolicy.setVerticalStretch(0);
//   sizePolicy.setHeightForWidth(lineEdit_Dy->sizePolicy().hasHeightForWidth());
//   lineEdit_Dy->setSizePolicy(sizePolicy);
//   lineEdit_Dy->setMinimumSize(QSize(60, 20));
//   lineEdit_Dy->setFont(font);

//   horizontalLayout_6->addWidget(lineEdit_Dy);


//   gridLayout->addLayout(horizontalLayout_6, 1, 2, 1, 1);

//   horizontalLayout_5 = new QHBoxLayout();
//   horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
//   label_3 = new QLabel(gBox_cursors);
//   label_3->setObjectName(QString::fromUtf8("label_3"));
//   label_3->setFont(font);

//   horizontalLayout_5->addWidget(label_3);

//   lineEdit_Dx = new QLineEdit(gBox_cursors);
//   lineEdit_Dx->setObjectName(QString::fromUtf8("lineEdit_Dx"));
//   lineEdit_Dx->setEnabled(false);
//   sizePolicy.setHeightForWidth(lineEdit_Dx->sizePolicy().hasHeightForWidth());
//   lineEdit_Dx->setSizePolicy(sizePolicy);
//   lineEdit_Dx->setMinimumSize(QSize(60, 20));
//   lineEdit_Dx->setFont(font);

//   horizontalLayout_5->addWidget(lineEdit_Dx);


//   gridLayout->addLayout(horizontalLayout_5, 0, 2, 1, 1);

//   horizontalLayout_2 = new QHBoxLayout();
//   horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
//   label_32 = new QLabel(gBox_cursors);
//   label_32->setObjectName(QString::fromUtf8("label_32"));
//   label_32->setFont(font);

//   horizontalLayout_2->addWidget(label_32);

//   lineEdit_x2 = new QLineEdit(gBox_cursors);
//   lineEdit_x2->setObjectName(QString::fromUtf8("lineEdit_x2"));
//   lineEdit_x2->setEnabled(false);
//   sizePolicy.setHeightForWidth(lineEdit_x2->sizePolicy().hasHeightForWidth());
//   lineEdit_x2->setSizePolicy(sizePolicy);
//   lineEdit_x2->setMinimumSize(QSize(60, 20));
//   lineEdit_x2->setFont(font);

//   horizontalLayout_2->addWidget(lineEdit_x2);

//   label_33 = new QLabel(gBox_cursors);
//   label_33->setObjectName(QString::fromUtf8("label_33"));
//   label_33->setFont(font);

//   horizontalLayout_2->addWidget(label_33);

//   lineEdit_y2 = new QLineEdit(gBox_cursors);
//   lineEdit_y2->setObjectName(QString::fromUtf8("lineEdit_y2"));
//   lineEdit_y2->setEnabled(false);
//   sizePolicy.setHeightForWidth(lineEdit_y2->sizePolicy().hasHeightForWidth());
//   lineEdit_y2->setSizePolicy(sizePolicy);
//   lineEdit_y2->setMinimumSize(QSize(60, 20));
//   lineEdit_y2->setFont(font);

//   horizontalLayout_2->addWidget(lineEdit_y2);


//   gridLayout->addLayout(horizontalLayout_2, 1, 1, 1, 1);

//   horizontalLayout = new QHBoxLayout();
//   horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
//   label_30 = new QLabel(gBox_cursors);
//   label_30->setObjectName(QString::fromUtf8("label_30"));
//   label_30->setFont(font);

//   horizontalLayout->addWidget(label_30);

//   lineEdit_x1 = new QLineEdit(gBox_cursors);
//   lineEdit_x1->setObjectName(QString::fromUtf8("lineEdit_x1"));
//   lineEdit_x1->setEnabled(false);
//   sizePolicy.setHeightForWidth(lineEdit_x1->sizePolicy().hasHeightForWidth());
//   lineEdit_x1->setSizePolicy(sizePolicy);
//   lineEdit_x1->setMinimumSize(QSize(60, 20));
//   lineEdit_x1->setFont(font);

//   horizontalLayout->addWidget(lineEdit_x1);

//   label_31 = new QLabel(gBox_cursors);
//   label_31->setObjectName(QString::fromUtf8("label_31"));
//   label_31->setFont(font);

//   horizontalLayout->addWidget(label_31);

//   lineEdit_y1 = new QLineEdit(gBox_cursors);
//   lineEdit_y1->setObjectName(QString::fromUtf8("lineEdit_y1"));
//   lineEdit_y1->setEnabled(false);
//   sizePolicy.setHeightForWidth(lineEdit_y1->sizePolicy().hasHeightForWidth());
//   lineEdit_y1->setSizePolicy(sizePolicy);
//   lineEdit_y1->setMinimumSize(QSize(60, 20));
//   lineEdit_y1->setFont(font);

//   horizontalLayout->addWidget(lineEdit_y1);


//   gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

//   horizontalLayout_3 = new QHBoxLayout();
//   horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
//   label = new QLabel(gBox_cursors);
//   label->setObjectName(QString::fromUtf8("label"));

//   horizontalLayout_3->addWidget(label);

//   cBox_C_trace = new QComboBox(gBox_cursors);
//   cBox_C_trace->setObjectName(QString::fromUtf8("cBox_C_trace"));
//   cBox_C_trace->setMinimumSize(QSize(0, 20));

//   horizontalLayout_3->addWidget(cBox_C_trace);


//   gridLayout->addLayout(horizontalLayout_3, 0, 0, 2, 1);


//   gridLayout_4->addWidget(gBox_cursors, 0, 4, 1, 1);


//   retranslateUi(controlPanel);

//   QMetaObject::connectSlotsByName(controlPanel);
// } // setupUi

// void retranslateUi(QWidget *controlPanel)
// {

//   groupBox_3->setTitle(QString());
//   label_6->setText(QApplication::translate("controlPanel", "Trigger Mode", nullptr));
//   cBox_trigger_mode->setItemText(0, QApplication::translate("controlPanel", "Auto", nullptr));
//   cBox_trigger_mode->setItemText(1, QApplication::translate("controlPanel", "Normal", nullptr));

//   label_7->setText(QApplication::translate("controlPanel", "Trigger Source", nullptr));
//   cBox_trigger_source->setItemText(0, QApplication::translate("controlPanel", "Software", nullptr));
//   cBox_trigger_source->setItemText(1, QApplication::translate("controlPanel", "Self", nullptr));
//   cBox_trigger_source->setItemText(2, QApplication::translate("controlPanel", "Ext-TrgIn", nullptr));
//   cBox_trigger_source->setItemText(3, QApplication::translate("controlPanel", "Ext-GPIO", nullptr));
//   cBox_trigger_source->setItemText(4, QApplication::translate("controlPanel", "Internal", nullptr));

//   pB_Trigger->setText(QApplication::translate("controlPanel", "Force Trigger", nullptr));
//   pB_trg_mask->setText(QApplication::translate("controlPanel", "Set Self Trigger Mask", nullptr));
//   groupBox->setTitle(QString());
//   label_reclen->setText(QApplication::translate("controlPanel", "Record Length", nullptr));
//   label_pretrg->setText(QApplication::translate("controlPanel", "Pre Trigger", nullptr));
//   label_4->setText(QApplication::translate("controlPanel", "X unit", nullptr));
//   cBox_x_unit->setItemText(0, QApplication::translate("controlPanel", "time", nullptr));
//   cBox_x_unit->setItemText(1, QApplication::translate("controlPanel", "samples", nullptr));

//   label_5->setText(QApplication::translate("controlPanel", "Y unit", nullptr));
//   cBox_y_unit->setItemText(0, QApplication::translate("controlPanel", "mV", nullptr));
//   cBox_y_unit->setItemText(1, QApplication::translate("controlPanel", "LSB", nullptr));

//   gBox_cursors->setTitle(QApplication::translate("controlPanel", "Cursors", nullptr));
//   label_13->setText(QApplication::translate("controlPanel", "DY", nullptr));
//   label_3->setText(QApplication::translate("controlPanel", "DX", nullptr));
//   label_32->setText(QApplication::translate("controlPanel", "X2", nullptr));
//   label_33->setText(QApplication::translate("controlPanel", "Y2", nullptr));
//   label_30->setText(QApplication::translate("controlPanel", "X1", nullptr));
//   label_31->setText(QApplication::translate("controlPanel", "Y1", nullptr));
//   label->setText(QApplication::translate("controlPanel", "Trace", nullptr));
// } // retranslateUi



// 
// ControlPanel.cc ends here
