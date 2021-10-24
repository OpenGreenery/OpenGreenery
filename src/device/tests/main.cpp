#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    spdlog::set_level(spdlog::level::warn);
    return RUN_ALL_TESTS();
}
