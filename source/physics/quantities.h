#pragma once
#include "dimensions.h"
#include "quantity.h"

namespace physics
{
    // Dimensionless number of objects.
    using Number = Quantity<Dimensionless, Reference::zero, Domain::non_negative>;
    // Signed additive dimensionless number.
    using SignedNumber = Quantity<Dimensionless, Reference::zero, Domain::unrestricted>;

    // Signed dimensionless difference with no retained reference.
    using Difference = Quantity<Dimensionless, Reference::difference>;
    // Magnitude of a dimensionless difference.
    using Magnitude = Quantity<Dimensionless, Reference::difference, Domain::non_negative>;

    // Multiplicative dimensionless factor.
    using Factor = Quantity<Dimensionless, Reference::ratio, Domain::unrestricted>;
    // Ratio of two nonnegative quantities of the same kind.
    using Ratio = Quantity<Dimensionless, Reference::ratio, Domain::non_negative>;
    // Dimensionless fraction of a whole.
    using Fraction = Quantity<Dimensionless, Reference::ratio, Domain::unit_interval>;

    using Probability = Fraction;
}

namespace physics::quantity
{
    // Ratio of two signed quantities of the same kind.
    using SignedRatio = Factor;
    using Coefficient = Factor;


    // Mass of a constituent divided by total mass.
    using MassFraction = Fraction;
    // Volume of a constituent divided by total volume.
    using VolumeFraction = Fraction;
    // Amount of substance of a constituent divided by total amount of substance.
    using MoleFraction = Fraction;
    // Fraction of a material volume occupied by voids.
    using Porosity = Fraction;
    // Relative deformation of a body.
    using Strain = SignedNumber;
    // Engineering strain relative to the original length.
    using EngineeringStrain = SignedNumber;
    // Logarithmic strain based on incremental deformation.
    using TrueStrain = SignedNumber;
    // Ratio of transverse to axial strain.
    using PoissonsRatio = SignedRatio;
    // Ratio of the speed of an object to the local speed of sound.
    using Mach = Ratio;
    // Ratio of light speed in vacuum to that in a medium.
    using RefractiveIndex = SignedRatio;
    // Permittivity relative to vacuum permittivity.
    using RelativePermittivity = SignedRatio;
    // Permeability relative to vacuum permeability.
    using RelativePermeability = SignedRatio;
    // Linear magnetic response of a material.
    using MagneticSusceptibility = SignedRatio;
    // Ratio between friction force and normal force.
    using CoefficientOfFriction = Ratio;
    // Fraction of black-body radiation emitted by a surface.
    using Emissivity = Fraction;
    // Fraction of incident radiation reflected by a surface.
    using Reflectance = Fraction;
    // Fraction of incident radiation transmitted through a material.
    using Transmittance = Fraction;

    // Ratio of circular arc length to radius.
    using PlaneAngle = Quantity<dimension::Angle>;
    // Ratio of spherical surface area to radius squared.
    using SolidAngle = Quantity<dimension::SolidAngle, Reference::zero, Domain::non_negative>;

    // Coordinate on a time axis relative to an epoch.
    using Time = Quantity<dimension::Time, Reference::zero>;
    // Signed difference between two time points.
    using TimeDifference = Quantity<dimension::Time, Reference::difference>;
    // Nonnegative elapsed time.
    using Duration = Quantity<dimension::Time, Reference::difference, Domain::non_negative>;

    // Duration of one cycle of a periodic process.
    using Period = Duration;
    // Time required for a quantity to fall to half its initial value.
    using HalfLife = Duration;
    // Mean lifetime of an unstable state or population.
    using MeanLifetime = Duration;

    // Number of periodic occurrences per unit time.
    using Frequency = Quantity<dimension::Frequency, Reference::zero, Domain::non_negative>;
    // Rate of change of angular position.
    using AngularVelocity = Quantity<dimension::AngularVelocity>;
    // Number of nuclear decays per unit time.
    using Radioactivity = Quantity<dimension::Frequency, Reference::zero, Domain::non_negative>;
    // First-order exponential decay coefficient.
    using DecayConstant = Quantity<dimension::Frequency, Reference::zero, Domain::non_negative>;
    // Rate of deformation per unit time.
    using StrainRate = Quantity<dimension::Frequency>;
    // Rate of shear deformation per unit time.
    using ShearRate = Quantity<dimension::Frequency>;
    // Rate of change of angular velocity.
    using AngularAcceleration = Quantity<dimension::AngularAcceleration>;

