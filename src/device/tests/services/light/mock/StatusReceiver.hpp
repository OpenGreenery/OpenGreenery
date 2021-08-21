#ifndef STATUSRECEIVER_HPP
#define STATUSRECEIVER_HPP

#include <open_greenery/dataflow/light/Participants.hpp>
#include <vector>

namespace open_greenery::tests::services::light::mock
{

class StatusReceiver : public open_greenery::dataflow::light::IStatusReceiver
{
public:
    // IStatusReceiver
    void set(bool _is_enabled) override;

    const std::vector<bool> & getReceivedStatuses() const;
private:
    std::vector<bool> m_received_statuses;
};

}

#endif //STATUSRECEIVER_HPP
