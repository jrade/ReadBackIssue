#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    // QQuickWindow::setGraphicsApi(QSGRendererInterface::Direct3D11);
    // QQuickWindow::setGraphicsApi(QSGRendererInterface::Vulkan);
    // QQuickWindow::setGraphicsApi(QSGRendererInterface::Direct3D12);

    QGuiApplication app(argc, argv);

    ::qmlRegisterType<MainWindow>("ReadBackIssue", 1, 0, "MainWindow");

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/Main.qml"));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
