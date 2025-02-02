#include <print>
#include <rhi/qrhi.h>

#include "MainWindow.h"

MainWindow::MainWindow()
{
    QObject::connect(this, &QQuickWindow::afterRendering, this, [this] { onAfterRendering_(); }, Qt::DirectConnection);
}

void MainWindow::onAfterRendering_()
{
    const unsigned int texel = 0xc0540814;
    QRhiCommandBuffer* cb = swapChain()->currentFrameCommandBuffer();
    QRhiResourceUpdateBatch* rub = rhi()->nextResourceUpdateBatch();

    if (!tex_) {
        std::println("Qt version: {}", qVersion());
        std::println("RHI backend: {}\n", rhi()->backendName());

        tex_ = rhi()->newTexture(QRhiTexture::RGBA8, { 1, 1 }, 1, QRhiTexture::UsedAsTransferSource);
        tex_->create();
        rub->uploadTexture(tex_, { {0, 0,  {&texel, 4}} });
        rhi()->addCleanupCallback([this](QRhi*) { delete tex_; });
    }

    const int i = readBackIndex_++;
    QRhiReadbackResult* rr = new QRhiReadbackResult;
    rr->completed = [i, rr, texel] {
        std::println("Readback {} complete", i);
        assert(*reinterpret_cast<const unsigned int*>(rr->data.data()) == texel);
        delete rr;
        };
    rub->readBackTexture(tex_, rr);

    cb->resourceUpdate(rub);

    std::println("Readback {} started", i);

    update();
}
