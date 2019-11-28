// samplers/poisson.cpp*
#include "samplers/poisson.h"
#include "paramset.h"
#include "sampling.h"
#include "stats.h"

namespace pbrt {

// PoissonSampler Method Definitions
void PoissonSampler::StartPixel(const Point2i &p) {
    ProfilePhase _(Prof::StartPixel);

    typedef bool (*PoissonSample1D_fn)(Float *samp, int nSamples, RNG &rng, Float radius);
    typedef bool (*PoissonSample2D_fn)(Point2f *samp, int nSamples, RNG &rng, Float radius);

    auto sample1D = (PoissonSample1D_fn) PoissonSample1D;
    auto sample2D = (PoissonSample2D_fn) PoissonSample2D;

    if (relaxed) {
        sample1D = (PoissonSample1D_fn) PoissonRelaxedSample1D;
        sample2D = (PoissonSample2D_fn) PoissonRelaxedSample2D;
    }

    // Generate single poisson samples for the pixel
    for (size_t i = 0; i < samples1D.size(); ++i) {
        sample1D(&samples1D[i][0], samplesPerPixel, rng, radius);
    }
    for (size_t i = 0; i < samples2D.size(); ++i) {
        sample2D(&samples2D[i][0], samplesPerPixel, rng, radius);
    }

    // Generate arrays of poisson samples for the pixel
    for (size_t i = 0; i < samples1DArraySizes.size(); ++i)
        for (int64_t j = 0; j < samplesPerPixel; ++j) {
            int count = samples1DArraySizes[i];
            sample1D(&sampleArray1D[i][j * count], samplesPerPixel, rng, radius);
        }
    for (size_t i = 0; i < samples2DArraySizes.size(); ++i)
        for (int64_t j = 0; j < samplesPerPixel; ++j) {
            int count = samples2DArraySizes[i];
            sample2D(&sampleArray2D[i][j * count], samplesPerPixel, rng, radius);
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
    Float radius = params.FindOneFloat("radius", 0.1);
    bool relaxed = params.FindOneBool("relaxed", false);
    std::cout << "Relaxed: " << relaxed << std::endl;
    if (PbrtOptions.quickRender) samplesPerPixel = 1;
    return new PoissonSampler(samplesPerPixel, nSampledDimensions, relaxed, radius);
}

}  // namespace pbrt
