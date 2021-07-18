#include "StatusReceiver.hpp"

namespace open_greenery::tests::services::light::mock
{

void StatusReceiver::set(bool _is_enabled)
{
    m_received_statuses.push_back(_is_enabled);
}

const std::vector<bool> & StatusReceiver::getReceivedStatuses() const
{
    return m_received_statuses;
}

}