    // Position coordinate measured from a reference.
    using Position = Quantity<dimension::Length, Reference::zero>;
    // Radius of a circle or sphere.
    using Radius = Quantity<dimension::Length, Reference::zero, Domain::non_negative>;
    // Diameter of a circle or sphere.
    using Diameter = Quantity<dimension::Length, Reference::zero, Domain::non_negative>;
    // Thickness of a layer or body.
    using Thickness = Quantity<dimension::Length, Reference::zero, Domain::non_negative>;
    // Spatial period of a wave.
    using Wavelength = Quantity<dimension::Length, Reference::zero, Domain::non_negative>;
    // One-dimensional spatial extent.
    using Length = Quantity<dimension::Length, Reference::zero, Domain::non_negative>;

    // Change in position.
    using Displacement = Quantity<dimension::Length, Reference::difference>;
    // Magnitude of the separation between two positions.
    using Distance = Quantity<dimension::Length, Reference::difference, Domain::non_negative>;
    // Number of wave cycles per unit distance.
    using Wavenumber = Quantity<dimension::Wavenumber, Reference::zero, Domain::non_negative>;
    // Spatial frequency vector magnitude.
    using Wavevector = Quantity<dimension::Wavevector, Reference::zero, Domain::non_negative>;
    // Reciprocal focal length of an optical system.
    using OpticalPower = Quantity<dimension::Wavenumber>;
    // Extent of a surface.
    using Area = Quantity<dimension::Area, Reference::zero, Domain::non_negative>;
    // Three-dimensional extent of a body or region.
    using Volume = Quantity<dimension::Volume, Reference::zero, Domain::non_negative>;
    // Number of dimensionless entities per unit volume.
    using NumberDensity = Quantity<dimension::NumberDensity, Reference::zero, Domain::non_negative>;
    // Time integral of displacement.
    using Absement = Quantity<dimension::Absement>;
    // Rate of change of position.
    using Velocity = Quantity<dimension::Velocity>;
    // Magnitude of velocity.
    using Speed = Quantity<dimension::Velocity, Reference::zero, Domain::non_negative>;
    // Rate of change of velocity.
    using Acceleration = Quantity<dimension::Acceleration>;
    // Rate of change of acceleration.
    using Jerk = Quantity<dimension::Jerk>;
    // Fourth time derivative of position.
    using Jounce = Quantity<dimension::Jounce>;
    // Fifth time derivative of position.
    using Crackle = Quantity<dimension::Crackle>;
    // Sixth time derivative of position.
    using Pop = Quantity<dimension::Pop>;
    // Generic transport coefficient with dimensions of area per time.
    using Diffusivity = Quantity<dimension::Diffusivity>;
    // Thermal diffusivity governing heat spreading in a material.
    using ThermalDiffusivity = Quantity<dimension::Diffusivity>;
    // Momentum diffusivity of a fluid.
    using KinematicViscosity = Quantity<dimension::Diffusivity>;
    // Diffusion coefficient in Fickian transport.
    using DiffusionCoefficient = Quantity<dimension::Diffusivity>;
    // Volume transported per unit time.
    using VolumetricFlowRate = Quantity<dimension::VolumetricFlowRate>;

    // Resistance of a body to acceleration.
    using Mass = Quantity<dimension::Mass, Reference::zero, Domain::non_negative>;
    // Mass per unit length.
    using LinearDensity = Quantity<dimension::LinearDensity, Reference::zero, Domain::non_negative>;
    // Mass per unit area.
    using AreaDensity = Quantity<dimension::AreaDensity, Reference::zero, Domain::non_negative>;
    // Mass per unit volume.
    using MassDensity = Quantity<dimension::MassDensity, Reference::zero, Domain::non_negative>;
    // Mass transported per unit time.
    using MassFlowRate = Quantity<dimension::MassFlowRate>;
    // Mass transported through unit area per unit time.
    using MassFlux = Quantity<dimension::MassFlux>;
    // Mass per unit amount of substance.
    using MolarMass = Quantity<dimension::MolarMass, Reference::zero, Domain::non_negative>;
    // Volume per unit mass.
    using SpecificVolume =
        Quantity<dimension::SpecificVolume, Reference::zero, Domain::non_negative>;

