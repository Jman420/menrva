// Author : Jman420

#include "menrva_engine_interface.h"
#include "menrva_module_interface.h"
#include "menrva_command_map.h"

const effect_descriptor_t MenrvaEngineInterface::effectDescriptor = {
    // UUID of to the OpenSL ES interface implemented by this effect (EFFECT_TYPE_NULL)
    .type = { 0xec7178ec, 0xe5e1, 0x4432, 0xa3f4, { 0x46, 0x57, 0xe6, 0x79, 0x52, 0x10 } },
    // UUID for this particular implementation (http://www.itu.int/ITU-T/asn1/uuid.html)
    .uuid = { 0xde1b7837, 0x24fb, 0x4091, 0x852b, { 0x27, 0x88, 0x1a, 0xfb, 0x9e, 0x5e } },
    // Version of the effect control API implemented
    .apiVersion = EFFECT_CONTROL_API_VERSION,
    // effect engine capabilities/requirements flags (see below)
    .flags = EFFECT_FLAG_TYPE_INSERT | EFFECT_FLAG_INSERT_FIRST,
    // CPU load indication
    .cpuLoad = 10,
    // Data Memory usage
    .memoryUsage = 1,
    // human readable effect name
    .name = "Menrva",
    // human readable effect implementor name
    .implementor = "Jman420"
};

int MenrvaEngineInterface::Process(effect_handle_t self, audio_buffer_t *in, audio_buffer_t *out)
{
    struct menrva_module_context *context = (menrva_module_context*)self;

    if (context->moduleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING) {
        return -ENODATA;
    }
    if (context->moduleStatus != MenrvaModuleStatus::MENRVA_MODULE_READY) {
        return 0;
    }

    int result = context->effectsEngine->Process(in, out);
    return result;
}

int MenrvaEngineInterface::Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize,
                                   void *pCmdData, uint32_t *replySize, void *pReplyData)
{
    struct menrva_module_context *context = (menrva_module_context*)self;

    if (context->moduleStatus == MenrvaModuleStatus::MENRVA_MODULE_RELEASING ||
        context->moduleStatus == MenrvaModuleStatus::MENRVA_MODULE_INITIALIZING) {

        return -EINVAL;
    }

    int result = MenrvaCommandMap::Command(context, cmdCode, cmdSize, pCmdData, replySize,
                                           pReplyData);
    return result;
}

int MenrvaEngineInterface::GetDescriptorFromModule(effect_handle_t self,
                                                   effect_descriptor_t *pDescriptor)
{
    menrva_module_context* module = (menrva_module_context*)self;
    if (module == NULL || pDescriptor == NULL) {
        return -EINVAL;
    }

    *pDescriptor = effectDescriptor;
    return 0;
}

MenrvaEngineInterface::MenrvaEngineInterface() {}
