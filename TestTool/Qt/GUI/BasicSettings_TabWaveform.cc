// BasicSettings_TabWaveform.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 24 21:16:06 2022 (+0800)
// Last-Updated: 二 10月 25 12:26:57 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BasicSettings::TabWaveform(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Wave", nullptr));

  tablewave = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tablewave);
  // tablewave->setSortingEnabled(false);
  tablewave->setRowCount(MAXCHANNELNUM);
  tablewave->setColumnCount(3);
  tablewave->setSelectionBehavior(QAbstractItemView::SelectItems);
  //tablewave->setEditTriggers(QAbstractItemView::CurrentChanged);//全部可编辑
  
  QStringList headercol;
  headercol << "Delay(us)" << "Length(us)" << "Dec";
  tablewave->setHorizontalHeaderLabels(headercol);

  QStringList headerrow;
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    headerrow << QString::number(i);
  tablewave->setVerticalHeaderLabels(headerrow);
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 3; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);
	item->setText(QString::asprintf("%f", i*MAXCHANNELNUM+j+0.1));
        tablewave->setItem(i, j, item);
      }

  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[0])
	tablewave->setRowHidden(i, false);
      else
	tablewave->setRowHidden(i, true);
    }


  
  // {
  //   QTableWidgetItem *item = tablewave->item(0, 0);
  //   std::cout << item->text().toStdString().data()  << std::endl;
  // }


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxwave0 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxwave0);
  groupboxwave0->setObjectName(QString::fromUtf8("groupboxwave0"));

  horizontalgbwave0 = new QHBoxLayout(groupboxwave0);
  horizontalgbwave0->setSpacing(10);
  horizontalgbwave0->setContentsMargins(11, 11, 11, 11);
  horizontalgbwave0->setObjectName(QString::fromUtf8("horizontalgbwave0"));
  horizontalgbwave0->setContentsMargins(-1, 2, -1, 2);

  QLabel *label0 = new QLabel(groupboxwave0);
  horizontalgbwave0->addWidget(label0);
  label0->setObjectName(QString::fromUtf8("label0"));
  label0->setText(QApplication::translate("BasicSettings", "Enabled", nullptr));

  for (int i = 0; i < 3; ++i)
    {
      chkboxwaveenabled[i] = new QCheckBox(groupboxwave0);
      horizontalgbwave0->addWidget(chkboxwaveenabled[i]);
      chkboxwaveenabled[i]->setObjectName(QString::fromUtf8("chkboxwaveenabled")+QString::number(i));
      chkboxwaveenabled[i]->setChecked(true);
      if(i == 0)
	chkboxwaveenabled[i]->setText(QString::fromUtf8("Trace Delay"));
      else if(i == 1)
	chkboxwaveenabled[i]->setText(QString::fromUtf8("Trace Length"));
      else if(i == 2)
	chkboxwaveenabled[i]->setText(QString::fromUtf8("Dec"));
    }

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  groupboxwave1 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxwave1);
  groupboxwave1->setObjectName(QString::fromUtf8("groupboxwave1"));

  horizontalgbwave1 = new QHBoxLayout(groupboxwave1);
  horizontalgbwave1->setSpacing(10);
  horizontalgbwave1->setContentsMargins(11, 11, 11, 11);
  horizontalgbwave1->setObjectName(QString::fromUtf8("horizontalgbwave1"));
  horizontalgbwave1->setContentsMargins(-1, 2, -1, 2);

  QLabel *label1 = new QLabel(groupboxwave1);
  horizontalgbwave1->addWidget(label1);
  label1->setObjectName(QString::fromUtf8("label1"));
  label1->setText(QApplication::translate("BasicSettings", "Select", nullptr));

  rbtablewaverow = new QRadioButton(groupboxwave1);
  horizontalgbwave1->addWidget(rbtablewaverow);
  rbtablewaverow->setText(QApplication::translate("BasicSettings", "Row", nullptr));
  rbtablewaverow->setObjectName(QString::fromUtf8("rbtablewaverow"));
  rbtablewaverow->setChecked(false);
  rbtablewavecol = new QRadioButton(groupboxwave1);
  horizontalgbwave1->addWidget(rbtablewavecol);
  rbtablewavecol->setText(QApplication::translate("BasicSettings", "Column", nullptr));
  rbtablewavecol->setObjectName(QString::fromUtf8("rbtablewavecol"));
  rbtablewavecol->setChecked(false);
  rbtablewaveitem = new QRadioButton(groupboxwave1);
  horizontalgbwave1->addWidget(rbtablewaveitem);
  rbtablewaveitem->setText(QApplication::translate("BasicSettings", "Item", nullptr));
  rbtablewaveitem->setObjectName(QString::fromUtf8("rbtablewaveitem"));
  rbtablewaveitem->setChecked(true);


  QLabel *label30 = new QLabel(groupboxwave1);
  horizontalgbwave1->addWidget(label30);
  label30->setObjectName(QString::fromUtf8("label30"));
  label30->setText(QApplication::translate("BasicSettings", "copy from ", nullptr));

 
  cbwavecopych = new QComboBox(groupboxwave1);
  horizontalgbwave1->addWidget(cbwavecopych);
  cbwavecopych->setObjectName(QString::fromUtf8("cbwavecopych"));
  cbwavecopych->setEditable(false);
  for (int i = 0; i < TESTCH[0]; ++i)
    {
      cbwavecopych->addItem(QString::asprintf("ch - %02d", i));
    }

  pbtablewavecopy = new QPushButton(groupboxwave1);
  horizontalgbwave1->addWidget(pbtablewavecopy);
  pbtablewavecopy->setObjectName(QString::fromUtf8("pbtablewavecopy"));
  pbtablewavecopy->setEnabled(true);
  pbtablewavecopy->setText(QApplication::translate("BasicSettings", "Copy", nullptr));
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  groupboxwave3 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxwave3);
  groupboxwave3->setObjectName(QString::fromUtf8("groupboxwave3"));

  horizontalgbwave3 = new QHBoxLayout(groupboxwave3);
  horizontalgbwave3->setSpacing(10);
  horizontalgbwave3->setContentsMargins(11, 11, 11, 11);
  horizontalgbwave3->setObjectName(QString::fromUtf8("horizontalgbwave3"));
  horizontalgbwave3->setContentsMargins(-1, 2, -1, 2);  
    

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  groupboxwave2 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxwave2);
  groupboxwave2->setObjectName(QString::fromUtf8("groupboxwave2"));

  horizontalgbwave2 = new QHBoxLayout(groupboxwave2);
  horizontalgbwave2->setSpacing(10);
  horizontalgbwave2->setContentsMargins(11, 11, 11, 11);
  horizontalgbwave2->setObjectName(QString::fromUtf8("horizontalgbwave2"));
  horizontalgbwave2->setContentsMargins(-1, 2, -1, 2);

  
  pbtablewaveloadselected = new QPushButton(groupboxwave2);
  horizontalgbwave2->addWidget(pbtablewaveloadselected);
  pbtablewaveloadselected->setObjectName(QString::fromUtf8("pbtablewaveloadselected"));
  pbtablewaveloadselected->setEnabled(true);
  pbtablewaveloadselected->setText(QApplication::translate("BasicSettings", "Load Selected", nullptr));

  pbtablewaveloadall = new QPushButton(groupboxwave2);
  horizontalgbwave2->addWidget(pbtablewaveloadall);
  pbtablewaveloadall->setObjectName(QString::fromUtf8("pbtablewaveloadall"));
  pbtablewaveloadall->setEnabled(true);
  pbtablewaveloadall->setText(QApplication::translate("BasicSettings", "Load All", nullptr));
  
  pbtablewaveapplyselected = new QPushButton(groupboxwave2);
  horizontalgbwave2->addWidget(pbtablewaveapplyselected);
  pbtablewaveapplyselected->setObjectName(QString::fromUtf8("pbtablewaveapplyselected"));
  pbtablewaveapplyselected->setEnabled(true);
  pbtablewaveapplyselected->setText(QApplication::translate("BasicSettings", "Apply Selected", nullptr));
  
  pbtablewaveapplyall = new QPushButton(groupboxwave2);
  horizontalgbwave2->addWidget(pbtablewaveapplyall);
  pbtablewaveapplyall->setObjectName(QString::fromUtf8("pbtablewaveapplyall"));
  pbtablewaveapplyall->setEnabled(true);
  pbtablewaveapplyall->setText(QApplication::translate("BasicSettings", "Apply All", nullptr));
  
  
}


