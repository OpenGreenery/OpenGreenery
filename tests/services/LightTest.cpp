#include <gtest/gtest.h>
#include <memory>
#include <SQLiteCpp/Database.h>

namespace ogdfl = open_greenery::dataflow::light;
namespace ogdbl = open_greenery::database::light;

class LightTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        db = std::make_shared<SQLite::Database>(DATABASE_PATH, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    };

    void TearDown() override
    {
        std::remove(DATABASE_PATH);
    }

    static constexpr char DATABASE_PATH[]{"test.db3"};

    std::shared_ptr<SQLite::Database> db;

}
