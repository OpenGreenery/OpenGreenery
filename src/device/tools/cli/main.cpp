#include <cassert>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>

#include <QTime>
#include <spdlog/cfg/env.h>

#include <open_greenery/rpc/relay/Client.hpp>

namespace open_greenery::cli
{

class MenuEntity
{
public:
    using Position = int;
    static constexpr Position RETURN_POS = 0;
    enum IsRoot : bool
    {
        NOT_ROOT = false, ROOT = true
    };

    MenuEntity(std::string text, std::map<Position, MenuEntity> children, IsRoot is_root = NOT_ROOT)
            : m_text(std::move(text)),
              m_is_root(is_root),
              m_children(std::move(children)),
              m_scenario(nullptr)
    {
        assert(!m_children.empty() && "Menu entity children map is empty");
    }

    MenuEntity(std::string text, std::function<void()> scenario)
            : m_text(std::move(text)),
              m_is_root(NOT_ROOT),  // Scenario entity can't be root
              m_children(),
              m_scenario(std::move(scenario))
    {
        assert(m_scenario != nullptr && "Menu entity scenario is empty");
    }

    void show() const
    {
        std::cout << std::endl;
        if (!m_children.empty())  // Menu entity
        {
            while (true)
            {
                std::cout << m_text << std::endl << "---------" << std::endl;
                // Print all child entities
                for (auto &&[pos, entity] : m_children)
                {
                    assert((pos != RETURN_POS) && "ERROR: Entity position reserved for return");
                    std::cout << int(pos) << ": " << entity.text() << std::endl;
                }
                std::cout << int(RETURN_POS) << ": Return" << std::endl << ">>> ";

                // Select entity by position
                Position user_selection{};
                std::cin >> user_selection;

                if (user_selection == RETURN_POS)
                {
                    return;   // Return to parent
                }

                auto entity = m_children.find(user_selection);
                if (entity == m_children.end())
                {
                    std::cerr << "Invalid entity position!" << std::endl;
                    continue;
                }
                entity->second.show();

                if (m_is_root == NOT_ROOT)
                {
                    return;  // Return to parent
                }
            }
        } else  // Scenario entity
        {
            m_scenario();
            std::cout << m_text << " --> done" << std::endl;
        }
    }

    std::string text() const
    {
        return m_text;
    }

private:
    const std::string m_text;
    const bool m_is_root;

    std::map<Position, MenuEntity> m_children;
    std::function<void()> m_scenario;
};

}

int main()
{
    spdlog::cfg::load_env_levels();

    signal(SIGTERM, exit);
    signal(SIGINT, exit);

    // Light
    constexpr char LIGHT_RPC_HOST[]{"localhost:8090"};
    auto light_rpc_client = std::make_unique<open_greenery::rpc::relay::Client>(LIGHT_RPC_HOST);
    // Ventilation
    constexpr char VENTILATION_RPC_HOST[]{"localhost:8091"};
    auto ventilation_rpc_client = std::make_unique<open_greenery::rpc::relay::Client>(VENTILATION_RPC_HOST);

    using open_greenery::cli::MenuEntity;
    // Light: Set mode
    MenuEntity light_mode_set(
            "Set mode",
            {
                    {1, {"Manual mode",
                                [&light_rpc_client] {
                                    light_rpc_client->set(open_greenery::dataflow::relay::Mode::MANUAL);
                                }
                        }},
                    {2, {"Auto mode",
                                [&light_rpc_client] {
                                    light_rpc_client->set(open_greenery::dataflow::relay::Mode::AUTO);
                                }
                        }}
            }
    );

    // Light: Manual control
    MenuEntity light_manual_control(
            "Manual control",
            {
                    {1, {"Enable",
                                [&light_rpc_client] {
                                    light_rpc_client->set(open_greenery::dataflow::relay::Control::ENABLE);
                                }
                        }},
                    {2, {"Disable",
                                [&light_rpc_client] {
                                    light_rpc_client->set(open_greenery::dataflow::relay::Control::DISABLE);
                                }
                        }},
                    {3, {"Toggle",
                                [&light_rpc_client] {
                                    light_rpc_client->set(open_greenery::dataflow::relay::Control::TOGGLE);
                                }
                        }}
            });

    // Light: Configure Auto mode
    MenuEntity light_config (
            "Configure Auto mode",
            [&light_rpc_client]
            {
                constexpr char FORMAT [] {"hh:mm"};

                auto RequestTime = [FORMAT](std::string time_name)
                {
                    std::cout << "Enter " << time_name << " in format " << FORMAT << std::endl;
                    std::string time_str;
                    std::cin >> time_str;
                    return QTime::fromString(QString::fromStdString(time_str), FORMAT);
                };

                const auto start = RequestTime("day start");
                const auto end = RequestTime("day end");

                light_rpc_client->set({start, end});
            });

    MenuEntity light_menu(
            "Light",
            {{1, light_mode_set},
             {2, light_manual_control},
             {3, light_config}}
    );

    // Ventilation: Set mode
    MenuEntity ventilation_mode_set(
            "Set mode",
            {
                    {1, {"Manual mode",
                                [&ventilation_rpc_client] {
                                    ventilation_rpc_client->set(open_greenery::dataflow::relay::Mode::MANUAL);
                                }
                        }},
                    {2, {"Auto mode",
                                [&ventilation_rpc_client] {
                                    ventilation_rpc_client->set(open_greenery::dataflow::relay::Mode::AUTO);
                                }
                        }}
            }
    );

    // Ventilation: Manual control
    MenuEntity ventilation_manual_control(
            "Manual control",
            {
                    {1, {"Enable",
                                [&ventilation_rpc_client] {
                                ventilation_rpc_client->set(open_greenery::dataflow::relay::Control::ENABLE);
                                }
                        }},
                    {2, {"Disable",
                                [&ventilation_rpc_client] {
                                ventilation_rpc_client->set(open_greenery::dataflow::relay::Control::DISABLE);
                                }
                        }},
                    {3, {"Toggle",
                                [&ventilation_rpc_client] {
                                ventilation_rpc_client->set(open_greenery::dataflow::relay::Control::TOGGLE);
                                }
                        }}
            });

    // Ventilation: Configure Auto mode
    MenuEntity ventilation_config (
            "Configure Auto mode",
            [&ventilation_rpc_client]
            {
                constexpr char FORMAT [] {"hh:mm"};

                auto RequestTime = [FORMAT](std::string time_name)
                {
                    std::cout << "Enter " << time_name << " in format " << FORMAT << std::endl;
                    std::string time_str;
                    std::cin >> time_str;
                    return QTime::fromString(QString::fromStdString(time_str), FORMAT);
                };

                const auto start = RequestTime("day start");
                const auto end = RequestTime("day end");

                ventilation_rpc_client->set({start, end});
            });

    MenuEntity ventilation_menu(
            "Ventilation",
            {{1, ventilation_mode_set},
             {2, ventilation_manual_control},
             {3, ventilation_config}}
    );

    MenuEntity main_menu(
            "OpenGreenery CLI",
            {
                {1, light_menu},
                {2, ventilation_menu},
            },
            MenuEntity::ROOT
    );
    main_menu.show();

    return 0;
}
