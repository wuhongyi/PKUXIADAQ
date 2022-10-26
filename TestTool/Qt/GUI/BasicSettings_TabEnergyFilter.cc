// BasicSettings_TabEnergyFilter.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 24 21:15:22 2022 (+0800)
// Last-Updated: 二 10月 25 12:25:29 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 3
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BasicSettings::TabEnergyFilter(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Energy", nullptr));

  tableenergy = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tableenergy);
  // tableenergy->setSortingEnabled(false);
  tableenergy->setRowCount(MAXCHANNELNUM);
  tableenergy->setColumnCount(4);
  tableenergy->setSelectionBehavior(QAbstractItemView::SelectItems);
  //tableenergy->setEditTriggers(QAbstractItemView::CurrentChanged);//全部可编辑
  
  QStringList headercol;
  headercol << "Rise(us)" << "Top(us)" << "Tau(us)" << "Find tau(us)";
  tableenergy->setHorizontalHeaderLabels(headercol);

  QStringList headerrow;
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    headerrow << QString::number(i);
  tableenergy->setVerticalHeaderLabels(headerrow);
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 4; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);
	item->setText(QString::asprintf("%f", i*MAXCHANNELNUM+j+0.1));
	if(j == 3) item->setFlags(Qt::ItemIsEnabled);
        tableenergy->setItem(i, j, item);
      }

  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[0])
	tableenergy->setRowHidden(i, false);
      else
	tableenergy->setRowHidden(i, true);
    }


  
  // {
  //   QTableWidgetItem *item = tableenergy->item(0, 0);
  //   std::cout << item->text().toStdString().data()  << std::endl;
  // }


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxenergy0 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxenergy0);
  groupboxenergy0->setObjectName(QString::fromUtf8("groupboxenergy0"));

  horizontalgbenergy0 = new QHBoxLayout(groupboxenergy0);
  horizontalgbenergy0->setSpacing(10);
  horizontalgbenergy0->setContentsMargins(11, 11, 11, 11);
  horizontalgbenergy0->setObjectName(QString::fromUtf8("horizontalgbenergy0"));
  horizontalgbenergy0->setContentsMargins(-1, 2, -1, 2);

  QLabel *label0 = new QLabel(groupboxenergy0);
  horizontalgbenergy0->addWidget(label0);
  label0->setObjectName(QString::fromUtf8("label0"));
  label0->setText(QApplication::translate("BasicSettings", "Enabled", nullptr));

  for (int i = 0; i < 3; ++i)
    {
      chkboxenergyenabled[i] = new QCheckBox(groupboxenergy0);
      horizontalgbenergy0->addWidget(chkboxenergyenabled[i]);
      chkboxenergyenabled[i]->setObjectName(QString::fromUtf8("chkboxenergyenabled")+QString::number(i));
      chkboxenergyenabled[i]->setChecked(true);

      if(i == 0)
	chkboxenergyenabled[i]->setText(QString::fromUtf8("Rise"));
      else if(i == 1)
	chkboxenergyenabled[i]->setText(QString::fromUtf8("Top"));
      else if(i == 2)
	chkboxenergyenabled[i]->setText(QString::fromUtf8("Tau"));
    }

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  groupboxenergy1 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxenergy1);
  groupboxenergy1->setObjectName(QString::fromUtf8("groupboxenergy1"));

  horizontalgbenergy1 = new QHBoxLayout(groupboxenergy1);
  horizontalgbenergy1->setSpacing(10);
  horizontalgbenergy1->setContentsMargins(11, 11, 11, 11);
  horizontalgbenergy1->setObjectName(QString::fromUtf8("horizontalgbenergy1"));
  horizontalgbenergy1->setContentsMargins(-1, 2, -1, 2);

  QLabel *label1 = new QLabel(groupboxenergy1);
  horizontalgbenergy1->addWidget(label1);
  label1->setObjectName(QString::fromUtf8("label1"));
  label1->setText(QApplication::translate("BasicSettings", "Select", nullptr));

  rbtableenergyrow = new QRadioButton(groupboxenergy1);
  horizontalgbenergy1->addWidget(rbtableenergyrow);
  rbtableenergyrow->setText(QApplication::translate("BasicSettings", "Row", nullptr));
  rbtableenergyrow->setObjectName(QString::fromUtf8("rbtableenergyrow"));
  rbtableenergyrow->setChecked(false);
  rbtableenergycol = new QRadioButton(groupboxenergy1);
  horizontalgbenergy1->addWidget(rbtableenergycol);
  rbtableenergycol->setText(QApplication::translate("BasicSettings", "Column", nullptr));
  rbtableenergycol->setObjectName(QString::fromUtf8("rbtableenergycol"));
  rbtableenergycol->setChecked(false);
  rbtableenergyitem = new QRadioButton(groupboxenergy1);
  horizontalgbenergy1->addWidget(rbtableenergyitem);
  rbtableenergyitem->setText(QApplication::translate("BasicSettings", "Item", nullptr));
  rbtableenergyitem->setObjectName(QString::fromUtf8("rbtableenergyitem"));
  rbtableenergyitem->setChecked(true);


  QLabel *label30 = new QLabel(groupboxenergy1);
  horizontalgbenergy1->addWidget(label30);
  label30->setObjectName(QString::fromUtf8("label30"));
  label30->setText(QApplication::translate("BasicSettings", "copy from ", nullptr));

 
  cbenergycopych = new QComboBox(groupboxenergy1);
  horizontalgbenergy1->addWidget(cbenergycopych);
  cbenergycopych->setObjectName(QString::fromUtf8("cbenergycopych"));
  cbenergycopych->setEditable(false);
  for (int i = 0; i < TESTCH[0]; ++i)
    {
      cbenergycopych->addItem(QString::asprintf("ch - %02d", i));
    }

  pbtableenergycopy = new QPushButton(groupboxenergy1);
  horizontalgbenergy1->addWidget(pbtableenergycopy);
  pbtableenergycopy->setObjectName(QString::fromUtf8("pbtableenergycopy"));
  pbtableenergycopy->setEnabled(true);
  pbtableenergycopy->setText(QApplication::translate("BasicSettings", "Copy", nullptr));
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  groupboxenergy3 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxenergy3);
  groupboxenergy3->setObjectName(QString::fromUtf8("groupboxenergy3"));

  horizontalgbenergy3 = new QHBoxLayout(groupboxenergy3);
  horizontalgbenergy3->setSpacing(10);
  horizontalgbenergy3->setContentsMargins(11, 11, 11, 11);
  horizontalgbenergy3->setObjectName(QString::fromUtf8("horizontalgbenergy3"));
  horizontalgbenergy3->setContentsMargins(-1, 2, -1, 2);  
    

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  groupboxenergy2 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxenergy2);
  groupboxenergy2->setObjectName(QString::fromUtf8("groupboxenergy2"));

  horizontalgbenergy2 = new QHBoxLayout(groupboxenergy2);
  horizontalgbenergy2->setSpacing(10);
  horizontalgbenergy2->setContentsMargins(11, 11, 11, 11);
  horizontalgbenergy2->setObjectName(QString::fromUtf8("horizontalgbenergy2"));
  horizontalgbenergy2->setContentsMargins(-1, 2, -1, 2);

  
  pbtableenergyloadselected = new QPushButton(groupboxenergy2);
  horizontalgbenergy2->addWidget(pbtableenergyloadselected);
  pbtableenergyloadselected->setObjectName(QString::fromUtf8("pbtableenergyloadselected"));
  pbtableenergyloadselected->setEnabled(true);
  pbtableenergyloadselected->setText(QApplication::translate("BasicSettings", "Load Selected", nullptr));

  pbtableenergyloadall = new QPushButton(groupboxenergy2);
  horizontalgbenergy2->addWidget(pbtableenergyloadall);
  pbtableenergyloadall->setObjectName(QString::fromUtf8("pbtableenergyloadall"));
  pbtableenergyloadall->setEnabled(true);
  pbtableenergyloadall->setText(QApplication::translate("BasicSettings", "Load All", nullptr));
  
  pbtableenergyapplyselected = new QPushButton(groupboxenergy2);
  horizontalgbenergy2->addWidget(pbtableenergyapplyselected);
  pbtableenergyapplyselected->setObjectName(QString::fromUtf8("pbtableenergyapplyselected"));
  pbtableenergyapplyselected->setEnabled(true);
  pbtableenergyapplyselected->setText(QApplication::translate("BasicSettings", "Apply Selected", nullptr));
  
  pbtableenergyapplyall = new QPushButton(groupboxenergy2);
  horizontalgbenergy2->addWidget(pbtableenergyapplyall);
  pbtableenergyapplyall->setObjectName(QString::fromUtf8("pbtableenergyapplyall"));
  pbtableenergyapplyall->setEnabled(true);
  pbtableenergyapplyall->setText(QApplication::translate("BasicSettings", "Apply All", nullptr));
  
  
}



