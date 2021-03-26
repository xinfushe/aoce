#pragma once

#include <memory>

#include "../VkExtraExport.hpp"
#include "VkConvertImageLayer.hpp"
#include "VkLinearFilterLayer.hpp"
#include "VkSeparableLinearLayer.hpp"
#include "aoce_vulkan/layer/VkLayer.hpp"
#include "aoce_vulkan/layer/VkResizeLayer.hpp"

namespace aoce {
namespace vulkan {
namespace layer {

// 导向滤波求值 Guided filter
// 论文地址http://kaiminghe.com/publications/pami12guidedfilter.pdf
// https://www.cnblogs.com/zhouxin/p/10203954.html
class VkToMatLayer : public VkLayer {
   public:
    VkToMatLayer();
    virtual ~VkToMatLayer();

   protected:
    virtual void onInitGraph() override;
};

class VkGuidedSolveLayer : public VkLayer, public ITLayer<float> {
    AOCE_LAYER_QUERYINTERFACE(VkGuidedSolveLayer)
    AOCE_VULKAN_PARAMETUPDATE()
   public:
    VkGuidedSolveLayer();
    virtual ~VkGuidedSolveLayer();

   protected:
    virtual void onInitGraph() override;
};

class VkGuidedLayer : public VkLayer, public ITLayer<GuidedParamet> {
    AOCE_LAYER_QUERYINTERFACE(VkGuidedLayer)
   private:
    /* data */
    std::unique_ptr<VkConvertImageLayer> convertLayer;
    std::unique_ptr<VkResizeLayer> resizeLayer;
    std::unique_ptr<VkToMatLayer> toMatLayer;
    std::unique_ptr<VkBoxBlurSLayer> box1Layer;
    std::unique_ptr<VkBoxBlurSLayer> box2Layer;
    std::unique_ptr<VkBoxBlurSLayer> box3Layer;
    std::unique_ptr<VkBoxBlurSLayer> box4Layer;
    std::unique_ptr<VkGuidedSolveLayer> guidedSlayerLayer;
    std::unique_ptr<VkBoxBlurSLayer> box5Layer;    
    std::unique_ptr<VkResizeLayer> resize1Layer;

    const int32_t zoom = 8;

   public:
    VkGuidedLayer(/* args */);
    virtual ~VkGuidedLayer();

   protected:
    virtual void onUpdateParamet() override;
    virtual void onInitGraph() override;
    virtual void onInitNode() override;
    virtual void onInitLayer() override;
};


}  // namespace layer
}  // namespace vulkan
}  // namespace aoce