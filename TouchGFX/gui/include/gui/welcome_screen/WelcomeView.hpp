#ifndef WELCOMEVIEW_HPP
#define WELCOMEVIEW_HPP

#include <gui_generated/welcome_screen/WelcomeViewBase.hpp>
#include <gui/welcome_screen/WelcomePresenter.hpp>

class WelcomeView : public WelcomeViewBase
{
public:
    WelcomeView();
    virtual ~WelcomeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // WELCOMEVIEW_HPP