    // Linear momentum of a body.
    using Momentum = Quantity<dimension::Momentum>;
    // Change in momentum accumulated over time.
    using Impulse = Quantity<dimension::Momentum, Reference::difference>;
    // Explicit name for translational momentum.
    using LinearMomentum = Quantity<dimension::Momentum>;
    // Rate of change of momentum.
    using Force = Quantity<dimension::Force>;
    // Gravitational force acting on a body.
    using Weight = Quantity<dimension::Force>;
    // Apparent outward force in a rotating reference frame.
    using CentrifugalForce = Quantity<dimension::Force>;
    // Propulsive force produced by an engine or actuator.
    using Thrust = Quantity<dimension::Force>;
    // Resistive force opposing motion through a fluid.
    using Drag = Quantity<dimension::Force>;
    // Force component perpendicular to a flow direction.
    using Lift = Quantity<dimension::Force>;

    // Normal force per unit area in a fluid or solid.
    using Pressure = Quantity<dimension::Pressure>;
    // Internal force per unit area in a material.
    using Stress = Quantity<dimension::Pressure>;
    // Stress component normal to a plane.
    using NormalStress = Quantity<dimension::Pressure>;
    // Stress component tangential to a plane.
    using ShearStress = Quantity<dimension::Pressure>;
    // Energy stored or carried per unit volume.
    using EnergyDensity = Quantity<dimension::Pressure>;
    // Ratio of uniaxial stress to elastic strain.
    using YoungsModulus = Quantity<dimension::Pressure>;
    // Ratio of shear stress to shear strain.
    using ShearModulus = Quantity<dimension::Pressure>;
    // Resistance to uniform compression.
    using BulkModulus = Quantity<dimension::Pressure>;
    // Generic elastic stiffness expressed as stress per strain.
    using ElasticModulus = Quantity<dimension::Pressure>;
    // Resistance to local permanent indentation.
    using Hardness = Quantity<dimension::Pressure>;
    // Maximum tensile stress sustained by a material.
    using TensileStrength = Quantity<dimension::Pressure>;
    // Stress at the onset of plastic deformation.
    using YieldStrength = Quantity<dimension::Pressure>;
    // Maximum compressive stress sustained by a material.
    using CompressiveStrength = Quantity<dimension::Pressure>;
    // Stress associated with fracture under a specified loading mode.
    using FractureStrength = Quantity<dimension::Pressure>;
    // Reciprocal bulk modulus or pressure sensitivity of volume.
    using Compressibility = Quantity<dimension::Compressibility>;

    // Dynamic resistance of a fluid to shear deformation.
    using Viscosity = Quantity<dimension::Viscosity>;
    // Explicit name for dynamic viscosity.
    using DynamicViscosity = Quantity<dimension::Viscosity>;

    // Rotational inertia about an axis.
    using MomentOfInertia =
        Quantity<dimension::MomentOfInertia, Reference::zero, Domain::non_negative>;
    // Rotational analogue of linear momentum.
    using AngularMomentum = Quantity<dimension::AngularMomentum>;
    // Intrinsic angular momentum.
    using Spin = Quantity<dimension::AngularMomentum>;
    // Energy-time or momentum-length action.
    using Action = Quantity<dimension::Action>;

    // Mechanical or thermodynamic energy.
    using Energy = Quantity<dimension::Energy>;
    // Energy transferred by a force through a displacement.
    using Work = Quantity<dimension::Energy, Reference::difference>;
    // Energy transferred because of a temperature difference.
    using Heat = Quantity<dimension::Energy, Reference::difference>;
    // Moment of force about an axis.
    using Torque = Quantity<dimension::Torque>;
    // Microscopic and macroscopic energy stored within a system.
    using InternalEnergy = Quantity<dimension::Energy>;
    // Thermodynamic energy including pressure-volume work.
    using Enthalpy = Quantity<dimension::Energy>;
    // Gibbs free energy of a system.
    using GibbsFreeEnergy = Quantity<dimension::Energy>;
    // Helmholtz free energy of a system.
    using HelmholtzFreeEnergy = Quantity<dimension::Energy>;
    // Energy barrier associated with a process or reaction.
    using ActivationEnergy = Quantity<dimension::Energy, Reference::difference>;

