// BasicSettings_TabPolarityGainBaseline.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 24 21:11:56 2022 (+0800)
// Last-Updated: 二 10月 25 14:16:55 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BasicSettings::TabPolarityGainBaseline(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Base", nullptr));

  tablepgb = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tablepgb);
  // tablepgb->setSortingEnabled(false);
  tablepgb->setRowCount(MAXCHANNELNUM);
  tablepgb->setColumnCount(5);
  tablepgb->setSelectionBehavior(QAbstractItemView::SelectItems);
  //tablepgb->setEditTriggers(QAbstractItemView::CurrentChanged);//全部可编辑
  
  QStringList headercol;
  headercol << "Polarity" << "Gain" << "Baseline(%)" << "DCOffset" << "BLcut";
  tablepgb->setHorizontalHeaderLabels(headercol);

  QStringList headerrow;
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    headerrow << QString::number(i);
  tablepgb->setVerticalHeaderLabels(headerrow);
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 5; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);

	if(j == 0)
	  {
	    item->setCheckState(Qt::Checked);
	    item->setText(QString::fromUtf8("+"));
	  }
	else if(j == 1)
	  {
	    item->setCheckState(Qt::Checked);
	    item->setText(QString::fromUtf8("Large"));
	  }
	else
	  {
	    item->setText(QString::asprintf("%f", i*MAXCHANNELNUM+j+0.1));
	  }

	if(j == 3 || j == 4) item->setFlags(Qt::ItemIsEnabled);
	tablepgb->setItem(i, j, item);
      }

  connect(tablepgb, SIGNAL(cellChanged(int, int)), this, SLOT(TablePolarityGainBaselineCheckState(int, int)));
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[0])
	tablepgb->setRowHidden(i, false);
      else
	tablepgb->setRowHidden(i, true);
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxpgb0 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxpgb0);
  groupboxpgb0->setObjectName(QString::fromUtf8("groupboxpgb0"));

  horizontalgbpgb0 = new QHBoxLayout(groupboxpgb0);
  horizontalgbpgb0->setSpacing(10);
  horizontalgbpgb0->setContentsMargins(11, 11, 11, 11);
  horizontalgbpgb0->setObjectName(QString::fromUtf8("horizontalgbpgb0"));
  horizontalgbpgb0->setContentsMargins(-1, 2, -1, 2);

  QLabel *label0 = new QLabel(groupboxpgb0);
  horizontalgbpgb0->addWidget(label0);
  label0->setObjectName(QString::fromUtf8("label0"));
  label0->setText(QApplication::translate("BasicSettings", "Enabled", nullptr));

  for (int i = 0; i < 3; ++i)
    {
      chkboxpgbenabled[i] = new QCheckBox(groupboxpgb0);
      horizontalgbpgb0->addWidget(chkboxpgbenabled[i]);
      chkboxpgbenabled[i]->setObjectName(QString::fromUtf8("chkboxpgbenabled")+QString::number(i));
      chkboxpgbenabled[i]->setChecked(true);

      if(i == 0)
	chkboxpgbenabled[i]->setText(QString::fromUtf8("Polarity"));
      else if(i == 1)
	chkboxpgbenabled[i]->setText(QString::fromUtf8("Gain"));
      else if(i == 2)
	chkboxpgbenabled[i]->setText(QString::fromUtf8("Baseline(%)"));
    }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  groupboxpgb1 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxpgb1);
  groupboxpgb1->setObjectName(QString::fromUtf8("groupboxpgb1"));

  horizontalgbpgb1 = new QHBoxLayout(groupboxpgb1);
  horizontalgbpgb1->setSpacing(10);
  horizontalgbpgb1->setContentsMargins(11, 11, 11, 11);
  horizontalgbpgb1->setObjectName(QString::fromUtf8("horizontalgbpgb1"));
  horizontalgbpgb1->setContentsMargins(-1, 2, -1, 2);

  QLabel *label1 = new QLabel(groupboxpgb1);
  horizontalgbpgb1->addWidget(label1);
  label1->setObjectName(QString::fromUtf8("label1"));
  label1->setText(QApplication::translate("BasicSettings", "Select", nullptr));

  rbtablepgbrow = new QRadioButton(groupboxpgb1);
  horizontalgbpgb1->addWidget(rbtablepgbrow);
  rbtablepgbrow->setText(QApplication::translate("BasicSettings", "Row", nullptr));
  rbtablepgbrow->setObjectName(QString::fromUtf8("rbtablepgbrow"));
  rbtablepgbrow->setChecked(false);
  rbtablepgbcol = new QRadioButton(groupboxpgb1);
  horizontalgbpgb1->addWidget(rbtablepgbcol);
  rbtablepgbcol->setText(QApplication::translate("BasicSettings", "Column", nullptr));
  rbtablepgbcol->setObjectName(QString::fromUtf8("rbtablepgbcol"));
  rbtablepgbcol->setChecked(false);
  rbtablepgbitem = new QRadioButton(groupboxpgb1);
  horizontalgbpgb1->addWidget(rbtablepgbitem);
  rbtablepgbitem->setText(QApplication::translate("BasicSettings", "Item", nullptr));
  rbtablepgbitem->setObjectName(QString::fromUtf8("rbtablepgbitem"));
  rbtablepgbitem->setChecked(true);


  QLabel *label30 = new QLabel(groupboxpgb1);
  horizontalgbpgb1->addWidget(label30);
  label30->setObjectName(QString::fromUtf8("label30"));
  label30->setText(QApplication::translate("BasicSettings", "copy from ", nullptr));

 
  cbpgbcopych = new QComboBox(groupboxpgb1);
  horizontalgbpgb1->addWidget(cbpgbcopych);
  cbpgbcopych->setObjectName(QString::fromUtf8("cbpgbcopych"));
  cbpgbcopych->setEditable(false);
  for (int i = 0; i < TESTCH[0]; ++i)
    {
      cbpgbcopych->addItem(QString::asprintf("ch - %02d", i));
    }

  pbtablepgbcopy = new QPushButton(groupboxpgb1);
  horizontalgbpgb1->addWidget(pbtablepgbcopy);
  pbtablepgbcopy->setObjectName(QString::fromUtf8("pbtablepgbcopy"));
  pbtablepgbcopy->setEnabled(true);
  pbtablepgbcopy->setText(QApplication::translate("BasicSettings", "Copy", nullptr));
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  groupboxpgb3 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxpgb3);
  groupboxpgb3->setObjectName(QString::fromUtf8("groupboxpgb3"));

  horizontalgbpgb3 = new QHBoxLayout(groupboxpgb3);
  horizontalgbpgb3->setSpacing(10);
  horizontalgbpgb3->setContentsMargins(11, 11, 11, 11);
  horizontalgbpgb3->setObjectName(QString::fromUtf8("horizontalgbpgb3"));
  horizontalgbpgb3->setContentsMargins(-1, 2, -1, 2);  
    

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  groupboxpgb2 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxpgb2);
  groupboxpgb2->setObjectName(QString::fromUtf8("groupboxpgb2"));

  horizontalgbpgb2 = new QHBoxLayout(groupboxpgb2);
  horizontalgbpgb2->setSpacing(10);
  horizontalgbpgb2->setContentsMargins(11, 11, 11, 11);
  horizontalgbpgb2->setObjectName(QString::fromUtf8("horizontalgbpgb2"));
  horizontalgbpgb2->setContentsMargins(-1, 2, -1, 2);

  
  pbtablepgbloadselected = new QPushButton(groupboxpgb2);
  horizontalgbpgb2->addWidget(pbtablepgbloadselected);
  pbtablepgbloadselected->setObjectName(QString::fromUtf8("pbtablepgbloadselected"));
  pbtablepgbloadselected->setEnabled(true);
  pbtablepgbloadselected->setText(QApplication::translate("BasicSettings", "Load Selected", nullptr));

  pbtablepgbloadall = new QPushButton(groupboxpgb2);
  horizontalgbpgb2->addWidget(pbtablepgbloadall);
  pbtablepgbloadall->setObjectName(QString::fromUtf8("pbtablepgbloadall"));
  pbtablepgbloadall->setEnabled(true);
  pbtablepgbloadall->setText(QApplication::translate("BasicSettings", "Load All", nullptr));
  
  pbtablepgbapplyselected = new QPushButton(groupboxpgb2);
  horizontalgbpgb2->addWidget(pbtablepgbapplyselected);
  pbtablepgbapplyselected->setObjectName(QString::fromUtf8("pbtablepgbapplyselected"));
  pbtablepgbapplyselected->setEnabled(true);
  pbtablepgbapplyselected->setText(QApplication::translate("BasicSettings", "Apply Selected", nullptr));
  
  pbtablepgbapplyall = new QPushButton(groupboxpgb2);
  horizontalgbpgb2->addWidget(pbtablepgbapplyall);
  pbtablepgbapplyall->setObjectName(QString::fromUtf8("pbtablepgbapplyall"));
  pbtablepgbapplyall->setEnabled(true);
  pbtablepgbapplyall->setText(QApplication::translate("BasicSettings", "Apply All", nullptr));
    
}



