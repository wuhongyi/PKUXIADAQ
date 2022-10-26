// BasicSettings_TabCFD.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 24 21:12:57 2022 (+0800)
// Last-Updated: 二 10月 25 14:37:59 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 9
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BasicSettings::TabCFD(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "CFD", nullptr));

  tablecfd = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tablecfd);
  // tablecfd->setSortingEnabled(false);
  tablecfd->setRowCount(MAXCHANNELNUM);
  tablecfd->setColumnCount(5);
  tablecfd->setSelectionBehavior(QAbstractItemView::SelectItems);
  //tablecfd->setEditTriggers(QAbstractItemView::CurrentChanged);//全部可编辑
  
  QStringList headercol;
  headercol << "ECT" << "Delay(us)" << "Frac" << "Thre" << "delay filter";
  tablecfd->setHorizontalHeaderLabels(headercol);

  QStringList headerrow;
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    headerrow << QString::number(i);
  tablecfd->setVerticalHeaderLabels(headerrow);
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 5; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);
	if(j == 0)
	  {
	    item->setCheckState(Qt::Checked);
	    item->setText(QString::fromUtf8("Enabled"));
	  }
	else
	  {
	    item->setText(QString::asprintf("%f", i*MAXCHANNELNUM+j+0.1));
	  }
        tablecfd->setItem(i, j, item);
      }

  connect(tablecfd, SIGNAL(cellChanged(int, int)), this, SLOT(TableCFDCheckState(int, int)));
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[0])
	tablecfd->setRowHidden(i, false);
      else
	tablecfd->setRowHidden(i, true);
    }


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxcfd0 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxcfd0);
  groupboxcfd0->setObjectName(QString::fromUtf8("groupboxcfd0"));

  horizontalgbcfd0 = new QHBoxLayout(groupboxcfd0);
  horizontalgbcfd0->setSpacing(10);
  horizontalgbcfd0->setContentsMargins(11, 11, 11, 11);
  horizontalgbcfd0->setObjectName(QString::fromUtf8("horizontalgbcfd0"));
  horizontalgbcfd0->setContentsMargins(-1, 2, -1, 2);

  QLabel *label0 = new QLabel(groupboxcfd0);
  horizontalgbcfd0->addWidget(label0);
  label0->setObjectName(QString::fromUtf8("label0"));
  label0->setText(QApplication::translate("BasicSettings", "Enabled", nullptr));

  for (int i = 0; i < 5; ++i)
    {
      chkboxcfdenabled[i] = new QCheckBox(groupboxcfd0);
      horizontalgbcfd0->addWidget(chkboxcfdenabled[i]);
      chkboxcfdenabled[i]->setObjectName(QString::fromUtf8("chkboxcfdenabled")+QString::number(i));
      chkboxcfdenabled[i]->setChecked(true);

      if(i == 0)
	chkboxcfdenabled[i]->setText(QString::fromUtf8("ECT"));
      else if(i == 1)
	chkboxcfdenabled[i]->setText(QString::fromUtf8("Delay"));
      else if(i == 2)
	chkboxcfdenabled[i]->setText(QString::fromUtf8("Frac"));
      else if(i == 3)
	chkboxcfdenabled[i]->setText(QString::fromUtf8("Thre"));
      else if(i == 4)
	chkboxcfdenabled[i]->setText(QString::fromUtf8("Filter"));
    }

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  groupboxcfd1 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxcfd1);
  groupboxcfd1->setObjectName(QString::fromUtf8("groupboxcfd1"));

  horizontalgbcfd1 = new QHBoxLayout(groupboxcfd1);
  horizontalgbcfd1->setSpacing(10);
  horizontalgbcfd1->setContentsMargins(11, 11, 11, 11);
  horizontalgbcfd1->setObjectName(QString::fromUtf8("horizontalgbcfd1"));
  horizontalgbcfd1->setContentsMargins(-1, 2, -1, 2);

  QLabel *label1 = new QLabel(groupboxcfd1);
  horizontalgbcfd1->addWidget(label1);
  label1->setObjectName(QString::fromUtf8("label1"));
  label1->setText(QApplication::translate("BasicSettings", "Select", nullptr));

  rbtablecfdrow = new QRadioButton(groupboxcfd1);
  horizontalgbcfd1->addWidget(rbtablecfdrow);
  rbtablecfdrow->setText(QApplication::translate("BasicSettings", "Row", nullptr));
  rbtablecfdrow->setObjectName(QString::fromUtf8("rbtablecfdrow"));
  rbtablecfdrow->setChecked(false);
  rbtablecfdcol = new QRadioButton(groupboxcfd1);
  horizontalgbcfd1->addWidget(rbtablecfdcol);
  rbtablecfdcol->setText(QApplication::translate("BasicSettings", "Column", nullptr));
  rbtablecfdcol->setObjectName(QString::fromUtf8("rbtablecfdcol"));
  rbtablecfdcol->setChecked(false);
  rbtablecfditem = new QRadioButton(groupboxcfd1);
  horizontalgbcfd1->addWidget(rbtablecfditem);
  rbtablecfditem->setText(QApplication::translate("BasicSettings", "Item", nullptr));
  rbtablecfditem->setObjectName(QString::fromUtf8("rbtablecfditem"));
  rbtablecfditem->setChecked(true);


  QLabel *label30 = new QLabel(groupboxcfd1);
  horizontalgbcfd1->addWidget(label30);
  label30->setObjectName(QString::fromUtf8("label30"));
  label30->setText(QApplication::translate("BasicSettings", "copy from ", nullptr));

 
  cbcfdcopych = new QComboBox(groupboxcfd1);
  horizontalgbcfd1->addWidget(cbcfdcopych);
  cbcfdcopych->setObjectName(QString::fromUtf8("cbcfdcopych"));
  cbcfdcopych->setEditable(false);
  for (int i = 0; i < TESTCH[0]; ++i)
    {
      cbcfdcopych->addItem(QString::asprintf("ch - %02d", i));
    }

  pbtablecfdcopy = new QPushButton(groupboxcfd1);
  horizontalgbcfd1->addWidget(pbtablecfdcopy);
  pbtablecfdcopy->setObjectName(QString::fromUtf8("pbtablecfdcopy"));
  pbtablecfdcopy->setEnabled(true);
  pbtablecfdcopy->setText(QApplication::translate("BasicSettings", "Copy", nullptr));
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  groupboxcfd3 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxcfd3);
  groupboxcfd3->setObjectName(QString::fromUtf8("groupboxcfd3"));

  horizontalgbcfd3 = new QHBoxLayout(groupboxcfd3);
  horizontalgbcfd3->setSpacing(10);
  horizontalgbcfd3->setContentsMargins(11, 11, 11, 11);
  horizontalgbcfd3->setObjectName(QString::fromUtf8("horizontalgbcfd3"));
  horizontalgbcfd3->setContentsMargins(-1, 2, -1, 2);  
    

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  groupboxcfd2 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxcfd2);
  groupboxcfd2->setObjectName(QString::fromUtf8("groupboxcfd2"));

  horizontalgbcfd2 = new QHBoxLayout(groupboxcfd2);
  horizontalgbcfd2->setSpacing(10);
  horizontalgbcfd2->setContentsMargins(11, 11, 11, 11);
  horizontalgbcfd2->setObjectName(QString::fromUtf8("horizontalgbcfd2"));
  horizontalgbcfd2->setContentsMargins(-1, 2, -1, 2);

  
  pbtablecfdloadselected = new QPushButton(groupboxcfd2);
  horizontalgbcfd2->addWidget(pbtablecfdloadselected);
  pbtablecfdloadselected->setObjectName(QString::fromUtf8("pbtablecfdloadselected"));
  pbtablecfdloadselected->setEnabled(true);
  pbtablecfdloadselected->setText(QApplication::translate("BasicSettings", "Load Selected", nullptr));

  pbtablecfdloadall = new QPushButton(groupboxcfd2);
  horizontalgbcfd2->addWidget(pbtablecfdloadall);
  pbtablecfdloadall->setObjectName(QString::fromUtf8("pbtablecfdloadall"));
  pbtablecfdloadall->setEnabled(true);
  pbtablecfdloadall->setText(QApplication::translate("BasicSettings", "Load All", nullptr));
  
  pbtablecfdapplyselected = new QPushButton(groupboxcfd2);
  horizontalgbcfd2->addWidget(pbtablecfdapplyselected);
  pbtablecfdapplyselected->setObjectName(QString::fromUtf8("pbtablecfdapplyselected"));
  pbtablecfdapplyselected->setEnabled(true);
  pbtablecfdapplyselected->setText(QApplication::translate("BasicSettings", "Apply Selected", nullptr));
  
  pbtablecfdapplyall = new QPushButton(groupboxcfd2);
  horizontalgbcfd2->addWidget(pbtablecfdapplyall);
  pbtablecfdapplyall->setObjectName(QString::fromUtf8("pbtablecfdapplyall"));
  pbtablecfdapplyall->setEnabled(true);
  pbtablecfdapplyall->setText(QApplication::translate("BasicSettings", "Apply All", nullptr));
  
}


