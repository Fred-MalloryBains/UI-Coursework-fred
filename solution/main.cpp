// COMP2811 Coursework 2: application entry point
// fred
#include <QtWidgets>
#include "window.hpp"

int main(int argc, char *argv[])
{

  QApplication app(argc, argv);

  WaterWindow window;
  window.show();
  app.setStyleSheet("QToolTip {"
                    "color: white; background-color: grey; border: 1px solid black;"
                    "}");
  return app.exec();
}
