// samplers/poisson.cpp*
#include "samplers/poisson.h"
#include "paramset.h"
#include "sampling.h"
#include "stats.h"

namespace pbrt {

// PoissonSampler Method Definitions
void PoissonSampler::StartPixel(const Point2i &p) {
    ProfilePhase _(Prof::StartPixel);



    // Generate single poisson samples for the pixel
    for (size_t i = 0; i < samples1D.size(); ++i) {
        PoissonSample1D(&samples1D[i][0], samplesPerPixel, rng, radius);
    }
    for (size_t i = 0; i < samples2D.size(); ++i) {
        PoissonSample2D(&samples2D[i][0], samplesPerPixel, rng, radius);
    }

    // Generate arrays of poisson samples for the pixel
    for (size_t i = 0; i < samples1DArraySizes.size(); ++i)
        for (int64_t j = 0; j < samplesPerPixel; ++j) {
            int count = samples1DArraySizes[i];
            PoissonSample1D(&sampleArray1D[i][j * count], samplesPerPixel, rng, radius);
        }
    for (size_t i = 0; i < samples2DArraySizes.size(); ++i)
        for (int64_t j = 0; j < samplesPerPixel; ++j) {
            int count = samples2DArraySizes[i];
            PoissonSample2D(&sampleArray2D[i][j * count], samplesPerPixel, rng, radius);
        }

    PixelSampler::StartPixel(p);
}

std::unique_ptr<Sampler> PoissonSampler::Clone(int seed) {
    auto *ss = new PoissonSampler(*this);
    ss->rng.SetSequence(seed);
    return std::unique_ptr<Sampler>(ss);
}

PoissonSampler *CreatePoissonSampler(const ParamSet &params) {
    int samplesPerPixel = params.FindOneInt("pixelsamples", 16);
    int nSampledDimensions = params.FindOneInt("dimensions", 4);
    Float radius = params.FindOneFloat("radius", 0.0001);
    bool relaxed = params.FindOneBool("relaxed", false);
    if (PbrtOptions.quickRender) samplesPerPixel = 1;
    return new PoissonSampler(samplesPerPixel, nSampledDimensions, relaxed, radius);
}

}  // namespace pbrt
