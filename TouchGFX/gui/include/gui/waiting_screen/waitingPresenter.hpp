#ifndef WAITINGPRESENTER_HPP
#define WAITINGPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class waitingView;

class waitingPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    waitingPresenter(waitingView& v);

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

    virtual ~waitingPresenter() {};

private:
    waitingPresenter();

    waitingView& view;
};

#endif // WAITINGPRESENTER_HPP
