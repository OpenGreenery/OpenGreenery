#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QSplineSeries>
#include <QDebug>
#include <QString>
#include <algorithm>
#include <cstdint>
#include <QtWidgets/QGridLayout>
#include "open_greenery/database/SensorReader.hpp"

QT_CHARTS_USE_NAMESPACE

namespace ogdb = open_greenery::database;

class SoilMoistureChart
{
public:
    SoilMoistureChart(QString table_name, ogdb::SensorReader reader)
    :m_table_name(table_name),
    m_db_reader(reader)
    {}

    QChartView * getChart(const QDateTime from, const QDateTime to = QDateTime::currentDateTime())
    {
        auto series = new QSplineSeries();
        series->setName("spline");

        const auto all_moisture_data = m_db_reader.read(from, to);
        std::uint32_t cnt {};
        std::uint16_t min, max;
        min = max = all_moisture_data.front().data;
        for (const auto moisture_data : all_moisture_data)
        {
            const auto & value = moisture_data.data;
            if (value > max)
            {
                max = value;
            }
            if (value < min)
            {
                min = value;
            }
            series->append(cnt++, value);
        }

        auto chart = new QChart();
        chart->legend()->hide();
        chart->addSeries(series);
        chart->setTitle(m_table_name+" plant soil moisture");
        chart->createDefaultAxes();

        chart->axes(Qt::Vertical).first()->setRange(min-10, max+10);

        auto chart_view = new QChartView(chart);
        chart_view->setRenderHint(QPainter::Antialiasing);

        return chart_view;
    }

private:
    QString m_table_name;
    ogdb::SensorReader m_db_reader;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget main_wdg;

    auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/soil_moisture.db3", SQLite::OPEN_READONLY);
    qDebug() << "SQLite database file " << QString::fromStdString(db->getFilename()) << " opened successfully";

    const QString table_names [] {"A0", "A1", "A2", "A3"};
    std::vector<SoilMoistureChart> charts;
    for (const auto table_name : table_names)
    {
        charts.push_back({table_name, {{db, table_name.toStdString()}}});
    }

    auto layout = new QGridLayout();
    layout->setMargin(1);
    layout->setSpacing(1);

    {
        const QDateTime data_from {{2020, 5, 12},{0, 0}};
        constexpr std::array<std::pair<std::uint8_t, std::uint8_t>, 4> grid_positions {{
            {0, 0}, {0, 1}, {1, 0}, {1, 1}
        }};
        auto chart_it = charts.begin();
        auto pos_it = grid_positions.begin();
        for (;chart_it != charts.end() && pos_it != grid_positions.end(); chart_it++, pos_it++)
        {
            QChartView * widget = chart_it->getChart(data_from);
            widget->show();
            layout->addWidget(widget, pos_it->first, pos_it->second);
        }
    }


    main_wdg.setLayout(layout);
    main_wdg.show();
    return a.exec();
}