#include <QDateTimeAxis>
#include <QValueAxis>
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
    SoilMoistureChart(ogdb::SensorReader reader)
    :m_db_reader(reader)
    {}

    QChartView * getChart(const QDateTime from, const QDateTime to = QDateTime::currentDateTime())
    {
        auto series = new QSplineSeries();

        for (const auto moisture_data : m_db_reader.read(from, to))
        {
            series->append(moisture_data.timestamp.toMSecsSinceEpoch(), moisture_data.data);
        }

        auto chart = new QChart();
        chart->legend()->hide();
        chart->addSeries(series);
        chart->setTitle(QString::fromStdString(m_db_reader.table().name)+" soil moisture");

        QDateTimeAxis * axisX = new QDateTimeAxis;
        axisX->setFormat("dd.MM");
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis * axisY = new QValueAxis;
        axisY->setLabelFormat("%i");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        auto chart_view = new QChartView(chart);
        chart_view->setRenderHint(QPainter::Antialiasing);

        return chart_view;
    }

private:
    ogdb::SensorReader m_db_reader;
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto db = std::make_shared<SQLite::Database>("/home/pi/og/db/soil_moisture.db3", SQLite::OPEN_READONLY);
    qDebug() << "SQLite database file " << QString::fromStdString(db->getFilename()) << " opened successfully";

    const std::string table_names [] {"A0", "A1", "A2", "A3"};
    std::vector<SoilMoistureChart> charts;
    for (const auto table_name : table_names)
    {
        ogdb::Table table {db, table_name};
        if (table.valid())
        {
            charts.push_back({{{db, table_name}}});
        }
    }

    auto layout = new QGridLayout();

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
            layout->addWidget(widget, pos_it->first, pos_it->second);
        }
    }

    auto main_wdg = new QWidget();
    main_wdg->setLayout(layout);

    QMainWindow main_wnd;
    main_wnd.setWindowTitle("OpenGreenery");
    main_wnd.setCentralWidget(main_wdg);
    main_wnd.showMaximized();

    return a.exec();
}