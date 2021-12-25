#ifndef WELCOMEPRESENTER_HPP
#define WELCOMEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class WelcomeView;

class WelcomePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    WelcomePresenter(WelcomeView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~WelcomePresenter() {};

private:
    WelcomePresenter();

    WelcomeView& view;
};

#endif // WELCOMEPRESENTER_HPP
