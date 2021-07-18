#ifndef OPTIONALPROVIDER_HPP
#define OPTIONALPROVIDER_HPP

#include <optional>

namespace open_greenery::tests::services::light::mock
{

template <class Interface, class Provide>
class OptionalProvider : public Interface
{
public:
    // Interface
    std::optional<Provide> get() override;

    void set(Provide data_to_provide);
    void reset();

private:
    std::optional<Provide> m_data_to_provide;
};

template<class Interface, class Provide>
std::optional<Provide> OptionalProvider<Interface, Provide>::get()
{
    auto rv = m_data_to_provide;
    m_data_to_provide.reset();
    return rv;
}

template<class Interface, class Provide>
void OptionalProvider<Interface, Provide>::set(Provide data_to_provide)
{
    m_data_to_provide = data_to_provide;
}

template<class Interface, class Provide>
void OptionalProvider<Interface, Provide>::reset()
{
    return m_data_to_provide.reset();
}


}

#endif //OPTIONALPROVIDER_HPP
