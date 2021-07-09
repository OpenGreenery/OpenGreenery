#ifndef I_PAGE_HPP
#define I_PAGE_HPP

#include <QWidget>

namespace open_greenery::gui
{

class IPage
{
public:
    IPage(const IPage &) = delete;
    IPage & operator=(IPage &) = delete;
    virtual ~IPage() = default;

    virtual QWidget * widget() = 0;

protected:
    IPage() = default;
};

}

#endif //I_PAGE_HPP
