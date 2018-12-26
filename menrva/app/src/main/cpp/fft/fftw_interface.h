// Author : Jman420

#ifndef MENRVA_FFTW_INTERFACE_H
#define MENRVA_FFTW_INTERFACE_H

#include <map>
#include <fftw3.h>
#include "../abstracts/fft_interface_base.h"

struct fftw_plan_pair {
    fftwf_plan Real2ComplexPlan,
               Complex2RealPlan;
};
typedef std::map<std::string, fftw_plan_pair> PlanCache;

class FFTW_Interface : public FFTInterfaceBase {
public:
    FFTW_Interface(unsigned int signalSize, unsigned int componentSize);
    int Initialize(unsigned int signalSize, unsigned int componentSize = 0) override;
    void SignalToComponents(float* signal, float* realComponents, float* imagComponents) override;
    void ComponentsToSignal(float* signal, float* realComponents, float* imagComponents) override;
    float* Allocate(unsigned int size) override;
    void Deallocate(float* data) override;

protected:
    static PlanCache* _PlansCache;

    fftw_plan_pair _Plans;
};

#endif //MENRVA_FFTW_INTERFACE_H
