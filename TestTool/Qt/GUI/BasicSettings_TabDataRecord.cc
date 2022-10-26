// BasicSettings_TabDataRecord.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 24 21:07:45 2022 (+0800)
// Last-Updated: 二 10月 25 09:45:31 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void BasicSettings::TabDataRecord(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Record", nullptr));

  tabledatarecord = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tabledatarecord);
  // verticalLayout[n]->setAlignment(Qt::AlignCenter);
  tabledatarecord->setRowCount(MAXCHANNELNUM);
  tabledatarecord->setColumnCount(5);
  tabledatarecord->setSelectionBehavior(QAbstractItemView::SelectItems);

  QStringList headercol;
  headercol << "Good channel" << "TC" << "EQS" << "ERB" << "ETS";
  tabledatarecord->setHorizontalHeaderLabels(headercol);

  QStringList headerrow;
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    headerrow << QString::number(i);
  tabledatarecord->setVerticalHeaderLabels(headerrow);

  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 5; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);
	item->setCheckState(Qt::Checked);
	item->setText(QString::fromUtf8("Record"));
        tabledatarecord->setItem(i, j, item);
	
      }

  connect(tabledatarecord, SIGNAL(cellChanged(int, int)), this, SLOT(TableDataRecordCheckState(int, int)));
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[0])
	tabledatarecord->setRowHidden(i, false);
      else
	tabledatarecord->setRowHidden(i, true);
    }

  // {
  //   QWidget *widget = (QWidget *)tabledatarecord->cellWidget(0, 0);
  //   QCheckBox *ckb = (QCheckBox *)widget->children().at(1);
  //   ckb->setChecked(true);
  // }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxrecord0 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxrecord0);
  groupboxrecord0->setObjectName(QString::fromUtf8("groupboxrecord0"));

  horizontalgbrecord0 = new QHBoxLayout(groupboxrecord0);
  horizontalgbrecord0->setSpacing(10);                                       
  horizontalgbrecord0->setContentsMargins(11, 11, 11, 11);                   
  horizontalgbrecord0->setObjectName(QString::fromUtf8("horizontalgbrecord0")); 
  horizontalgbrecord0->setContentsMargins(-1, 2, -1, 2);                     

  QLabel *label0 = new QLabel(groupboxrecord0);
  horizontalgbrecord0->addWidget(label0);
  label0->setObjectName(QString::fromUtf8("label0"));
  label0->setText(QApplication::translate("BasicSettings", "Enabled", nullptr));

  for (int i = 0; i < 5; ++i)
    {
      chkboxrecordenabled[i] = new QCheckBox(groupboxrecord0);
      horizontalgbrecord0->addWidget(chkboxrecordenabled[i]);
      chkboxrecordenabled[i]->setObjectName(QString::fromUtf8("chkboxrecordenabled")+QString::number(i));
      chkboxrecordenabled[i]->setChecked(true);

      if(i == 0)
	chkboxrecordenabled[i]->setText(QString::fromUtf8("GC"));
      else if(i == 1)
	chkboxrecordenabled[i]->setText(QString::fromUtf8("TC"));
      else if(i == 2)
	chkboxrecordenabled[i]->setText(QString::fromUtf8("EQS"));
      else if(i == 3)
	chkboxrecordenabled[i]->setText(QString::fromUtf8("ERB"));
      else if(i == 4)
	chkboxrecordenabled[i]->setText(QString::fromUtf8("ETS"));
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxrecord1 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxrecord1);
  groupboxrecord1->setObjectName(QString::fromUtf8("groupboxrecord1"));

  horizontalgbrecord1 = new QHBoxLayout(groupboxrecord1);
  horizontalgbrecord1->setSpacing(10);
  horizontalgbrecord1->setContentsMargins(11, 11, 11, 11);
  horizontalgbrecord1->setObjectName(QString::fromUtf8("horizontalgbrecord1"));
  horizontalgbrecord1->setContentsMargins(-1, 2, -1, 2);

  QLabel *label1 = new QLabel(groupboxrecord1);
  horizontalgbrecord1->addWidget(label1);
  label1->setObjectName(QString::fromUtf8("label1"));
  label1->setText(QApplication::translate("BasicSettings", "Select", nullptr));

  rbtablerecordrow = new QRadioButton(groupboxrecord1);
  horizontalgbrecord1->addWidget(rbtablerecordrow);
  rbtablerecordrow->setText(QApplication::translate("BasicSettings", "Row", nullptr));
  rbtablerecordrow->setObjectName(QString::fromUtf8("rbtablerecordrow"));
  rbtablerecordrow->setChecked(false);
  rbtablerecordcol = new QRadioButton(groupboxrecord1);
  horizontalgbrecord1->addWidget(rbtablerecordcol);
  rbtablerecordcol->setText(QApplication::translate("BasicSettings", "Column", nullptr));
  rbtablerecordcol->setObjectName(QString::fromUtf8("rbtablerecordcol"));
  rbtablerecordcol->setChecked(false);
  rbtablerecorditem = new QRadioButton(groupboxrecord1);
  horizontalgbrecord1->addWidget(rbtablerecorditem);
  rbtablerecorditem->setText(QApplication::translate("BasicSettings", "Item", nullptr));
  rbtablerecorditem->setObjectName(QString::fromUtf8("rbtablerecorditem"));
  rbtablerecorditem->setChecked(true);


  QLabel *label30 = new QLabel(groupboxrecord1);
  horizontalgbrecord1->addWidget(label30);
  label30->setObjectName(QString::fromUtf8("label30"));
  label30->setText(QApplication::translate("BasicSettings", "Selected copy from ", nullptr));

  cbrecordcopych = new QComboBox(groupboxrecord1);
  horizontalgbrecord1->addWidget(cbrecordcopych);
  cbrecordcopych->setObjectName(QString::fromUtf8("cbrecordcopych"));
  cbrecordcopych->setEditable(false);
  for (int i = 0; i < TESTCH[0]; ++i)
    {
      cbrecordcopych->addItem(QString::asprintf("ch - %02d", i));
    }

  pbtablerecordcopy = new QPushButton(groupboxrecord1);
  horizontalgbrecord1->addWidget(pbtablerecordcopy);
  pbtablerecordcopy->setObjectName(QString::fromUtf8("pbtablerecordcopy"));
  pbtablerecordcopy->setEnabled(true);
  pbtablerecordcopy->setText(QApplication::translate("BasicSettings", "Copy", nullptr));
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxrecord3 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxrecord3);
  groupboxrecord3->setObjectName(QString::fromUtf8("groupboxrecord3"));

  horizontalgbrecord3 = new QHBoxLayout(groupboxrecord3);
  horizontalgbrecord3->setSpacing(10);
  horizontalgbrecord3->setContentsMargins(11, 11, 11, 11);
  horizontalgbrecord3->setObjectName(QString::fromUtf8("horizontalgbrecord3"));
  horizontalgbrecord3->setContentsMargins(-1, 2, -1, 2);  
    


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  groupboxrecord2 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxrecord2);
  groupboxrecord2->setObjectName(QString::fromUtf8("groupboxrecord2"));

  horizontalgbrecord2 = new QHBoxLayout(groupboxrecord2);
  horizontalgbrecord2->setSpacing(10);
  horizontalgbrecord2->setContentsMargins(11, 11, 11, 11);
  horizontalgbrecord2->setObjectName(QString::fromUtf8("horizontalgbrecord2"));
  horizontalgbrecord2->setContentsMargins(-1, 2, -1, 2);

  
  pbtablerecordloadselected = new QPushButton(groupboxrecord2);
  horizontalgbrecord2->addWidget(pbtablerecordloadselected);
  pbtablerecordloadselected->setObjectName(QString::fromUtf8("pbtablerecordloadselected"));
  pbtablerecordloadselected->setEnabled(true);
  pbtablerecordloadselected->setText(QApplication::translate("BasicSettings", "Load Selected", nullptr));

  pbtablerecordloadall = new QPushButton(groupboxrecord2);
  horizontalgbrecord2->addWidget(pbtablerecordloadall);
  pbtablerecordloadall->setObjectName(QString::fromUtf8("pbtablerecordloadall"));
  pbtablerecordloadall->setEnabled(true);
  pbtablerecordloadall->setText(QApplication::translate("BasicSettings", "Load All", nullptr));
  
  pbtablerecordapplyselected = new QPushButton(groupboxrecord2);
  horizontalgbrecord2->addWidget(pbtablerecordapplyselected);
  pbtablerecordapplyselected->setObjectName(QString::fromUtf8("pbtablerecordapplyselected"));
  pbtablerecordapplyselected->setEnabled(true);
  pbtablerecordapplyselected->setText(QApplication::translate("BasicSettings", "Apply Selected", nullptr));
  
  pbtablerecordapplyall = new QPushButton(groupboxrecord2);
  horizontalgbrecord2->addWidget(pbtablerecordapplyall);
  pbtablerecordapplyall->setObjectName(QString::fromUtf8("pbtablerecordapplyall"));
  pbtablerecordapplyall->setEnabled(true);
  pbtablerecordapplyall->setText(QApplication::translate("BasicSettings", "Apply All", nullptr));
  
}

