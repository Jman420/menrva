/*
 * Author : Jman420
 * Description : Provides interfaces for the Android Audio Framework to consume and interact with
 *   the Menrva Audio Effect Engine.
 */

#include "menrva_interface.h"
#include "menrva_command_map.h"

// Effect Engine Descriptor
const effect_descriptor_t effect_descriptor = {
        // UUID of to the OpenSL ES interface implemented by this effect (EFFECT_TYPE_DYNAMICS_PROCESSING)
        .type = { 0x7261676f, 0x6d75, 0x7369, 0x6364, { 0x28, 0xe2, 0xfd, 0x3a, 0xc3, 0x9e } },
        // UUID for this particular implementation (see http://www.itu.int/ITU-T/asn1/uuid.html for UUID)
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

// Effect Engine Control Interface Functions & Struct
int Process(effect_handle_t self, audio_buffer_t *in, audio_buffer_t *out)
{
    struct menrva_engine_context *menrvaEngineModule = (menrva_engine_context*)self;
    int result = menrvaEngineModule->effectsEngine->process(in, out);
    return result;
}
int Command(effect_handle_t self, uint32_t cmdCode, uint32_t cmdSize, void *pCmdData,
                   uint32_t *replySize, void *pReplyData)
{
    struct menrva_engine_context *context = (menrva_engine_context*)self;
    int result = MenrvaCommandMap::Process(context, cmdCode, cmdSize, pCmdData, replySize,
            pReplyData);
    return result;
}
int GetDescriptorFromModule(effect_handle_t self, effect_descriptor_t *pDescriptor)
{
    menrva_engine_context* module = (menrva_engine_context*)self;
    if (module == NULL || pDescriptor == NULL) {
        return -EINVAL;
    }

    *pDescriptor = effect_descriptor;
    return 0;
}
const struct effect_interface_s control_interface =
{
    Process,
    Command,
    GetDescriptorFromModule,
    NULL
};

// Menrva Module Functions
void Reset(menrva_engine_context *context){
    if (context->effectsEngine != NULL) {
        context->effectsEngine->reset();
    }
}
int SetConfig(menrva_engine_context *context, effect_config_t *pConfig) {
    if (pConfig->inputCfg.samplingRate != pConfig->outputCfg.samplingRate) {
        return -EINVAL;
    }
    if (pConfig->inputCfg.channels != pConfig->outputCfg.channels) {
        return -EINVAL;
    }
    if (pConfig->inputCfg.format != pConfig->outputCfg.format) {
        return -EINVAL;
    }
    if (pConfig->outputCfg.accessMode != EFFECT_BUFFER_ACCESS_WRITE &&
        pConfig->outputCfg.accessMode != EFFECT_BUFFER_ACCESS_ACCUMULATE) {
        return -EINVAL;
    }
    if (pConfig->inputCfg.format != AUDIO_FORMAT_PCM_FLOAT) {
        return -EINVAL;
    }

    *context->config = *pConfig;

    Reset(context);
    return 0;
}
void InitModule(menrva_engine_context *context) {
    context->effectsEngine = new MenrvaEffectsEngine;
    context->control_interface = &control_interface;

    // TODO : Configure any necessary parameters
}

// Menrva Module Interface Functions & Struct
int CreateModule(const effect_uuid_t *uuid, int32_t sessionId __unused, int32_t ioId __unused,
                 effect_handle_t *pHandle) {
    if (pHandle == NULL || uuid == NULL) {
        return -EINVAL;
    }
    if (memcmp(uuid, &effect_descriptor.uuid, sizeof(*uuid)) != 0) {
        return -EINVAL;
    }

    menrva_engine_context *module = new menrva_engine_context;
    InitModule(module);

    *pHandle = (effect_handle_t)module;
    return 0;
}
int ReleaseModule(effect_handle_t moduleHandle) {
    menrva_engine_context* module = (menrva_engine_context*)moduleHandle;

    if (module == NULL){
        return -EINVAL;
    }

    delete module->effectsEngine;
    delete module;

    return 0;
}
int GetDescriptorFromUUID(const effect_uuid_t *uuid, effect_descriptor_t *pDescriptor) {
    if (pDescriptor == NULL || uuid == NULL) {
        return -EINVAL;
    }
    else if (memcmp(uuid, &effect_descriptor.uuid, sizeof(*uuid)) != 0) {
        return -EINVAL;
    }

    *pDescriptor = effect_descriptor;
    return 0;
}
extern "C" {
    __attribute__ ((visibility ("default")))
    audio_effect_library_t AUDIO_EFFECT_LIBRARY_INFO_SYM = {
        .tag = AUDIO_EFFECT_LIBRARY_TAG,
        .version = EFFECT_LIBRARY_API_VERSION,
        .name = effect_descriptor.name,
        .implementor = effect_descriptor.implementor,
        .create_effect = CreateModule,
        .release_effect = ReleaseModule,
        .get_descriptor = GetDescriptorFromUUID,
    };
}
