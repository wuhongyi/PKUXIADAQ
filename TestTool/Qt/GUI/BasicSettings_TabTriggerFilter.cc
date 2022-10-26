// BasicSettings_TabTriggerFilter.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 10月 24 21:14:39 2022 (+0800)
// Last-Updated: 二 10月 25 12:26:48 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#include "BasicSettings.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void BasicSettings::TabTriggerFilter(int n)
{
  tabWidget->setTabText(tabWidget->indexOf(tab[n]), QApplication::translate("BasicSettings", "Trigger", nullptr));

  tabletrigger = new QTableWidget(tab[n]);
  verticalLayout[n]->addWidget(tabletrigger);
  // tabletrigger->setSortingEnabled(false);
  tabletrigger->setRowCount(MAXCHANNELNUM);
  tabletrigger->setColumnCount(3);
  tabletrigger->setSelectionBehavior(QAbstractItemView::SelectItems);
  //tabletrigger->setEditTriggers(QAbstractItemView::CurrentChanged);//全部可编辑
  
  QStringList headercol;
  headercol << "rise(us)" << "flat top(us)" << "Thre";
  tabletrigger->setHorizontalHeaderLabels(headercol);

  QStringList headerrow;
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    headerrow << QString::number(i);
  tabletrigger->setVerticalHeaderLabels(headerrow);
  
  for (int i = 0; i < MAXCHANNELNUM; ++i)
    for (int j = 0; j < 3; ++j)
      {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setTextAlignment(Qt::AlignCenter);
	item->setText(QString::asprintf("%f", i*MAXCHANNELNUM+j+0.1));
        tabletrigger->setItem(i, j, item);
      }

  for (int i = 0; i < MAXCHANNELNUM; ++i)
    {
      if(i < TESTCH[0])
	tabletrigger->setRowHidden(i, false);
      else
	tabletrigger->setRowHidden(i, true);
    }


  
  // {
  //   QTableWidgetItem *item = tabletrigger->item(0, 0);
  //   std::cout << item->text().toStdString().data()  << std::endl;
  // }


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  groupboxtrigger0 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxtrigger0);
  groupboxtrigger0->setObjectName(QString::fromUtf8("groupboxtrigger0"));

  horizontalgbtrigger0 = new QHBoxLayout(groupboxtrigger0);
  horizontalgbtrigger0->setSpacing(10);
  horizontalgbtrigger0->setContentsMargins(11, 11, 11, 11);
  horizontalgbtrigger0->setObjectName(QString::fromUtf8("horizontalgbtrigger0"));
  horizontalgbtrigger0->setContentsMargins(-1, 2, -1, 2);

  QLabel *label0 = new QLabel(groupboxtrigger0);
  horizontalgbtrigger0->addWidget(label0);
  label0->setObjectName(QString::fromUtf8("label0"));
  label0->setText(QApplication::translate("BasicSettings", "Enabled", nullptr));

  for (int i = 0; i < 3; ++i)
    {
      chkboxtriggerenabled[i] = new QCheckBox(groupboxtrigger0);
      horizontalgbtrigger0->addWidget(chkboxtriggerenabled[i]);
      chkboxtriggerenabled[i]->setObjectName(QString::fromUtf8("chkboxtriggerenabled")+QString::number(i));
      chkboxtriggerenabled[i]->setChecked(true);

      if(i == 0)
	chkboxtriggerenabled[i]->setText(QString::fromUtf8("Rise"));
      else if(i == 1)
	chkboxtriggerenabled[i]->setText(QString::fromUtf8("Top"));
      else if(i == 2)
	chkboxtriggerenabled[i]->setText(QString::fromUtf8("Thre"));
    }

  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  
  groupboxtrigger1 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxtrigger1);
  groupboxtrigger1->setObjectName(QString::fromUtf8("groupboxtrigger1"));

  horizontalgbtrigger1 = new QHBoxLayout(groupboxtrigger1);
  horizontalgbtrigger1->setSpacing(10);
  horizontalgbtrigger1->setContentsMargins(11, 11, 11, 11);
  horizontalgbtrigger1->setObjectName(QString::fromUtf8("horizontalgbtrigger1"));
  horizontalgbtrigger1->setContentsMargins(-1, 2, -1, 2);

  QLabel *label1 = new QLabel(groupboxtrigger1);
  horizontalgbtrigger1->addWidget(label1);
  label1->setObjectName(QString::fromUtf8("label1"));
  label1->setText(QApplication::translate("BasicSettings", "Select", nullptr));

  rbtabletriggerrow = new QRadioButton(groupboxtrigger1);
  horizontalgbtrigger1->addWidget(rbtabletriggerrow);
  rbtabletriggerrow->setText(QApplication::translate("BasicSettings", "Row", nullptr));
  rbtabletriggerrow->setObjectName(QString::fromUtf8("rbtabletriggerrow"));
  rbtabletriggerrow->setChecked(false);
  rbtabletriggercol = new QRadioButton(groupboxtrigger1);
  horizontalgbtrigger1->addWidget(rbtabletriggercol);
  rbtabletriggercol->setText(QApplication::translate("BasicSettings", "Column", nullptr));
  rbtabletriggercol->setObjectName(QString::fromUtf8("rbtabletriggercol"));
  rbtabletriggercol->setChecked(false);
  rbtabletriggeritem = new QRadioButton(groupboxtrigger1);
  horizontalgbtrigger1->addWidget(rbtabletriggeritem);
  rbtabletriggeritem->setText(QApplication::translate("BasicSettings", "Item", nullptr));
  rbtabletriggeritem->setObjectName(QString::fromUtf8("rbtabletriggeritem"));
  rbtabletriggeritem->setChecked(true);


  QLabel *label30 = new QLabel(groupboxtrigger1);
  horizontalgbtrigger1->addWidget(label30);
  label30->setObjectName(QString::fromUtf8("label30"));
  label30->setText(QApplication::translate("BasicSettings", "copy from ", nullptr));

 
  cbtriggercopych = new QComboBox(groupboxtrigger1);
  horizontalgbtrigger1->addWidget(cbtriggercopych);
  cbtriggercopych->setObjectName(QString::fromUtf8("cbtriggercopych"));
  cbtriggercopych->setEditable(false);
  for (int i = 0; i < TESTCH[0]; ++i)
    {
      cbtriggercopych->addItem(QString::asprintf("ch - %02d", i));
    }

  pbtabletriggercopy = new QPushButton(groupboxtrigger1);
  horizontalgbtrigger1->addWidget(pbtabletriggercopy);
  pbtabletriggercopy->setObjectName(QString::fromUtf8("pbtabletriggercopy"));
  pbtabletriggercopy->setEnabled(true);
  pbtabletriggercopy->setText(QApplication::translate("BasicSettings", "Copy", nullptr));
  
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  groupboxtrigger3 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxtrigger3);
  groupboxtrigger3->setObjectName(QString::fromUtf8("groupboxtrigger3"));

  horizontalgbtrigger3 = new QHBoxLayout(groupboxtrigger3);
  horizontalgbtrigger3->setSpacing(10);
  horizontalgbtrigger3->setContentsMargins(11, 11, 11, 11);
  horizontalgbtrigger3->setObjectName(QString::fromUtf8("horizontalgbtrigger3"));
  horizontalgbtrigger3->setContentsMargins(-1, 2, -1, 2);  
    

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  groupboxtrigger2 = new QGroupBox(tab[n]);
  verticalLayout[n]->addWidget(groupboxtrigger2);
  groupboxtrigger2->setObjectName(QString::fromUtf8("groupboxtrigger2"));

  horizontalgbtrigger2 = new QHBoxLayout(groupboxtrigger2);
  horizontalgbtrigger2->setSpacing(10);
  horizontalgbtrigger2->setContentsMargins(11, 11, 11, 11);
  horizontalgbtrigger2->setObjectName(QString::fromUtf8("horizontalgbtrigger2"));
  horizontalgbtrigger2->setContentsMargins(-1, 2, -1, 2);

  
  pbtabletriggerloadselected = new QPushButton(groupboxtrigger2);
  horizontalgbtrigger2->addWidget(pbtabletriggerloadselected);
  pbtabletriggerloadselected->setObjectName(QString::fromUtf8("pbtabletriggerloadselected"));
  pbtabletriggerloadselected->setEnabled(true);
  pbtabletriggerloadselected->setText(QApplication::translate("BasicSettings", "Load Selected", nullptr));

  pbtabletriggerloadall = new QPushButton(groupboxtrigger2);
  horizontalgbtrigger2->addWidget(pbtabletriggerloadall);
  pbtabletriggerloadall->setObjectName(QString::fromUtf8("pbtabletriggerloadall"));
  pbtabletriggerloadall->setEnabled(true);
  pbtabletriggerloadall->setText(QApplication::translate("BasicSettings", "Load All", nullptr));
  
  pbtabletriggerapplyselected = new QPushButton(groupboxtrigger2);
  horizontalgbtrigger2->addWidget(pbtabletriggerapplyselected);
  pbtabletriggerapplyselected->setObjectName(QString::fromUtf8("pbtabletriggerapplyselected"));
  pbtabletriggerapplyselected->setEnabled(true);
  pbtabletriggerapplyselected->setText(QApplication::translate("BasicSettings", "Apply Selected", nullptr));
  
  pbtabletriggerapplyall = new QPushButton(groupboxtrigger2);
  horizontalgbtrigger2->addWidget(pbtabletriggerapplyall);
  pbtabletriggerapplyall->setObjectName(QString::fromUtf8("pbtabletriggerapplyall"));
  pbtabletriggerapplyall->setEnabled(true);
  pbtabletriggerapplyall->setText(QApplication::translate("BasicSettings", "Apply All", nullptr));
  
}


