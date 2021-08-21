#ifndef I_RECEIVER_HPP
#define I_RECEIVER_HPP

namespace open_greenery::dataflow::common
{

template<class T>
class IReceiver
{
public:
    virtual void set(T value) = 0;

    IReceiver() = default;
    virtual ~IReceiver() = default;
};

}

#endif //I_RECEIVER_HPP
