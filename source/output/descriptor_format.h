#pragma once

namespace output
{
    // Study-owned specializations describe kinetic descriptors for CSV and labels.
    // The KMC model and simulation do not depend on this presentation contract.
    template <class Descriptor>
    struct DescriptorFormat;
}
