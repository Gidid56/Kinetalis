#pragma once
#include "utility/color.h"

#include <cstdint>
#include <string_view>
#include <utility>

namespace chemistry::element
{
    // Element is a one-byte enumeration whose integer value corresponds
    // to the proton number for the 118 chemical elements. Element::none
    // is the zero-valued sentinel used to represent a vacant or unassigned site.
    //
    // The element data are generated from data/elements.xlsx.
    // Colors follow the Jmol color scheme where values are available.
    //
    // The binary metal/nonmetal classification is intentionally simplified.
    // Metalloids do not form a separate category; B, Si, As, Te, and At are
    // grouped with the nonmetals, while Ge, Sb, and Po are grouped with the metals.
    //
    // NOLINTBEGIN(readability-identifier-naming)
    enum class Element : std::uint8_t
    {
        none = 0,
        H = 1,
        He,
        Li,
        Be,
        B,
        C,
        N,
        O,
        F,
        Ne,
        Na,
        Mg,
        Al,
        Si,
        P,
        S,
        Cl,
        Ar,
        K,
        Ca,
        Sc,
        Ti,
        V,
        Cr,
        Mn,
        Fe,
        Co,
        Ni,
        Cu,
        Zn,
        Ga,
        Ge,
        As,
        Se,
        Br,
        Kr,
        Rb,
        Sr,
        Y,
        Zr,
        Nb,
        Mo,
        Tc,
        Ru,
        Rh,
        Pd,
        Ag,
        Cd,
        In,
        Sn,
        Sb,
        Te,
        I,
        Xe,
        Cs,
        Ba,
        La,
        Ce,
        Pr,
        Nd,
        Pm,
        Sm,
        Eu,
        Gd,
        Tb,
        Dy,
        Ho,
        Er,
        Tm,
        Yb,
        Lu,
        Hf,
        Ta,
        W,
        Re,
        Os,
        Ir,
        Pt,
        Au,
        Hg,
        Tl,
        Pb,
        Bi,
        Po,
        At,
        Rn,
        Fr,
        Ra,
        Ac,
        Th,
        Pa,
        U,
        Np,
        Pu,
        Am,
        Cm,
        Bk,
        Cf,
        Es,
        Fm,
        Md,
        No,
        Lr,
        Rf,
        Db,
        Sg,
        Bh,
        Hs,
        Mt,
        Ds,
        Rg,
        Cn,
        Nh,
        Fl,
        Mc,
        Lv,
        Ts,
        Og,
    };
    // NOLINTEND(readability-identifier-naming)
    using enum Element;

    constexpr bool is_valid(Element element)
    {
        const auto value{ std::to_underlying(element) };

        return value >= std::to_underlying(H) and value <= std::to_underlying(Og);
    }
    constexpr bool is_noble_gas(Element element)
    {
        switch (element)
        {
            case He:
            case Ne:
            case Ar:
            case Kr:
            case Xe:
            case Rn:
            case Og: return true;
            default: return false;
        }
    }
    constexpr bool is_non_metal(Element element)
    {
        switch (element)
        {
            case H:
            case B:
            case C:
            case N:
            case O:
            case F:
            case Si:
            case P:
            case S:
            case Cl:
            case As:
            case Se:
            case Br:
            case Te:
            case I:
            case At:
            case Ts: return true;
            default: return is_noble_gas(element);
        }
    }
    constexpr bool is_metal(Element element)
    {
        return is_valid(element) and not is_non_metal(element);
    }

