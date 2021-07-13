#include <QtWidgets/QTimeEdit>
#include "EmptyCurrentTimeProvider.hpp"

namespace open_greenery::tests::services::light::mock
{

QTime EmptyCurrentTimeProvider::get()
{
    return {};
}

}
