#include <print>
#include <rhi/qrhi.h>

#include "MainWindow.h"

MainWindow::MainWindow()
{
    QObject::connect(this, &QQuickWindow::afterRendering, this, [this] { onAfterRendering_(); }, Qt::DirectConnection);
}

void MainWindow::onAfterRendering_()
{
    QRhiCommandBuffer* cb = swapChain()->currentFrameCommandBuffer();
    QRhiResourceUpdateBatch* rub = rhi()->nextResourceUpdateBatch();

    if (!tex_) {
        std::println("Qt version: {}", qVersion());
        std::println("RHI backend: {}\n", rhi()->backendName());

        tex_ = rhi()->newTexture(QRhiTexture::RGBA8, { 1, 1 }, 1, QRhiTexture::UsedAsTransferSource);
        tex_->create();
        rhi()->addCleanupCallback([this](QRhi*) { delete tex_; });
    }

    int i = readBackIndex_++;
    QRhiReadbackResult* rr = new QRhiReadbackResult;
    rr->completed = [i, rr] {
        std::println("Readback {} complete", i);
        delete rr;
        };
    rub->readBackTexture(tex_, rr);

    cb->resourceUpdate(rub);

    std::println("Readback {} started", i);

    update();
}
