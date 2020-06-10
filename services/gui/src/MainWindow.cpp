#include "open_greenery/gui/MainWindow.hpp"
#include "open_greenery/gui/page/MultipleCharts.hpp"
#include "open_greenery/gui/page/Configurations.hpp"
#include <QDebug>

namespace open_greenery::gui
{

MainWindow::MainWindow(std::function<void()> _cb_quit_app)
    : m_window_main(),
    m_cb_quit_app(_cb_quit_app),
    m_topbar(static_cast<IAppQuit *>(this), static_cast<IPageControl *>(this)),
    m_page_layout(new QStackedLayout())
{
    auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/open_greenery.db3", SQLite::OPEN_READONLY);
    qDebug() << "SQLite database file " << QString::fromStdString(db->getFilename()) << " opened successfully";

    open_greenery::database::Table db_tables [] {{db, "A0"},
                                                 {db, "A1"},
                                                 {db, "A2"},
                                                 {db, "A3"}};
    std::vector<open_greenery::gui::page::DateValueChart> soil_moisture_charts;
    for (const auto & db_table : db_tables)
    {
        if (db_table.valid())
        {
            soil_moisture_charts.emplace_back(
                    open_greenery::database::SensorReader(db_table),
                    QString::fromStdString(db_table.name+" soil moisture"));
        }
    }

    const QDateTime date_from = QDateTime::currentDateTime().addMonths(-1);// Month ago
    const QDateTime date_to = QDateTime::currentDateTime();
    for (auto & chart : soil_moisture_charts)
    {
        chart.update(date_from, date_to);
    }
    m_page_soil = std::make_unique<open_greenery::gui::page::MultipleCharts>(std::move(soil_moisture_charts));
    m_page_config = std::make_unique<open_greenery::gui::page::Configurations>();

    auto main_layout = new QVBoxLayout();
    main_layout->addLayout(m_topbar.layout());
    main_layout->addLayout(m_page_layout);
//    main_layout->addWidget(m_page_config->widget());

    m_page_layout->addWidget(m_page_soil->widget());
    m_page_layout->addWidget(m_page_config->widget());

    auto main_wdg = new QWidget();
    main_wdg->setLayout(main_layout);
    m_window_main.setWindowTitle("OpenGreenery");
    m_window_main.setCentralWidget(main_wdg);
    m_window_main.show();
}

void MainWindow::quit()
{
    m_cb_quit_app();
}

void MainWindow::showConfigurations()
{
    m_page_layout->setCurrentIndex(1);
}

void MainWindow::showMultipleCharts()
{
    m_page_layout->setCurrentIndex(0);
}

}