void BasicSettings::on_rbtablewaverow_clicked()
{
  tablewave->setSelectionBehavior(QAbstractItemView::SelectRows);
  std::cout << tablewave->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablewaveitem_clicked()
{
  tablewave->setSelectionBehavior(QAbstractItemView::SelectItems);
  std::cout << tablewave->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablewavecol_clicked()
{
  tablewave->setSelectionBehavior(QAbstractItemView::SelectColumns);
  std::cout << tablewave->selectionBehavior() << std::endl;
}


void BasicSettings::on_pbtablewaveloadselected_clicked()
{
  std::cout << "load selected" << std::endl;

  std::cout << tablewave->selectionBehavior() << std::endl;

  QList<QTableWidgetItem*> items = tablewave->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tablewave->row(items.at(i));//获取选中的行
	  int column = tablewave->column(items.at(i));
	  
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



void BasicSettings::on_pbtablewaveloadall_clicked()
{
  std::cout << "load all" << std::endl;
  
}

void BasicSettings::on_pbtablewaveapplyselected_clicked()
{
  std::cout << "apply select" << std::endl;

  
}

void BasicSettings::on_pbtablewaveapplyall_clicked()
{
  std::cout << "apply all" << std::endl;

  
}

void BasicSettings::on_pbtablewavecopy_clicked()
{
  QList<QTableWidgetItem*> items = tablewave->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      // std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tablewave->row(items.at(i));//获取选中的行
	  int column = tablewave->column(items.at(i));
	  
	  // QTableWidgetItem *item = items.at(i);
	  // QString name = item->text();//获取内容
	  // std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }

  std::cout << "CH  " << cbwavecopych->currentIndex() << std::endl;
}


void BasicSettings::on_chkboxwaveenabled0_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablewave->item(i, 0);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxwaveenabled1_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablewave->item(i, 1);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxwaveenabled2_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablewave->item(i, 2);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}






// 
// BasicSettings_TabWaveform.cc ends here
