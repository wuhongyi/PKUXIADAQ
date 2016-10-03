#ifndef POPUP_
#define POPUP_

#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGLabel.h"
#include "TGButton.h"

using namespace std;

class Popup: public TGTransientFrame
{
public:
  Popup(const TGWindow * p, const TGWindow * main, char *message);
  virtual ~Popup();

private:
  Pixel_t color;
  
  Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

  enum Commands
    {
      OK
    };
};



#endif
