// About.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 4月 23 13:24:45 2022 (+0800)
// Last-Updated: 六 4月 23 15:45:24 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 4
// URL: http://wuhongyi.cn 

#ifndef _ABOUT_H_
#define _ABOUT_H_

#include <QVariant>
#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private:

  QVBoxLayout *verticallayout;
  QTextEdit *textedit;
  QLabel *label;

};

#endif /* _ABOUT_H_ */
// 
// About.hh ends here
