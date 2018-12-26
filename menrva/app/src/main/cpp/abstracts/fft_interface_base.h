// Author : Jman420

#ifndef MENRVA_FFTENGINEBASE_H
#define MENRVA_FFTENGINEBASE_H

class FFTInterfaceBase {
public:
    FFTInterfaceBase(unsigned int signalSize, unsigned int componentSize = 0);
    virtual int Initialize(unsigned int signalSize, unsigned int componentSize = 0);
    virtual void SignalToComponents(float* signal, float* realComponents, float* imagComponents) = 0;
    virtual void ComponentsToSignal(float* signal, float* realComponents, float* imagComponents) = 0;
    virtual float* Allocate(unsigned int size) = 0;
    virtual void Deallocate(float* data) = 0;
    int getSignalSize();
    int getComponentSize();

protected:
    int _SignalSize,
        _ComponentSize;
};

#endif //MENRVA_FFTENGINEBASE_H