    // Energy per unit mass.
    using SpecificEnergy = Quantity<dimension::SpecificEnergy>;
    // Internal energy per unit mass.
    using SpecificInternalEnergy = Quantity<dimension::SpecificEnergy>;
    // Enthalpy per unit mass.
    using SpecificEnthalpy = Quantity<dimension::SpecificEnergy>;
    // Gibbs free energy per unit mass.
    using SpecificGibbsFreeEnergy = Quantity<dimension::SpecificEnergy>;
    // Helmholtz free energy per unit mass.
    using SpecificHelmholtzFreeEnergy = Quantity<dimension::SpecificEnergy>;
    // Absorbed ionizing-radiation energy per unit mass.
    using AbsorbedDose = Quantity<dimension::SpecificEnergy, Reference::zero, Domain::non_negative>;
    // Dose weighted for biological effectiveness.
    using DoseEquivalent =
        Quantity<dimension::SpecificEnergy, Reference::zero, Domain::non_negative>;
    // Generic radioactive dose expressed as energy per mass.
    using RadioactiveDose =
        Quantity<dimension::SpecificEnergy, Reference::zero, Domain::non_negative>;

    // Power per unit mass.
    using SpecificPower = Quantity<dimension::SpecificPower>;
    // Rate of absorbed dose.
    using AbsorbedDoseRate =
        Quantity<dimension::SpecificPower, Reference::zero, Domain::non_negative>;

    // Energy per unit amount of substance.
    using MolarEnergy = Quantity<dimension::MolarEnergy>;
    // Partial molar Gibbs free energy of a species.
    using ChemicalPotential = Quantity<dimension::MolarEnergy>;
    // Enthalpy per unit amount of substance.
    using MolarEnthalpy = Quantity<dimension::MolarEnergy>;
    // Gibbs free energy per unit amount of substance.
    using MolarGibbsFreeEnergy = Quantity<dimension::MolarEnergy>;
    // Helmholtz free energy per unit amount of substance.
    using MolarHelmholtzFreeEnergy = Quantity<dimension::MolarEnergy>;

    // Rate of energy transfer.
    using Power = Quantity<dimension::Power>;
    // Electromagnetic power emitted into unit solid angle.
    using RadiantIntensity =
        Quantity<dimension::RadiantIntensity, Reference::zero, Domain::non_negative>;
    // Total radiant power emitted or received.
    using RadiantFlux = Quantity<dimension::Power, Reference::zero, Domain::non_negative>;
    // Rate of heat transfer.
    using HeatFlowRate = Quantity<dimension::Power>;
    // Mechanical work performed per unit time.
    using MechanicalPower = Quantity<dimension::Power>;
    // Electrical energy transferred per unit time.
    using ElectricalPower = Quantity<dimension::Power>;
    // Acoustic energy transferred per unit time.
    using AcousticPower = Quantity<dimension::Power, Reference::zero, Domain::non_negative>;

    // Power transported through unit area.
    using Intensity = Quantity<dimension::Intensity, Reference::zero, Domain::non_negative>;
    // Incident radiant power per unit area.
    using Irradiance = Quantity<dimension::Intensity, Reference::zero, Domain::non_negative>;
    // Radiant power per projected area and unit solid angle.
    using Radiance = Quantity<dimension::Radiance, Reference::zero, Domain::non_negative>;
    // Radiant power leaving a surface per unit area.
    using RadiantExitance = Quantity<dimension::Intensity, Reference::zero, Domain::non_negative>;
    // Heat flow through unit area.
    using HeatFluxDensity = Quantity<dimension::Intensity>;
    // Energy transported through unit area per unit time.
    using EnergyFluxDensity = Quantity<dimension::Intensity>;

