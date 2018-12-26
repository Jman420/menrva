// Author : Jman420

#include "fft_interface_base.h"

FFTInterfaceBase::FFTInterfaceBase(unsigned int signalSize, unsigned int componentSize) {
    Initialize(signalSize, componentSize);
}

int FFTInterfaceBase::getSignalSize() {
    return _SignalSize;
}

int FFTInterfaceBase::getComponentSize() {
    return _ComponentSize;
}

int FFTInterfaceBase::Initialize(unsigned int signalSize, unsigned int componentSize) {
    _SignalSize = signalSize;
    _ComponentSize = componentSize;

    return componentSize;
}
