
#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#pragma once

#include <utility>

#include "pbrt.h"
#include "material.h"

namespace pbrt {

// AnphMaterial Declarations
class AnphMaterial : public Material {
  public:
    // AnphMaterial Public Methods
    AnphMaterial(bool importance, std::shared_ptr<Texture<Spectrum>> Rd,
                  std::shared_ptr<Texture<Spectrum>> Rs,
                  std::shared_ptr<Texture<Float>> Nv,
                  std::shared_ptr<Texture<Float>> Nu)
        : Rd(std::move(Rd)), Rs(std::move(Rs)), Nv(std::move(Nv)), Nu(std::move(Nu)) {}
    void ComputeScatteringFunctions(SurfaceInteraction *si, MemoryArena &arena,
                                    TransportMode mode,
                                    bool allowMultipleLobes) const;

  private:
    // AnphMaterial Private Data
    std::shared_ptr<Texture<Spectrum>> Rd, Rs;
    std::shared_ptr<Texture<Float>> Nv, Nu;
    bool importance;
};

AnphMaterial *CreateAnphMaterial(const TextureParams &mp);

}  // namespace pbrt
