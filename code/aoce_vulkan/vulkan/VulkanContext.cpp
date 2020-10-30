#include "VulkanContext.hpp"

#include "VulkanManager.hpp"
namespace aoce {
namespace vulkan {
VulkanContext::VulkanContext(/* args */) {}

VulkanContext::~VulkanContext() {
    if (pipelineCache) {
        vkDestroyPipelineCache(device, pipelineCache, nullptr);
        pipelineCache = VK_NULL_HANDLE;
    }
    if (computerCmd) {
        vkFreeCommandBuffers(device, cmdPool, 1, &computerCmd);
        computerCmd = VK_NULL_HANDLE;
    }
    if (cmdPool) {
        vkDestroyCommandPool(device, cmdPool, nullptr);
        cmdPool = VK_NULL_HANDLE;
    }
}

void VulkanContext::initContext() {
    // 得到全局设置
    this->instace = VulkanManager::Get().instace;
    this->physicalDevice = VulkanManager::Get().physicalDevice;
    this->device = VulkanManager::Get().device;
    this->computeQueue = VulkanManager::Get().computeQueue;
    int computeIndex = VulkanManager::Get().computeIndex;
    // 得到当前graph需要的VkCommandBuffer
    VkPipelineCacheCreateInfo pipelineCacheInfo = {};
    pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheInfo, nullptr,
                                          &pipelineCache));
    // context和呈现渲染相应command分开
    VkCommandPoolCreateInfo cmdPoolInfo = {};
    cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmdPoolInfo.queueFamilyIndex = computeIndex;
    cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VK_CHECK_RESULT(
        vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &cmdPool));
    VkCommandBufferAllocateInfo cmdBufInfo = {};
    cmdBufInfo.commandPool = cmdPool;
    cmdBufInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufInfo.commandBufferCount = 1;
    VK_CHECK_RESULT(
        vkAllocateCommandBuffers(device, &cmdBufInfo, &computerCmd));
}

bool VulkanContext::checkFormat(VkFormat format, VkFormatFeatureFlags feature,
                                bool bLine) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, format,
                                        &props);
    if (bLine) {
        return (props.linearTilingFeatures & feature) == feature;
    } else {
        return (props.optimalTilingFeatures & feature) == feature;
    }
}

void VulkanContext::bufferToImage(VkCommandBuffer cmd,
                                  const VulkanBuffer* buffer,
                                  const VulkanTexture* texture) {
    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    // copyRegion.bufferRowLength = texture->width *
    // getByteSize(texture->format); copyRegion.bufferImageHeight =
    // texture->height;
    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageExtent.width = texture->width;
    copyRegion.imageExtent.height = texture->height;
    copyRegion.imageExtent.depth = 1;

    // buffer to image
    vkCmdCopyBufferToImage(cmd, buffer->buffer, texture->image,
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                           &copyRegion);
}

void VulkanContext::imageToBuffer(VkCommandBuffer cmd,
                                  const VulkanTexture* texture,
                                  const VulkanBuffer* buffer) {
    VkBufferImageCopy copyRegion = {};
    copyRegion.bufferOffset = 0;
    // copyRegion.bufferRowLength = texture->width *
    // getByteSize(texture->format); copyRegion.bufferImageHeight =
    // texture->height;
    copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copyRegion.imageSubresource.mipLevel = 0;
    copyRegion.imageSubresource.baseArrayLayer = 0;
    copyRegion.imageSubresource.layerCount = 1;
    copyRegion.imageExtent.width = texture->width;
    copyRegion.imageExtent.height = texture->height;
    copyRegion.imageExtent.depth = 1;
    vkCmdCopyImageToBuffer(cmd, texture->image,
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer->buffer,
                           1, &copyRegion);
}

void VulkanContext::blitFillImage(VkCommandBuffer cmd, const VulkanTexture* src,
                                  const VulkanTexture* dest) {
    blitFillImage(cmd, src, dest->image, dest->width, dest->height,
                  dest->layout);
}

void VulkanContext::blitFillImage(VkCommandBuffer cmd, const VulkanTexture* src,
                                  VkImage dest, int32_t destWidth,
                                  int32_t destHeight,
                                  VkImageLayout destLayout) {
    VulkanManager::blitFillImage(cmd, src, dest, destWidth, destHeight,
                                 destLayout);
}

}  // namespace vulkan
}  // namespace aoce