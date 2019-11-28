#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#pragma once

// samplers/gaussjitter.h*
#include "sampler.h"
#include "rng.h"

namespace pbrt {

// PoissonSampler Declarations
class PoissonSampler : public PixelSampler {
  public:
    // PoissonSampler Public Methods
    PoissonSampler(int xPixelSamples, int yPixelSamples, bool jitterSamples,
                      int nSampledDimensions)
        : PixelSampler(xPixelSamples * yPixelSamples, nSampledDimensions),
          xPixelSamples(xPixelSamples),
          yPixelSamples(yPixelSamples),
          jitterSamples(jitterSamples) {}
    void StartPixel(const Point2i &);
    std::unique_ptr<Sampler> Clone(int seed);

  private:
    // GaussSampler Private Data
    const int xPixelSamples, yPixelSamples;
    const bool jitterSamples;
};

PoissonSampler *CreatePoissonSampler(const ParamSet &params);

}  // namespace pbrt
