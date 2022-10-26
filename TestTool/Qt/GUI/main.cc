// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 五 12月 17 15:15:22 2021 (+0800)
// Last-Updated: 二 10月 25 14:36:08 2022 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 19
// URL: http://wuhongyi.cn 

#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTextCodec>
#include "Login.hh"
#include "MainWindow.hh"
#include <iostream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(gddaq);

  QTextCodec *codec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForLocale(codec); //解决汉字乱码问题
  
  QApplication app(argc, argv);

  Login *login = new Login;
  if(login->exec() == QDialog::Accepted)
    {
      QCoreApplication::setOrganizationName("QtProject");
      QCoreApplication::setApplicationName("GDDAQ -- GUI");
      QCoreApplication::setApplicationVersion(QT_VERSION_STR);
      QCommandLineParser parser;
      parser.setApplicationDescription(QCoreApplication::applicationName());
      parser.addHelpOption();
      parser.addVersionOption();
      parser.addPositionalArgument("file", "The file to open");
      parser.process(app);

      MainWindow mainWin;
      // if (!parser.positionalArguments().isEmpty())
      //   mainWin.loadFile(parser.positionalArguments().first());
      mainWin.show();

      // QScreen *screen = app.primaryScreen();
      // QPixmap screenshot = screen->grabWindow(0);
      // screenshot.save("wuscreenshot.png");

      // QList<QScreen*> screens = app.screens();
      // QScreen *screen;
      // QPixmap screenshot;
      // for (int i = 0; i < screens.length(); ++i)
      //   {
      //     std::cout<<i<<std::endl;
      //     screen = screens.at(i);
      //     screenshot = screen->grabWindow(0);
      //     char tmp[1024];
      //     sprintf(tmp,"wuscreenshot%d.png",i);
      //     screenshot.save(tmp);
      //   }
  
      return app.exec();
    }
  else
    {
      return 0;
    }

}

// 
// main.cc ends here
