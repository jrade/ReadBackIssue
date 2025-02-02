#pragma once

#include <QQuickWindow>

class MainWindow : public QQuickWindow {
public:
    MainWindow();
    ~MainWindow() = default;

private:
    void onAfterRendering_();

private:
    QRhiTexture* tex_ = nullptr;
    int readBackIndex_ = 0;
};
