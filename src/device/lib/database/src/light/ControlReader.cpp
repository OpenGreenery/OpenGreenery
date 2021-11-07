#include "open_greenery/database/light/ControlReader.hpp"
#include <QDebug>
#include <QTime>

namespace ogdfl = open_greenery::dataflow::relay;

namespace open_greenery::database::light
{

ControlReader::ControlReader(std::shared_ptr<SQLite::Database> _database)
    :LightDAO(std::move(_database))
{}

std::optional<ogdfl::Control> ControlReader::get()
{
    // TODO: Remove hardcoded user_id
    const std::string str_query = "SELECT control FROM light WHERE user_id = 0;";
    SQLite::Statement query(*m_database, str_query);
    query.executeStep();
    const std::string control = query.getColumn("control");

    if (control == "enable")
    {
        return {ogdfl::Control::ENABLE};
    }
    else if (control == "disable")
    {
        return {ogdfl::Control::DISABLE};
    }
    else if (control == "toggle")
    {
        return {ogdfl::Control::TOGGLE};
    }
    else
    {
        return {};
    }
}

}
