#ifndef WMCD_GUI
#define WMCD_GUI

#include <vu>

/**
 * WMCDGUI
 */

class WMCDGUI : public CSingletonT<WMCDGUI>
{
public:
  void Setup(); // Eg. Call this after Window GUI created. WMCDGUI::Instance().Setup();
};

#endif // WMCD_GUI