void BasicSettings::on_chkboxrecordenabled0_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabledatarecord->item(i, 0);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxrecordenabled1_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabledatarecord->item(i, 1);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxrecordenabled2_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabledatarecord->item(i, 2);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxrecordenabled3_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabledatarecord->item(i, 3);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxrecordenabled4_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabledatarecord->item(i, 4);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::TableDataRecordCheckState(int row, int column)
{
  if(tabledatarecord->item(row, column)->checkState() == Qt::Checked)
    {
      tabledatarecord->item(row, column)->setText(QString::fromUtf8("Record"));
    }
  else
    {
      tabledatarecord->item(row, column)->setText(QString::fromUtf8("Unrecord"));
    }
}


void BasicSettings::on_rbtablerecordrow_clicked()
{
  tabledatarecord->setSelectionBehavior(QAbstractItemView::SelectRows);
  std::cout << tabledatarecord->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablerecorditem_clicked()
{
  tabledatarecord->setSelectionBehavior(QAbstractItemView::SelectItems);
  std::cout << tabledatarecord->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablerecordcol_clicked()
{
  tabledatarecord->setSelectionBehavior(QAbstractItemView::SelectColumns);
  std::cout << tabledatarecord->selectionBehavior() << std::endl;
}

void BasicSettings::on_pbtablerecordloadselected_clicked()
{
  std::cout << "load selected" << std::endl;

  std::cout << tabledatarecord->selectionBehavior() << std::endl;

  QList<QTableWidgetItem*> items = tabledatarecord->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tabledatarecord->row(items.at(i));//获取选中的行
	  int column = tabledatarecord->column(items.at(i));
	  
	  QTableWidgetItem *item = items.at(i);
	  QString name = item->text();//获取内容
	  std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }
}

void BasicSettings::on_pbtablerecordloadall_clicked()
{
  std::cout << "load all" << std::endl;
  
}

void BasicSettings::on_pbtablerecordapplyselected_clicked()
{
  std::cout << "apply select" << std::endl;
 
}

void BasicSettings::on_pbtablerecordapplyall_clicked()
{
  std::cout << "apply all" << std::endl;
  
}

void BasicSettings::on_pbtablerecordcopy_clicked()
{
  QList<QTableWidgetItem*> items = tabledatarecord->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      // std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tabledatarecord->row(items.at(i));//获取选中的行
	  int column = tabledatarecord->column(items.at(i));
	  
	  // QTableWidgetItem *item = items.at(i);
	  // QString name = item->text();//获取内容
	  // std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }

  std::cout << "CH  " << cbrecordcopych->currentIndex() << std::endl;
}



// 
// BasicSettings_TabDataRecord.cc ends here