    // Energy change per unit surface area.
    using SurfaceTension = Quantity<dimension::SurfaceTension>;
    // Surface free energy per unit area.
    using SurfaceEnergyDensity = Quantity<dimension::SurfaceTension>;

    // Absolute thermodynamic temperature.
    using AbsoluteTemperature =
        Quantity<dimension::Temperature, Reference::zero, Domain::non_negative>;
    // Difference between two temperatures.
    using TemperatureDifference = Quantity<dimension::Temperature, Reference::difference>;
    // Spatial rate of change of temperature.
    using TemperatureGradient = Quantity<dimension::TemperatureGradient>;
    // Relative length change per unit temperature change.
    using LinearThermalExpansionCoefficient = Quantity<dimension::ThermalExpansionCoefficient>;
    // Relative volume change per unit temperature change.
    using VolumetricThermalExpansionCoefficient = Quantity<dimension::ThermalExpansionCoefficient>;
    // Entropy of a system.
    using Entropy = Quantity<dimension::Entropy>;
    // Heat required per unit temperature change.
    using HeatCapacity = Quantity<dimension::Entropy>;
    // Heat capacity per unit mass.
    using SpecificHeatCapacity = Quantity<dimension::SpecificHeatCapacity>;
    // Entropy per unit mass.
    using SpecificEntropy = Quantity<dimension::SpecificHeatCapacity>;
    // Entropy per unit amount of substance.
    using MolarEntropy = Quantity<dimension::MolarEntropy>;
    // Heat capacity per unit amount of substance.
    using MolarHeatCapacity = Quantity<dimension::MolarEntropy>;
    // Heat capacity per unit volume.
    using VolumetricHeatCapacity = Quantity<dimension::VolumetricHeatCapacity>;
    // Conductive heat-flow coefficient of a material.
    using ThermalConductivity = Quantity<dimension::ThermalConductivity>;
    // Heat-transfer coefficient across a surface.
    using HeatTransferCoefficient = Quantity<dimension::HeatTransferCoefficient>;
    // Thermal conductance of a body or interface.
    using ThermalConductance = Quantity<dimension::ThermalConductance>;
    // Resistance of a body or interface to heat flow.
    using ThermalResistance = Quantity<dimension::ThermalResistance>;
    // Material property corresponding to thermal resistance per geometry.
    using ThermalResistivity = Quantity<dimension::ThermalResistivity>;

    // Rate of flow of electric charge.
    using ElectricCurrent = Quantity<dimension::ElectricCurrent>;
    // Magnetomotive force around a magnetic circuit.
    using MagnetomotiveForce = Quantity<dimension::ElectricCurrent>;
    // Electric charge.
    using ElectricCharge = Quantity<dimension::ElectricCharge>;
    // Electric current per unit cross-sectional area.
    using CurrentDensity = Quantity<dimension::CurrentDensity>;
    // Electric charge per unit length.
    using LinearChargeDensity = Quantity<dimension::LinearChargeDensity>;
    // Electric charge per unit area.
    using SurfaceChargeDensity = Quantity<dimension::ElectricDisplacementField>;
    // Electric displacement field.
    using ElectricDisplacementField = Quantity<dimension::ElectricDisplacementField>;
    // Electric polarization density.
    using Polarization = Quantity<dimension::ElectricDisplacementField>;
    // Electric charge per unit volume.
    using ElectricChargeDensity = Quantity<dimension::ElectricChargeDensity>;
    // Electric charge multiplied by separation distance.
    using ElectricDipoleMoment = Quantity<dimension::ElectricDipoleMoment>;
    // Force per unit electric charge.
    using ElectricFieldStrength = Quantity<dimension::ElectricFieldStrength>;
    // Energy per unit electric charge.
    using ElectricPotential = Quantity<dimension::ElectricPotential>;
    // Electric potential difference between two points.
    using Voltage = Quantity<dimension::ElectricPotential, Reference::difference>;
    // Source-generated electric potential difference.
    using ElectromotiveForce = Quantity<dimension::ElectricPotential, Reference::difference>;
    // Opposition to direct electric current.
    using ElectricalResistance = Quantity<dimension::ElectricalResistance>;
    // Opposition to alternating electric current.
    using Impedance = Quantity<dimension::ElectricalResistance>;
    // Reactive component of impedance.
    using Reactance = Quantity<dimension::ElectricalResistance>;
    // Resistance normalized by conductor geometry.
    using ElectricalResistivity = Quantity<dimension::ElectricalResistivity>;
    // Reciprocal of electrical resistance.
    using ElectricalConductance = Quantity<dimension::ElectricalConductance>;
    // Reciprocal of impedance.
    using Admittance = Quantity<dimension::ElectricalConductance>;
    // Reciprocal of electrical resistivity.
    using ElectricalConductivity = Quantity<dimension::ElectricalConductivity>;
    // Charge stored per unit electric potential.
    using Capacitance = Quantity<dimension::Capacitance>;
    // Electric displacement response to electric field.
    using Permittivity = Quantity<dimension::Permittivity>;

