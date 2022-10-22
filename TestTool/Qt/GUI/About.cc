// About.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 4月 23 13:24:55 2022 (+0800)
// Last-Updated: 六 4月 23 15:45:24 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 8
// URL: http://wuhongyi.cn 

#include "About.hh"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

About::About(QWidget *parent) :
    QDialog(parent)
{

  if (this->objectName().isEmpty())
    this->setObjectName(QString::fromUtf8("About"));

  this->setWindowTitle(QApplication::translate("About", "About GDDAQ", nullptr));
  this->resize(400, 275);
  
  
  verticallayout = new QVBoxLayout(this);
  verticallayout->setObjectName(QString::fromUtf8("verticalLayout"));
  verticallayout->setStretch(0, 1);


  
  textedit = new QTextEdit(this);
  textedit->setObjectName(QString::fromUtf8("textEdit_2"));
  textedit->setEnabled(false);
  textedit->setFrameShadow(QFrame::Sunken);
  textedit->setLineWidth(1);
  textedit->setReadOnly(true);
  verticallayout->addWidget(textedit);

  textedit->setHtml(QApplication::translate("About", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
					      "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
					      "p, li { white-space: pre-wrap; }\n"
					      "</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
					      "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8.25pt;\"><br /></p>\n"
					      "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8.25pt;\"><br /></p>\n"
					      "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8.25pt;\"><br /></p>\n"
					      "<p align=\"ce"
					      "nter\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:14pt; color:#258bea;\">GDDAQ</span><span style=\" font-family:'MS Shell Dlg 2'; font-size:14pt; color:#0d0dea;\"> </span></p>\n"
					      "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:8.25pt;\"><br /></p>\n"
					      "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:10pt;\">GDDAQ STD release 2.0.0 build 04/2022</span></p>\n"
					      "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2'; font-size:10pt;\"><br /></p>\n"
					      "<p"
					      " align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'MS Shell Dlg 2';\">Content: Hongyi Wu(wuhongyi@qq.com)</span></p>\n"
					      "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'MS Shell Dlg 2';\"><br /></p>\n"
					      "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://www.caensys.com\"><span style=\" font-family:'MS Shell Dlg 2'; font-size:8pt; text-decoration: underline; color:#258bea;\">wuhongyi.cn</span></a></p></body></html>", nullptr));



  
  
  label = new QLabel(this);
  label->setObjectName(QString::fromUtf8("label"));
  label->setEnabled(false);
  QFont font;
  font.setPointSize(9);
  font.setBold(false);
  font.setWeight(50);
  label->setFont(font);
  label->setAlignment(Qt::AlignCenter);
  verticallayout->addWidget(label);

  label->setText("Cite: H.Y. Wu et al., NIM A 975(2020)164200");//QString()



  
  QMetaObject::connectSlotsByName(this);
}

About::~About()
{


}

// 
// About.cc ends here