void BasicSettings::on_rbtableenergyrow_clicked()
{
  tableenergy->setSelectionBehavior(QAbstractItemView::SelectRows);
  std::cout << tableenergy->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtableenergyitem_clicked()
{
  tableenergy->setSelectionBehavior(QAbstractItemView::SelectItems);
  std::cout << tableenergy->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtableenergycol_clicked()
{
  tableenergy->setSelectionBehavior(QAbstractItemView::SelectColumns);
  std::cout << tableenergy->selectionBehavior() << std::endl;
}


void BasicSettings::on_pbtableenergyloadselected_clicked()
{
  std::cout << "load selected" << std::endl;

  std::cout << tableenergy->selectionBehavior() << std::endl;

  QList<QTableWidgetItem*> items = tableenergy->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tableenergy->row(items.at(i));//获取选中的行
	  int column = tableenergy->column(items.at(i));
	  
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



void BasicSettings::on_pbtableenergyloadall_clicked()
{
  std::cout << "load all" << std::endl;
  
}

void BasicSettings::on_pbtableenergyapplyselected_clicked()
{
  std::cout << "apply select" << std::endl;

  
}

void BasicSettings::on_pbtableenergyapplyall_clicked()
{
  std::cout << "apply all" << std::endl;

  
}

void BasicSettings::on_pbtableenergycopy_clicked()
{
  QList<QTableWidgetItem*> items = tableenergy->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      // std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tableenergy->row(items.at(i));//获取选中的行
	  int column = tableenergy->column(items.at(i));
	  
	  // QTableWidgetItem *item = items.at(i);
	  // QString name = item->text();//获取内容
	  // std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }

  std::cout << "CH  " << cbenergycopych->currentIndex() << std::endl;
}


void BasicSettings::on_chkboxenergyenabled0_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableenergy->item(i, 0);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxenergyenabled1_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableenergy->item(i, 1);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxenergyenabled2_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableenergy->item(i, 2);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}


// 
// BasicSettings_TabEnergyFilter.cc ends here
