// BasicSettings_TabQDC.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 24 20:54:25 2022 (+0800)
// Last-Updated: 二 10月 25 12:25:49 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BasicSettings::TabQDC(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "QDC", nullptr));

  tableqdc = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tableqdc);
  // tableqdc->setSortingEnabled(false);
  tableqdc->setRowCount(MAXCHANNELNUM);
  tableqdc->setColumnCount(8);
  tableqdc->setSelectionBehavior(QAbstractItemView::SelectItems);
  //tableqdc->setEditTriggers(QAbstractItemView::CurrentChanged);//全部可编辑
  
  QStringList headercol;
  headercol << "QDC0(us)" << "QDC1(us)" << "QDC2(us)" << "QDC3(us)" << "QDC4(us)" << "QDC5(us)" << "QDC6(us)" << "QDC7(us)";
  tableqdc->setHorizontalHeaderLabels(headercol);

  QStringList headerrow;
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    headerrow << QString::number(i);
  tableqdc->setVerticalHeaderLabels(headerrow);
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 8; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);
	item->setText(QString::asprintf("%f", i*MAXCHANNELNUM+j+0.1));
        tableqdc->setItem(i, j, item);
      }

  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[0])
	tableqdc->setRowHidden(i, false);
      else
	tableqdc->setRowHidden(i, true);
    }


  
  // {
  //   QTableWidgetItem *item = tableqdc->item(0, 0);
  //   std::cout << item->text().toStdString().data()  << std::endl;
  // }


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxqdc0 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxqdc0);
  groupboxqdc0->setObjectName(QString::fromUtf8("groupboxqdc0"));

  horizontalgbqdc0 = new QHBoxLayout(groupboxqdc0);
  horizontalgbqdc0->setSpacing(10);
  horizontalgbqdc0->setContentsMargins(11, 11, 11, 11);
  horizontalgbqdc0->setObjectName(QString::fromUtf8("horizontalgbqdc0"));
  horizontalgbqdc0->setContentsMargins(-1, 2, -1, 2);

  QLabel *label0 = new QLabel(groupboxqdc0);
  horizontalgbqdc0->addWidget(label0);
  label0->setObjectName(QString::fromUtf8("label0"));
  label0->setText(QApplication::translate("BasicSettings", "Enabled", nullptr));

  for (int i = 0; i < 8; ++i)
    {
      chkboxqdcenabled[i] = new QCheckBox(groupboxqdc0);
      horizontalgbqdc0->addWidget(chkboxqdcenabled[i]);
      chkboxqdcenabled[i]->setObjectName(QString::fromUtf8("chkboxqdcenabled")+QString::number(i));
      chkboxqdcenabled[i]->setChecked(true);
      chkboxqdcenabled[i]->setText(QString::asprintf("QDC%d", i));
    }

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  groupboxqdc1 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxqdc1);
  groupboxqdc1->setObjectName(QString::fromUtf8("groupboxqdc1"));

  horizontalgbqdc1 = new QHBoxLayout(groupboxqdc1);
  horizontalgbqdc1->setSpacing(10);
  horizontalgbqdc1->setContentsMargins(11, 11, 11, 11);
  horizontalgbqdc1->setObjectName(QString::fromUtf8("horizontalgbqdc1"));
  horizontalgbqdc1->setContentsMargins(-1, 2, -1, 2);

  QLabel *label1 = new QLabel(groupboxqdc1);
  horizontalgbqdc1->addWidget(label1);
  label1->setObjectName(QString::fromUtf8("label1"));
  label1->setText(QApplication::translate("BasicSettings", "Select", nullptr));

  rbtableqdcrow = new QRadioButton(groupboxqdc1);
  horizontalgbqdc1->addWidget(rbtableqdcrow);
  rbtableqdcrow->setText(QApplication::translate("BasicSettings", "Row", nullptr));
  rbtableqdcrow->setObjectName(QString::fromUtf8("rbtableqdcrow"));
  rbtableqdcrow->setChecked(false);
  rbtableqdccol = new QRadioButton(groupboxqdc1);
  horizontalgbqdc1->addWidget(rbtableqdccol);
  rbtableqdccol->setText(QApplication::translate("BasicSettings", "Column", nullptr));
  rbtableqdccol->setObjectName(QString::fromUtf8("rbtableqdccol"));
  rbtableqdccol->setChecked(false);
  rbtableqdcitem = new QRadioButton(groupboxqdc1);
  horizontalgbqdc1->addWidget(rbtableqdcitem);
  rbtableqdcitem->setText(QApplication::translate("BasicSettings", "Item", nullptr));
  rbtableqdcitem->setObjectName(QString::fromUtf8("rbtableqdcitem"));
  rbtableqdcitem->setChecked(true);


  QLabel *label30 = new QLabel(groupboxqdc1);
  horizontalgbqdc1->addWidget(label30);
  label30->setObjectName(QString::fromUtf8("label30"));
  label30->setText(QApplication::translate("BasicSettings", "copy from ", nullptr));

 
  cbqdccopych = new QComboBox(groupboxqdc1);
  horizontalgbqdc1->addWidget(cbqdccopych);
  cbqdccopych->setObjectName(QString::fromUtf8("cbqdccopych"));
  cbqdccopych->setEditable(false);
  for (int i = 0; i < TESTCH[0]; ++i)
    {
      cbqdccopych->addItem(QString::asprintf("ch - %02d", i));
    }

  pbtableqdccopy = new QPushButton(groupboxqdc1);
  horizontalgbqdc1->addWidget(pbtableqdccopy);
  pbtableqdccopy->setObjectName(QString::fromUtf8("pbtableqdccopy"));
  pbtableqdccopy->setEnabled(true);
  pbtableqdccopy->setText(QApplication::translate("BasicSettings", "Copy", nullptr));
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  groupboxqdc3 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxqdc3);
  groupboxqdc3->setObjectName(QString::fromUtf8("groupboxqdc3"));

  horizontalgbqdc3 = new QHBoxLayout(groupboxqdc3);
  horizontalgbqdc3->setSpacing(10);
  horizontalgbqdc3->setContentsMargins(11, 11, 11, 11);
  horizontalgbqdc3->setObjectName(QString::fromUtf8("horizontalgbqdc3"));
  horizontalgbqdc3->setContentsMargins(-1, 2, -1, 2);  
    

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  groupboxqdc2 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxqdc2);
  groupboxqdc2->setObjectName(QString::fromUtf8("groupboxqdc2"));

  horizontalgbqdc2 = new QHBoxLayout(groupboxqdc2);
  horizontalgbqdc2->setSpacing(10);
  horizontalgbqdc2->setContentsMargins(11, 11, 11, 11);
  horizontalgbqdc2->setObjectName(QString::fromUtf8("horizontalgbqdc2"));
  horizontalgbqdc2->setContentsMargins(-1, 2, -1, 2);

  
  pbtableqdcloadselected = new QPushButton(groupboxqdc2);
  horizontalgbqdc2->addWidget(pbtableqdcloadselected);
  pbtableqdcloadselected->setObjectName(QString::fromUtf8("pbtableqdcloadselected"));
  pbtableqdcloadselected->setEnabled(true);
  pbtableqdcloadselected->setText(QApplication::translate("BasicSettings", "Load Selected", nullptr));

  pbtableqdcloadall = new QPushButton(groupboxqdc2);
  horizontalgbqdc2->addWidget(pbtableqdcloadall);
  pbtableqdcloadall->setObjectName(QString::fromUtf8("pbtableqdcloadall"));
  pbtableqdcloadall->setEnabled(true);
  pbtableqdcloadall->setText(QApplication::translate("BasicSettings", "Load All", nullptr));
  
  pbtableqdcapplyselected = new QPushButton(groupboxqdc2);
  horizontalgbqdc2->addWidget(pbtableqdcapplyselected);
  pbtableqdcapplyselected->setObjectName(QString::fromUtf8("pbtableqdcapplyselected"));
  pbtableqdcapplyselected->setEnabled(true);
  pbtableqdcapplyselected->setText(QApplication::translate("BasicSettings", "Apply Selected", nullptr));
  
  pbtableqdcapplyall = new QPushButton(groupboxqdc2);
  horizontalgbqdc2->addWidget(pbtableqdcapplyall);
  pbtableqdcapplyall->setObjectName(QString::fromUtf8("pbtableqdcapplyall"));
  pbtableqdcapplyall->setEnabled(true);
  pbtableqdcapplyall->setText(QApplication::translate("BasicSettings", "Apply All", nullptr));
  
}


