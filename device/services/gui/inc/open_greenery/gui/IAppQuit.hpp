#ifndef I_APP_QUIT_HPP
#define I_APP_QUIT_HPP

namespace open_greenery::gui
{

class IAppQuit : public QObject
{
public slots:
    virtual void quit() = 0;
};

}

#endif //I_APP_QUIT_HPP
