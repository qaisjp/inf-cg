---
title: Computer Graphics - CW3
author: QP - s1620208@inf.ed.ac.uk
geometry: margin=2cm
---
Kurwa!

But also, kurwa?


**sometimes..**

|            | Nu = 10                         | Nu = 100                        | Nu = 1000                       | Nu = 10000                      |
|------------|---------------------------------|---------------------------------|---------------------------------|---------------------------------|
| Nv = 10000 | ![](manysphere/sphere-Nv10000-Nu10.png) | ![](manysphere/sphere-Nv10000-Nu100.png) | ![](manysphere/sphere-Nv10000-Nu1000.png) | ![](manysphere/sphere-Nv10000-Nu10000.png) |
| Nv = 1000  | ![](manysphere/sphere-Nv1000-Nu10.png) | ![](manysphere/sphere-Nv1000-Nu100.png) | ![](manysphere/sphere-Nv1000-Nu1000.png) | ![](manysphere/sphere-Nv1000-Nu10000.png) |
| Nv = 100   | ![](manysphere/sphere-Nv100-Nu10.png) | ![](manysphere/sphere-Nv100-Nu100.png) | ![](manysphere/sphere-Nv100-Nu1000.png) | ![](manysphere/sphere-Nv100-Nu10000.png) |
| Nv = 10    | ![](manysphere/sphere-Nv10-Nu10.png) | ![](manysphere/sphere-Nv10-Nu100.png) | ![](manysphere/sphere-Nv10-Nu1000.png) | ![](manysphere/sphere-Nv10-Nu10000.png) |
|            | Nu = 10                         | Nu = 100                        | Nu = 1000                       | Nu = 10000                      |

```c++

Spectrum AnphBxDF::Sample_f(const Vector3f &wo, Vector3f *wi,
                              const Point2f &u, Float *pdf,
                              BxDFType *sampledType) const {
    Float phi, theta;
    if(u.x < 0.25){
        Q1(u, &phi, &theta);
    } else if(u.x < 0.5){
        Q2(u, &phi, &theta);
    } else if(u.x < 0.75){
        Q3(u, &phi, &theta);
    } else {
        Q4(u, &phi, &theta);
    }

    auto h = Vector3f(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
    *wi = -wo + (2 * Dot(wo, h) * h);
    *pdf = Pdf(wo, *wi);

    return f(wo, *wi);
}

Float AnphBxDF::Pdf(const Vector3f &wo, const Vector3f &wi) const {
    auto h = Normalize((Normalize(wo) + Normalize(wi)) / 2);

    auto a = sqrt((Nu+1)*(Nv+1)) / (2 * Pi);
    auto exp = (Nu * Cos2Phi(h)) + (Nv * Sin2Phi(h));
    auto b = pow(AbsCosTheta(h), exp);

    return a * b;
}

std::string AnphBxDF::ToString() const {
    return std::string("[ AnphdBxDF: ") + std::string(" ]");
}

```