    constexpr std::string_view symbol(Element element)
    {
        switch (element)
        {
            case none: return "None";

            case H: return "H";
            case He: return "He";
            case Li: return "Li";
            case Be: return "Be";
            case B: return "B";
            case C: return "C";
            case N: return "N";
            case O: return "O";
            case F: return "F";
            case Ne: return "Ne";
            case Na: return "Na";
            case Mg: return "Mg";
            case Al: return "Al";
            case Si: return "Si";
            case P: return "P";
            case S: return "S";
            case Cl: return "Cl";
            case Ar: return "Ar";
            case K: return "K";
            case Ca: return "Ca";
            case Sc: return "Sc";
            case Ti: return "Ti";
            case V: return "V";
            case Cr: return "Cr";
            case Mn: return "Mn";
            case Fe: return "Fe";
            case Co: return "Co";
            case Ni: return "Ni";
            case Cu: return "Cu";
            case Zn: return "Zn";
            case Ga: return "Ga";
            case Ge: return "Ge";
            case As: return "As";
            case Se: return "Se";
            case Br: return "Br";
            case Kr: return "Kr";
            case Rb: return "Rb";
            case Sr: return "Sr";
            case Y: return "Y";
            case Zr: return "Zr";
            case Nb: return "Nb";
            case Mo: return "Mo";
            case Tc: return "Tc";
            case Ru: return "Ru";
            case Rh: return "Rh";
            case Pd: return "Pd";
            case Ag: return "Ag";
            case Cd: return "Cd";
            case In: return "In";
            case Sn: return "Sn";
            case Sb: return "Sb";
            case Te: return "Te";
            case I: return "I";
            case Xe: return "Xe";
            case Cs: return "Cs";
            case Ba: return "Ba";
            case La: return "La";
            case Ce: return "Ce";
            case Pr: return "Pr";
            case Nd: return "Nd";
            case Pm: return "Pm";
            case Sm: return "Sm";
            case Eu: return "Eu";
            case Gd: return "Gd";
            case Tb: return "Tb";
            case Dy: return "Dy";
            case Ho: return "Ho";
            case Er: return "Er";
            case Tm: return "Tm";
            case Yb: return "Yb";
            case Lu: return "Lu";
            case Hf: return "Hf";
            case Ta: return "Ta";
            case W: return "W";
            case Re: return "Re";
            case Os: return "Os";
            case Ir: return "Ir";
            case Pt: return "Pt";
            case Au: return "Au";
            case Hg: return "Hg";
            case Tl: return "Tl";
            case Pb: return "Pb";
            case Bi: return "Bi";
            case Po: return "Po";
            case At: return "At";
            case Rn: return "Rn";
            case Fr: return "Fr";
            case Ra: return "Ra";
            case Ac: return "Ac";
            case Th: return "Th";
            case Pa: return "Pa";
            case U: return "U";
            case Np: return "Np";
            case Pu: return "Pu";
            case Am: return "Am";
            case Cm: return "Cm";
            case Bk: return "Bk";
            case Cf: return "Cf";
            case Es: return "Es";
            case Fm: return "Fm";
            case Md: return "Md";
            case No: return "No";
            case Lr: return "Lr";
            case Rf: return "Rf";
            case Db: return "Db";
            case Sg: return "Sg";
            case Bh: return "Bh";
            case Hs: return "Hs";
            case Mt: return "Mt";
            case Ds: return "Ds";
            case Rg: return "Rg";
            case Cn: return "Cn";
            case Nh: return "Nh";
            case Fl: return "Fl";
            case Mc: return "Mc";
            case Lv: return "Lv";
            case Ts: return "Ts";
            case Og: return "Og";

            default: return "Invalid";
        }
    }
    constexpr std::string_view name(Element element)
    {
        switch (element)
        {
            case H: return "Hydrogen";
            case He: return "Helium";
            case Li: return "Lithium";
            case Be: return "Beryllium";
            case B: return "Boron";
            case C: return "Carbon";
            case N: return "Nitrogen";
            case O: return "Oxygen";
            case F: return "Fluorine";
            case Ne: return "Neon";
            case Na: return "Sodium";
            case Mg: return "Magnesium";
            case Al: return "Aluminium";
            case Si: return "Silicon";
            case P: return "Phosphorus";
            case S: return "Sulfur";
            case Cl: return "Chlorine";
            case Ar: return "Argon";
            case K: return "Potassium";
            case Ca: return "Calcium";
            case Sc: return "Scandium";
            case Ti: return "Titanium";
            case V: return "Vanadium";
            case Cr: return "Chromium";
            case Mn: return "Manganese";
            case Fe: return "Iron";
            case Co: return "Cobalt";
            case Ni: return "Nickel";
            case Cu: return "Copper";
            case Zn: return "Zinc";
            case Ga: return "Gallium";
            case Ge: return "Germanium";
            case As: return "Arsenic";
            case Se: return "Selenium";
            case Br: return "Bromine";
            case Kr: return "Krypton";
            case Rb: return "Rubidium";
            case Sr: return "Strontium";
            case Y: return "Yttrium";
            case Zr: return "Zirconium";
            case Nb: return "Niobium";
            case Mo: return "Molybdenum";
            case Tc: return "Technetium";
            case Ru: return "Ruthenium";
            case Rh: return "Rhodium";
            case Pd: return "Palladium";
            case Ag: return "Silver";
            case Cd: return "Cadmium";
            case In: return "Indium";
            case Sn: return "Tin";
            case Sb: return "Antimony";
            case Te: return "Tellurium";
            case I: return "Iodine";
            case Xe: return "Xenon";
            case Cs: return "Caesium";
            case Ba: return "Barium";
            case La: return "Lanthanum";
            case Ce: return "Cerium";
            case Pr: return "Praseodymium";
            case Nd: return "Neodymium";
            case Pm: return "Promethium";
            case Sm: return "Samarium";
            case Eu: return "Europium";
            case Gd: return "Gadolinium";
            case Tb: return "Terbium";
            case Dy: return "Dysprosium";
            case Ho: return "Holmium";
            case Er: return "Erbium";
            case Tm: return "Thulium";
            case Yb: return "Ytterbium";
            case Lu: return "Lutetium";
            case Hf: return "Hafnium";
            case Ta: return "Tantalum";
            case W: return "Tungsten";
            case Re: return "Rhenium";
            case Os: return "Osmium";
            case Ir: return "Iridium";
            case Pt: return "Platinum";
            case Au: return "Gold";
            case Hg: return "Mercury";
            case Tl: return "Thallium";
            case Pb: return "Lead";
            case Bi: return "Bismuth";
            case Po: return "Polonium";
            case At: return "Astatine";
            case Rn: return "Radon";
            case Fr: return "Francium";
            case Ra: return "Radium";
            case Ac: return "Actinium";
            case Th: return "Thorium";
            case Pa: return "Protactinium";
            case U: return "Uranium";
            case Np: return "Neptunium";
            case Pu: return "Plutonium";
            case Am: return "Americium";
            case Cm: return "Curium";
            case Bk: return "Berkelium";
            case Cf: return "Californium";
            case Es: return "Einsteinium";
            case Fm: return "Fermium";
            case Md: return "Mendelevium";
            case No: return "Nobelium";
            case Lr: return "Lawrencium";
            case Rf: return "Rutherfordium";
            case Db: return "Dubnium";
            case Sg: return "Seaborgium";
            case Bh: return "Bohrium";
            case Hs: return "Hassium";
            case Mt: return "Meitnerium";
            case Ds: return "Darmstadtium";
            case Rg: return "Roentgenium";
            case Cn: return "Copernicium";
            case Nh: return "Nihonium";
            case Fl: return "Flerovium";
            case Mc: return "Moscovium";
            case Lv: return "Livermorium";
            case Ts: return "Tennessine";
            case Og: return "Oganesson";
            case none: return "no element";
            default: return "invalid element";
        }
    }
    constexpr utility::Color color(Element element)
    {
        using utility::Color;
        switch (element)
        {
            case none: return Color{ 0, 0, 0 };
            case H: return Color{ 255, 255, 255 };
            case He: return Color{ 217, 255, 255 };
            case Li: return Color{ 204, 128, 255 };
            case Be: return Color{ 194, 255, 0 };
            case B: return Color{ 255, 181, 181 };
            case C: return Color{ 144, 144, 144 };
            case N: return Color{ 48, 80, 248 };
            case O: return Color{ 255, 13, 13 };
            case F: return Color{ 144, 224, 80 };
            case Ne: return Color{ 179, 227, 245 };
            case Na: return Color{ 171, 92, 242 };
            case Mg: return Color{ 138, 255, 0 };
            case Al: return Color{ 191, 166, 166 };
            case Si: return Color{ 240, 200, 160 };
            case P: return Color{ 255, 128, 0 };
            case S: return Color{ 255, 255, 48 };
            case Cl: return Color{ 31, 240, 31 };
            case Ar: return Color{ 128, 209, 227 };
            case K: return Color{ 143, 64, 212 };
            case Ca: return Color{ 61, 255, 0 };
            case Sc: return Color{ 230, 230, 230 };
            case Ti: return Color{ 191, 194, 199 };
            case V: return Color{ 166, 166, 171 };
            case Cr: return Color{ 138, 153, 199 };
            case Mn: return Color{ 156, 122, 199 };
            case Fe: return Color{ 224, 102, 51 };
            case Co: return Color{ 240, 144, 160 };
            case Ni: return Color{ 80, 208, 80 };
            case Cu: return Color{ 200, 128, 51 };
            case Zn: return Color{ 125, 128, 176 };
            case Ga: return Color{ 194, 143, 143 };
            case Ge: return Color{ 102, 143, 143 };
            case As: return Color{ 189, 128, 227 };
            case Se: return Color{ 255, 161, 0 };
            case Br: return Color{ 166, 41, 41 };
            case Kr: return Color{ 92, 184, 209 };
            case Rb: return Color{ 112, 46, 176 };
            case Sr: return Color{ 0, 255, 0 };
            case Y: return Color{ 148, 255, 255 };
            case Zr: return Color{ 148, 224, 224 };
            case Nb: return Color{ 115, 194, 201 };
            case Mo: return Color{ 84, 181, 181 };
            case Tc: return Color{ 59, 158, 158 };
            case Ru: return Color{ 36, 143, 143 };
            case Rh: return Color{ 10, 125, 140 };
            case Pd: return Color{ 0, 105, 133 };
            case Ag: return Color{ 192, 192, 192 };
            case Cd: return Color{ 255, 217, 143 };
            case In: return Color{ 166, 117, 115 };
            case Sn: return Color{ 102, 128, 128 };
            case Sb: return Color{ 158, 99, 181 };
            case Te: return Color{ 212, 122, 0 };
            case I: return Color{ 148, 0, 148 };
            case Xe: return Color{ 66, 158, 176 };
            case Cs: return Color{ 87, 23, 143 };
            case Ba: return Color{ 0, 201, 0 };
            case La: return Color{ 112, 212, 255 };
            case Ce: return Color{ 255, 255, 199 };
            case Pr: return Color{ 217, 255, 199 };
            case Nd: return Color{ 199, 255, 199 };
            case Pm: return Color{ 163, 255, 199 };
            case Sm: return Color{ 143, 255, 199 };
            case Eu: return Color{ 97, 255, 199 };
            case Gd: return Color{ 69, 255, 199 };
            case Tb: return Color{ 48, 255, 199 };
            case Dy: return Color{ 31, 255, 199 };
            case Ho: return Color{ 0, 255, 156 };
            case Er: return Color{ 0, 230, 117 };
            case Tm: return Color{ 0, 212, 82 };
            case Yb: return Color{ 0, 191, 56 };
            case Lu: return Color{ 0, 171, 36 };
            case Hf: return Color{ 77, 194, 255 };
            case Ta: return Color{ 77, 166, 255 };
            case W: return Color{ 33, 148, 214 };
            case Re: return Color{ 38, 125, 171 };
            case Os: return Color{ 38, 102, 150 };
            case Ir: return Color{ 23, 84, 135 };
            case Pt: return Color{ 208, 208, 224 };
            case Au: return Color{ 255, 209, 35 };
            case Hg: return Color{ 184, 184, 208 };
            case Tl: return Color{ 166, 84, 77 };
            case Pb: return Color{ 87, 89, 97 };
            case Bi: return Color{ 158, 79, 181 };
            case Po: return Color{ 171, 92, 0 };
            case At: return Color{ 117, 79, 69 };
            case Rn: return Color{ 66, 130, 150 };
            case Fr: return Color{ 66, 0, 102 };
            case Ra: return Color{ 0, 125, 0 };
            case Ac: return Color{ 112, 171, 250 };
            case Th: return Color{ 0, 186, 255 };
            case Pa: return Color{ 0, 161, 255 };
            case U: return Color{ 0, 143, 255 };
            case Np: return Color{ 0, 128, 255 };
            case Pu: return Color{ 0, 107, 255 };
            case Am: return Color{ 84, 92, 242 };
            case Cm: return Color{ 120, 92, 227 };
            case Bk: return Color{ 138, 79, 227 };
            case Cf: return Color{ 161, 54, 212 };
            case Es: return Color{ 179, 31, 212 };
            case Fm: return Color{ 179, 31, 186 };
            case Md: return Color{ 179, 13, 166 };
            case No: return Color{ 189, 13, 135 };
            case Lr: return Color{ 199, 0, 102 };
            case Rf: return Color{ 204, 0, 89 };
            case Db: return Color{ 209, 0, 79 };
            case Sg: return Color{ 217, 0, 69 };
            case Bh: return Color{ 224, 0, 56 };
            case Hs: return Color{ 230, 0, 46 };
            case Mt: return Color{ 235, 0, 38 };
            default: return Color{ 255, 0, 255 };
        }
    }
}

namespace chemistry { using element::Element; }

template <>
struct std::formatter<chemistry::Element> : public std::formatter<std::string_view>
{
protected:
    enum class Format
    {
        symbol,
        name,
    } format_{ Format::symbol };

public:
    constexpr auto format() const { return format_; }

    template <class ParseContext>
    constexpr auto parse(ParseContext& context)
    {
        for (auto digit = std::begin(context); digit != std::end(context); ++digit)
        {
            switch (*digit)
            {
                case 's': format_ = Format::symbol; break;
                case 'l': format_ = Format::name; break;
                case '}': return digit;
            }
        }
        return std::end(context);
    }

    template <class FormatContext>
    auto format(chemistry::Element element, FormatContext& context) const
    {
        switch (format_)
        {
            default:
            case Format::symbol: return std::format_to(context.out(), "{}", symbol(element));

            case Format::name: return std::format_to(context.out(), "{}", name(element));
        }
    }
};