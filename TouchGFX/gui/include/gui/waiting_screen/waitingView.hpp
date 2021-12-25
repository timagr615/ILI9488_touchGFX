#ifndef WAITINGVIEW_HPP
#define WAITINGVIEW_HPP

#include <gui_generated/waiting_screen/waitingViewBase.hpp>
#include <gui/waiting_screen/waitingPresenter.hpp>

class waitingView : public waitingViewBase
{
public:
    waitingView();
    virtual ~waitingView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // WAITINGVIEW_HPP
