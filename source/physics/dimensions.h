#pragma once
#include "dimension.h"

namespace physics::dimension
{
    using SolidAngle = decltype(Angle{} + Angle{});

    using Frequency = decltype(-Time{});

    using Wavenumber = decltype(-Length{});
    using Wavevector = decltype(Angle{} - Length{});
    using Area = decltype(Length{} + Length{});
    using Volume = decltype(Length{} + Length{} + Length{});
    using NumberDensity = decltype(-Volume{});
    using Absement = decltype(Length{} + Time{});
    using Velocity = decltype(Length{} - Time{});
    using Acceleration = decltype(Velocity{} - Time{});
    using Jerk = decltype(Acceleration{} - Time{});
    using Jounce = decltype(Jerk{} - Time{});
    using Crackle = decltype(Jounce{} - Time{});
    using Pop = decltype(Crackle{} - Time{});
    using Diffusivity = decltype(Area{} - Time{});
    using VolumetricFlowRate = decltype(Volume{} - Time{});

    using LinearDensity = decltype(Mass{} - Length{});
    using AreaDensity = decltype(Mass{} - Area{});
    using MassDensity = decltype(Mass{} - Volume{});
    using MassFlowRate = decltype(Mass{} - Time{});
    using MassFlux = decltype(MassFlowRate{} - Area{});
    using MolarMass = decltype(Mass{} - AmountOfSubstance{});
    using SpecificVolume = decltype(Volume{} - Mass{});

    using Momentum = decltype(Mass{} + Velocity{});
    using Force = decltype(Mass{} + Acceleration{});
    using SurfaceTension = decltype(Force{} - Length{});
    using Pressure = decltype(Force{} - Area{});
    using Viscosity = decltype(Pressure{} + Time{});
    using MomentOfInertia = decltype(Mass{} + Area{} - SolidAngle{});
    using Action = decltype(Momentum{} + Length{});
    using AngularMomentum = decltype(Action{} - Angle{});

    using Energy = decltype(Force{} + Length{});
    using Torque = decltype(Energy{} - Angle{});
    using Power = decltype(Energy{} - Time{});
    using Intensity = decltype(Power{} - Area{});
    using RadiantIntensity = decltype(Power{} - SolidAngle{});
    using Radiance = decltype(RadiantIntensity{} - Area{});
    using SpecificEnergy = decltype(Energy{} - Mass{});
    using SpecificPower = decltype(Power{} - Mass{});
    using MolarEnergy = decltype(Energy{} - AmountOfSubstance{});

    using TemperatureGradient = decltype(Temperature{} - Length{});
    using ThermalExpansionCoefficient = decltype(-Temperature{});
    using Entropy = decltype(Energy{} - Temperature{});
    using SpecificHeatCapacity = decltype(SpecificEnergy{} - Temperature{});
    using MolarEntropy = decltype(MolarEnergy{} - Temperature{});
    using VolumetricHeatCapacity = decltype(Entropy{} - Volume{});
    using ThermalConductivity = decltype(Power{} - Length{} - Temperature{});
    using HeatTransferCoefficient = decltype(Intensity{} - Temperature{});
    using ThermalConductance = decltype(Power{} - Temperature{});
    using ThermalResistance = decltype(Temperature{} - Power{});
    using ThermalResistivity = decltype(ThermalResistance{} + Length{});
    using Compressibility = decltype(-Pressure{});

    using ElectricCharge = decltype(ElectricCurrent{} + Time{});
    using CurrentDensity = decltype(ElectricCurrent{} - Area{});
    using LinearChargeDensity = decltype(ElectricCharge{} - Length{});
    using ElectricDisplacementField = decltype(ElectricCharge{} - Area{});
    using ElectricChargeDensity = decltype(ElectricCharge{} - Volume{});
    using ElectricDipoleMoment = decltype(ElectricCharge{} + Length{});
    using ElectricFieldStrength = decltype(Force{} - ElectricCharge{});
    using ElectricPotential = decltype(Energy{} - ElectricCharge{});
    using ElectricalResistance = decltype(ElectricPotential{} - ElectricCurrent{});
    using ElectricalResistivity = decltype(ElectricalResistance{} + Length{});
    using ElectricalConductance = decltype(-ElectricalResistance{});
    using ElectricalConductivity = decltype(-ElectricalResistivity{});
    using Capacitance = decltype(ElectricCharge{} - ElectricPotential{});
    using Permittivity = decltype(ElectricDisplacementField{} - ElectricFieldStrength{});

    using MagneticFieldStrength = decltype(ElectricCurrent{} - Length{});
    using MagneticMoment = decltype(ElectricCurrent{} + Area{} - Angle{});
    using Magnetization = decltype(MagneticMoment{} - Volume{});
    using MagneticFlux = decltype(ElectricPotential{} + Time{});
    using MagneticFluxDensity = decltype(MagneticFlux{} - Area{});
    using Inductance = decltype(MagneticFlux{} - ElectricCurrent{});
    using Permeability = decltype(MagneticFluxDensity{} - MagneticFieldStrength{});
    using Reluctance = decltype(ElectricCurrent{} - MagneticFlux{});

    using MolarConcentration = decltype(AmountOfSubstance{} - Volume{});
    using MolarVolume = decltype(Volume{} - AmountOfSubstance{});
    using CatalyticActivity = decltype(AmountOfSubstance{} - Time{});
    using ReactionRate = decltype(CatalyticActivity{} - Volume{});

    using LuminousFlux = decltype(LuminousIntensity{} + SolidAngle{});
    using Illuminance = decltype(LuminousFlux{} - Area{});
    using Luminance = decltype(LuminousIntensity{} - Area{});
    using LuminousEfficacy = decltype(LuminousFlux{} - Power{});

    using AngularVelocity = decltype(Angle{} - Time{});
    using AngularAcceleration = decltype(AngularVelocity{} - Time{});
    using AngularJerk = decltype(AngularAcceleration{} - Time{});
}