void BasicSettings::on_rbtablepgbrow_clicked()
{
  tablepgb->setSelectionBehavior(QAbstractItemView::SelectRows);
  std::cout << tablepgb->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablepgbitem_clicked()
{
  tablepgb->setSelectionBehavior(QAbstractItemView::SelectItems);
  std::cout << tablepgb->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablepgbcol_clicked()
{
  tablepgb->setSelectionBehavior(QAbstractItemView::SelectColumns);
  std::cout << tablepgb->selectionBehavior() << std::endl;
}


void BasicSettings::on_pbtablepgbloadselected_clicked()
{
  std::cout << "load selected" << std::endl;

  std::cout << tablepgb->selectionBehavior() << std::endl;

  QList<QTableWidgetItem*> items = tablepgb->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tablepgb->row(items.at(i));//获取选中的行
	  int column = tablepgb->column(items.at(i));
	  
	  QTableWidgetItem *item = items.at(i);
	  QString name = item->text();//获取内容
	  std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }


  
  // QItemSelectionModel
  //   QModelIndexList
  
}



void BasicSettings::on_pbtablepgbloadall_clicked()
{
  std::cout << "load all" << std::endl;
  
}

void BasicSettings::on_pbtablepgbapplyselected_clicked()
{
  std::cout << "apply select" << std::endl;

  
}

void BasicSettings::on_pbtablepgbapplyall_clicked()
{
  std::cout << "apply all" << std::endl;

  
}

void BasicSettings::on_pbtablepgbcopy_clicked()
{
  QList<QTableWidgetItem*> items = tablepgb->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      // std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tablepgb->row(items.at(i));//获取选中的行
	  int column = tablepgb->column(items.at(i));
	  
	  // QTableWidgetItem *item = items.at(i);
	  // QString name = item->text();//获取内容
	  // std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }

  std::cout << "CH  " << cbpgbcopych->currentIndex() << std::endl;
}


void BasicSettings::on_chkboxpgbenabled0_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablepgb->item(i, 0);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxpgbenabled1_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablepgb->item(i, 1);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxpgbenabled2_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablepgb->item(i, 2);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}


void BasicSettings::TablePolarityGainBaselineCheckState(int row, int column)
{
  if(column >= 2)
    {
      // std::cout<< "table pgb error: " << row << " " << column << std::endl;
      return;
    }

  if(tablepgb->item(row, column)->checkState() == Qt::Checked)
    {
      if(column == 0)
	tablepgb->item(row, column)->setText(QString::fromUtf8("+"));
      else if(column == 1)
	tablepgb->item(row, column)->setText(QString::fromUtf8("Large"));
    }
  else
    {
      if(column == 0)
	tablepgb->item(row, column)->setText(QString::fromUtf8("-"));
      else if(column == 1)
	tablepgb->item(row, column)->setText(QString::fromUtf8("Small"));
    }
}



// 
// BasicSettings_TabPolarityGainBaseline.cc ends here