void BasicSettings::on_rbtableqdcrow_clicked()
{
  tableqdc->setSelectionBehavior(QAbstractItemView::SelectRows);
  std::cout << tableqdc->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtableqdcitem_clicked()
{
  tableqdc->setSelectionBehavior(QAbstractItemView::SelectItems);
  std::cout << tableqdc->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtableqdccol_clicked()
{
  tableqdc->setSelectionBehavior(QAbstractItemView::SelectColumns);
  std::cout << tableqdc->selectionBehavior() << std::endl;
}


void BasicSettings::on_pbtableqdcloadselected_clicked()
{
  std::cout << "load selected" << std::endl;

  std::cout << tableqdc->selectionBehavior() << std::endl;

  QList<QTableWidgetItem*> items = tableqdc->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tableqdc->row(items.at(i));//获取选中的行
	  int column = tableqdc->column(items.at(i));
	  
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



void BasicSettings::on_pbtableqdcloadall_clicked()
{
  std::cout << "load all" << std::endl;
  
}

void BasicSettings::on_pbtableqdcapplyselected_clicked()
{
  std::cout << "apply select" << std::endl;

  
}

void BasicSettings::on_pbtableqdcapplyall_clicked()
{
  std::cout << "apply all" << std::endl;

  
}

void BasicSettings::on_pbtableqdccopy_clicked()
{
  QList<QTableWidgetItem*> items = tableqdc->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      // std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tableqdc->row(items.at(i));//获取选中的行
	  int column = tableqdc->column(items.at(i));
	  
	  // QTableWidgetItem *item = items.at(i);
	  // QString name = item->text();//获取内容
	  // std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }

  std::cout << "CH  " << cbqdccopych->currentIndex() << std::endl;
}


void BasicSettings::on_chkboxqdcenabled0_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 0);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxqdcenabled1_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 1);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxqdcenabled2_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 2);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxqdcenabled3_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 3);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxqdcenabled4_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 4);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxqdcenabled5_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 5);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxqdcenabled6_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 6);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxqdcenabled7_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tableqdc->item(i, 7);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}


// void BasicSettings::on_cbqdccopymod_currentIndexChanged(int index)
// {
//   cbqdccopych->clear();
//   std::cout << "index: " << index << std::endl;
//   for (int i = 0; i < TESTCH; ++i)
//     {
//       cbqdccopych->addItem(QString::asprintf("ch-%02d", i));
//     }
// }


// 
// BasicSettings_TabQDC.cc ends here
