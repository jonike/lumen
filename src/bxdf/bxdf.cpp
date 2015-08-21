#include <bxdf.h>

namespace lumen {
nex::vector reflect(const nex::vector& incident, const nex::vector& normal)
{
        return 2.0f * normal * nex::dot(normal, incident) - incident;
}

nex::vector refract(const nex::vector& incident, const nex::vector& normal, float* eta)
{
        nex::vector n = normal;

        float cos = nex::dot(normal, incident);

        // check if the incident direction is inside the medium
        if (cos < 0.0f) {
                cos = -cos;
                n = -n;
                *eta = 1.0f / *eta;
        }

        // check for total internal reflection
        float sin2t = (*eta * *eta) * (1.0f - cos * cos);

        if (sin2t > 1.0f) {
                return nex::vector(0.0f, 0.0f, 0.0f);
        }

        return -(*eta) * incident + (*eta * cos - std::sqrt(1.0f - sin2t)) * n;
}

float fresnel(float ni, float nt, float cosi)
{
        // flip the index of refraction if the incident direction is inside the
        // transmitting medium
        if (cosi < 0.0f) {
                float temp = ni;

                ni = nt;
                nt = temp;
                cosi = -cosi;
        }

        // compute the transmitted angle using Snell's Law
        float sint = ni / nt * std::sqrt(1.0f - cosi * cosi);

        // handle total internal reflection
        if (sint > 1.0f) {
                return 1.0f;
        }

        float cost = std::sqrt(1.0f - sint * sint);

        // compute the fresnel coefficient
        float rp = (nt * cosi - ni * cost) / (nt * cosi + ni * cost);
        float rs = (ni * cosi - nt * cost) / (ni * cosi + nt * cost);

        return (rp * rp + rs * rs) * 0.5f;
}
}
