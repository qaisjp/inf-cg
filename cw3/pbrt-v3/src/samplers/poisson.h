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
    PoissonSampler(int samplesPerPixel, int nSampledDimensions, bool relaxed, Float radius)
        : PixelSampler(samplesPerPixel, nSampledDimensions),
          relaxed(relaxed), radius(radius) {}
    void StartPixel(const Point2i &);
    std::unique_ptr<Sampler> Clone(int seed);

  private:
    // PoissonSampler Private Data
    const bool relaxed;
    const Float radius;
};

PoissonSampler *CreatePoissonSampler(const ParamSet &params);

}  // namespace pbrt
