#include <gtest/gtest.h>
#include <memory>
#include <SQLiteCpp/Database.h>

#include <open_greenery/database/light/ControlReader.hpp>
#include <open_greenery/database/light/StatusWriter.hpp>

namespace ogdfl = open_greenery::dataflow::relay;
namespace ogdbl = open_greenery::database::light;

class LightDaoTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        db = std::make_shared<SQLite::Database>(DATABASE_PATH, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        status_receiver_ = std::make_unique<ogdbl::StatusWriter>(db);
        control_provider_ = std::make_unique<ogdbl::ControlReader>(db);
    }

    void TearDown() override
    {
        std::remove(DATABASE_PATH);
    }

    static constexpr char DATABASE_PATH[]{"test.db3"};

    std::shared_ptr<SQLite::Database> db;
    std::unique_ptr<ogdfl::IStatusReceiver> status_receiver_;
    std::unique_ptr<ogdfl::IManualControlOptionalProvider> control_provider_;
};

TEST_F(LightDaoTest, TableCreated)
{
     EXPECT_TRUE(db->tableExists("light"));
}

TEST_F(LightDaoTest, DefaultConfigurationExists)
{
    const auto query_str = "SELECT count(*) AS count FROM light;";
    SQLite::Statement query(*db, query_str);
    query.executeStep();
    const int lines = query.getColumn("count");

    EXPECT_EQ(lines, 1);
}

TEST_F(LightDaoTest, DefaultConfigurationValues)
{
    const auto query_str = "SELECT * FROM light;";
    SQLite::Statement query(*db, query_str);
    query.executeStep();
    const int id = query.getColumn("id");
    const int user_id = query.getColumn("user_id");
    const char * mode = query.getColumn("mode");
    // TODO: time values
    const int is_enabled = query.getColumn("is_enabled");
    const char * control = query.getColumn("control");

    EXPECT_EQ(id, 1);
    EXPECT_EQ(user_id, 0); // Hardcoded user_is
    EXPECT_STREQ(mode, "auto");
    EXPECT_EQ(is_enabled, 0);
    EXPECT_STREQ(control, "handled");
}

TEST_F(LightDaoTest, StatusReceiver)
{
    std::map<bool, std::string> expected {
            {true, "1"},
            {false, "0"}
    };

    SQLite::Statement query(*db, "SELECT is_enabled AS status_in_db FROM light WHERE user_id = 0;");

    for (const auto & [is_enabled_input, expected_status_in_db]: expected)
    {
        status_receiver_->set(is_enabled_input);

        query.executeStep();
        const std::string actual_status_in_db = query.getColumn("status_in_db");
        query.reset();

        EXPECT_EQ(expected_status_in_db, actual_status_in_db);
    }
}

TEST_F(LightDaoTest, ControlProvider)
{
    std::map<std::string, std::optional<ogdfl::Control>> expected {
            {"enable", {ogdfl::Control::ENABLE}},
            {"disable", {ogdfl::Control::DISABLE}},
            {"toggle", {ogdfl::Control::TOGGLE}},
            {"handled", {}}
    };

    SQLite::Statement update_query(*db, "UPDATE light SET control = ? WHERE user_id = 0;");

    for (const auto & [value_in_db, expected_control]: expected)
    {
        update_query.bind(1, value_in_db);
        update_query.exec();

        const auto actual_control = control_provider_->get();
        EXPECT_EQ(actual_control, expected_control);

        update_query.reset();
    }
}