void BasicSettings::on_rbtablecfdrow_clicked()
{
  tablecfd->setSelectionBehavior(QAbstractItemView::SelectRows);
  std::cout << tablecfd->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablecfditem_clicked()
{
  tablecfd->setSelectionBehavior(QAbstractItemView::SelectItems);
  std::cout << tablecfd->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtablecfdcol_clicked()
{
  tablecfd->setSelectionBehavior(QAbstractItemView::SelectColumns);
  std::cout << tablecfd->selectionBehavior() << std::endl;
}


void BasicSettings::on_pbtablecfdloadselected_clicked()
{
  std::cout << "load selected" << std::endl;

  std::cout << tablecfd->selectionBehavior() << std::endl;

  QList<QTableWidgetItem*> items = tablecfd->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tablecfd->row(items.at(i));//获取选中的行
	  int column = tablecfd->column(items.at(i));
	  
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



void BasicSettings::on_pbtablecfdloadall_clicked()
{
  std::cout << "load all" << std::endl;
  
}

void BasicSettings::on_pbtablecfdapplyselected_clicked()
{
  std::cout << "apply select" << std::endl;

  
}

void BasicSettings::on_pbtablecfdapplyall_clicked()
{
  std::cout << "apply all" << std::endl;

  
}

void BasicSettings::on_pbtablecfdcopy_clicked()
{
  QList<QTableWidgetItem*> items = tablecfd->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      // std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tablecfd->row(items.at(i));//获取选中的行
	  int column = tablecfd->column(items.at(i));
	  
	  // QTableWidgetItem *item = items.at(i);
	  // QString name = item->text();//获取内容
	  // std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }

  std::cout << "CH  " << cbcfdcopych->currentIndex() << std::endl;
}


void BasicSettings::on_chkboxcfdenabled0_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablecfd->item(i, 0);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsUserCheckable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxcfdenabled1_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablecfd->item(i, 1);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxcfdenabled2_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablecfd->item(i, 2);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxcfdenabled3_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablecfd->item(i, 3);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxcfdenabled4_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tablecfd->item(i, 4);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}


void BasicSettings::TableCFDCheckState(int row, int column)
{
  if(column != 0)
    {
      // std::cout<< "table cfd error: " << row << " " << column << std::endl;
      return;
    }
  if(tablecfd->item(row, column)->checkState() == Qt::Checked)
    {
      tablecfd->item(row, column)->setText(QString::fromUtf8("Enabled"));
    }
  else
    {
      tablecfd->item(row, column)->setText(QString::fromUtf8("Disabled"));
    }
}

// 
// BasicSettings_TabCFD.cc ends here
