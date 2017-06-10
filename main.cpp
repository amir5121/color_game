#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Material");

    Controller controller;

    QQmlApplicationEngine engine;

    auto rootContext = engine.rootContext();
    rootContext->setContextProperty("Controller", &controller);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