    // Magnetic dipole moment per unit volume.
    using Magnetization = Quantity<dimension::Magnetization>;
    // Magnetic field strength H.
    using MagneticFieldStrength = Quantity<dimension::MagneticFieldStrength>;
    // Magnetic dipole moment.
    using MagneticMoment = Quantity<dimension::MagneticMoment>;
    // Surface integral of magnetic flux density.
    using MagneticFlux = Quantity<dimension::MagneticFlux>;
    // Magnetic flux per unit area.
    using MagneticFluxDensity = Quantity<dimension::MagneticFluxDensity>;
    // Magnetic flux linkage per unit current.
    using Inductance = Quantity<dimension::Inductance>;
    // Magnetic response coefficient relating B and H.
    using Permeability = Quantity<dimension::Permeability>;
    // Opposition of a magnetic circuit to magnetic flux.
    using Reluctance = Quantity<dimension::Reluctance>;

    // Amount of substance measured in moles.
    using AmountOfSubstance =
        Quantity<dimension::AmountOfSubstance, Reference::zero, Domain::non_negative>;
    // Amount of substance per unit volume.
    using MolarConcentration =
        Quantity<dimension::MolarConcentration, Reference::zero, Domain::non_negative>;
    // Volume per unit amount of substance.
    using MolarVolume = Quantity<dimension::MolarVolume, Reference::zero, Domain::non_negative>;
    // Reaction extent change per unit volume and time.
    using ReactionRate = Quantity<dimension::ReactionRate>;
    // Catalytic activity per unit volume.
    using CatalyticActivityConcentration =
        Quantity<dimension::ReactionRate, Reference::zero, Domain::non_negative>;
    // Amount of substance transformed per unit time by a catalyst.
    using CatalyticActivity =
        Quantity<dimension::CatalyticActivity, Reference::zero, Domain::non_negative>;

    // Photometric luminous intensity of a source.
    using LuminousIntensity =
        Quantity<dimension::LuminousIntensity, Reference::zero, Domain::non_negative>;
    // Photometric power weighted by human visual response.
    using LuminousFlux = Quantity<dimension::LuminousFlux, Reference::zero, Domain::non_negative>;
    // Luminous flux incident per unit area.
    using Illuminance = Quantity<dimension::Illuminance, Reference::zero, Domain::non_negative>;
    // Luminous flux produced per unit radiant power.
    using LuminousEfficacy =
        Quantity<dimension::LuminousEfficacy, Reference::zero, Domain::non_negative>;
    // Luminous intensity per projected unit area.
    using Luminance = Quantity<dimension::Luminance, Reference::zero, Domain::non_negative>;

    // Change in angular position.
    using AngularDisplacement = Quantity<dimension::Angle, Reference::difference>;
    // Phase of a periodic quantity.
    using PhaseAngle = Quantity<dimension::Angle>;
    // Angular rate of a periodic process.
    using AngularFrequency =
        Quantity<dimension::AngularVelocity, Reference::zero, Domain::non_negative>;
    // Angular spatial frequency.
    using AngularWavenumber = Quantity<dimension::Wavevector>;
    // Time integral of torque.
    using AngularImpulse = Quantity<dimension::AngularMomentum, Reference::difference>;
    // Rate of change of angular acceleration.
    using AngularJerk = Quantity<dimension::AngularJerk>;
}
