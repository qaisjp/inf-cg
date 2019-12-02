
#include "materials/anph.h"
#include "paramset.h"
#include "reflection.h"
#include "interaction.h"
#include "texture.h"
#include "interaction.h"

namespace pbrt {

// AnphMaterial Method Definitions
void AnphMaterial::ComputeScatteringFunctions(SurfaceInteraction *si,
                                               MemoryArena &arena,
                                               TransportMode mode,
                                               bool allowMultipleLobes) const {

    Spectrum rd = Rd->Evaluate(*si).Clamp();
//    std::cout << "s: " << rd[0] << ", " << rd[1] << ", " << rd[2] << std::endl;
    Spectrum rs = Rs->Evaluate(*si).Clamp();
    Float nv = Nv->Evaluate(*si);
    Float nu = Nu->Evaluate(*si);

    // Evaluate textures for _AnphMaterial_ material and allocate BRDF
    si->bsdf = ARENA_ALLOC(arena, BSDF)(*si);
    si->bsdf->Add(ARENA_ALLOC(arena, AnphBxDF)(
            importance,
            rd, rs, nv, nu,
            si->dpdu, si->dpdv));
}

AnphMaterial *CreateAnphMaterial(const TextureParams &mp) {
    std::shared_ptr<Texture<Spectrum>> Rd =
        mp.GetSpectrumTexture("Rd", Spectrum(1.0f));
    std::shared_ptr<Texture<Spectrum>> Rs =
        mp.GetSpectrumTexture("Rs", Spectrum(0.5f));
    std::shared_ptr<Texture<Float>> Nv = mp.GetFloatTexture("nv", 0.f);
    std::shared_ptr<Texture<Float>> Nu = mp.GetFloatTexture("nu", 0.f);

    bool importance = mp.GetGeomParams().FindOneBool("importance", true);
    std::cout << "Importance sampling: " << (importance ? "enabled" : "disabled") << std::endl;
    return new AnphMaterial(importance, Rd, Rs, Nv, Nu);
}

}  // namespace pbrt