void BasicSettings::on_rbtabletriggerrow_clicked()
{
  tabletrigger->setSelectionBehavior(QAbstractItemView::SelectRows);
  std::cout << tabletrigger->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtabletriggeritem_clicked()
{
  tabletrigger->setSelectionBehavior(QAbstractItemView::SelectItems);
  std::cout << tabletrigger->selectionBehavior() << std::endl;
}

void BasicSettings::on_rbtabletriggercol_clicked()
{
  tabletrigger->setSelectionBehavior(QAbstractItemView::SelectColumns);
  std::cout << tabletrigger->selectionBehavior() << std::endl;
}


void BasicSettings::on_pbtabletriggerloadselected_clicked()
{
  std::cout << "load selected" << std::endl;

  std::cout << tabletrigger->selectionBehavior() << std::endl;

  QList<QTableWidgetItem*> items = tabletrigger->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tabletrigger->row(items.at(i));//获取选中的行
	  int column = tabletrigger->column(items.at(i));
	  
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


void BasicSettings::on_pbtabletriggerloadall_clicked()
{
  std::cout << "load all" << std::endl;
  
}

void BasicSettings::on_pbtabletriggerapplyselected_clicked()
{
  std::cout << "apply select" << std::endl;

  
}

void BasicSettings::on_pbtabletriggerapplyall_clicked()
{
  std::cout << "apply all" << std::endl;

  
}

void BasicSettings::on_pbtabletriggercopy_clicked()
{
  QList<QTableWidgetItem*> items = tabletrigger->selectedItems();

  if(!items.empty())
    {
      int count = items.count();
      // std::cout << "count: " << count << std::endl;
      for(int i = 0; i < count; i++)
	{
	  int row = tabletrigger->row(items.at(i));//获取选中的行
	  int column = tabletrigger->column(items.at(i));
	  
	  // QTableWidgetItem *item = items.at(i);
	  // QString name = item->text();//获取内容
	  // std::cout << row << " " << column << " " << name.toStdString().data() << std::endl;
	}
    }
  else
    {
      std::cout << "not selected!" << std::endl;
    }

  std::cout << "CH  " << cbtriggercopych->currentIndex() << std::endl;
}


void BasicSettings::on_chkboxtriggerenabled0_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabletrigger->item(i, 0);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxtriggerenabled1_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabletrigger->item(i, 1);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}

void BasicSettings::on_chkboxtriggerenabled2_clicked(bool checked)
{
  for (int i = 0; i < TESTCH[toolBox->currentIndex()]; ++i)
    {
      QTableWidgetItem *item = tabletrigger->item(i, 2);
      if(checked)
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
      else
	item->setFlags(Qt::ItemIsEnabled);
    }
}



// 
// BasicSettings_TabTriggerFilter.cc ends here
