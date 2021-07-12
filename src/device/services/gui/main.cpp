#include <QApplication>
#include "open_greenery/gui/MainWindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app (argc, argv);
    open_greenery::gui::MainWindow m (&app.quit);
    return app.exec();
}
