constexpr auto operator""_s(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_s(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ys(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ys(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Es(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Es(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Ps(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Ps(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Ts(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Ts(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Ms(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Ms(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_ks(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_ks(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_das(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_das(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_ds(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_ds(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_ms(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_ms(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_us(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_us(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_ns(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_ns(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_ps(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_ps(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_as(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_as(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ys(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ys(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qs(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qs(long double value) {
    return detail::literal<static_cast<::physics::Unit>(1), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_m(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_m(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ym(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ym(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Em(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Em(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_km(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_km(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dam(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dam(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_um(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_um(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_am(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_am(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ym(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ym(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(2), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_g(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_g(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Yg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Yg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Eg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Eg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dag(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dag(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ug(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ug(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_ng(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_ng(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_ag(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_ag(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(3), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_A(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_A(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qA(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qA(long double value) {
    return detail::literal<static_cast<::physics::Unit>(4), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_K(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_K(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qK(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qK(long double value) {
    return detail::literal<static_cast<::physics::Unit>(5), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_mol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_mol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ymol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ymol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Emol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Emol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_damol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_damol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_umol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_umol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_amol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_amol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ymol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ymol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qmol(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qmol(long double value) {
    return detail::literal<static_cast<::physics::Unit>(6), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_cd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_cd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ycd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ycd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Ecd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Ecd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dacd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dacd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_ccd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_ccd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ucd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ucd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_ncd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_ncd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_acd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_acd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ycd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ycd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qcd(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qcd(long double value) {
    return detail::literal<static_cast<::physics::Unit>(7), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_rad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_rad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Yrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Yrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Erad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Erad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Prad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Prad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Trad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Trad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Grad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Grad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_krad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_krad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_darad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_darad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_drad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_drad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_crad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_crad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_urad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_urad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_prad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_prad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_frad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_frad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_arad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_arad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qrad(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qrad(long double value) {
    return detail::literal<static_cast<::physics::Unit>(8), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_sr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_sr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ysr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ysr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Esr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Esr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Psr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Psr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Msr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Msr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_ksr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_ksr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dasr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dasr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_csr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_csr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_msr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_msr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_usr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_usr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_psr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_psr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_asr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_asr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ysr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ysr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qsr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qsr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(9), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Hz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Hz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_THz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_THz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qHz(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qHz(long double value) {
    return detail::literal<static_cast<::physics::Unit>(10), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_N(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_N(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qN(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qN(long double value) {
    return detail::literal<static_cast<::physics::Unit>(11), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Pa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Pa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qPa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qPa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(12), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_J(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_J(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qJ(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qJ(long double value) {
    return detail::literal<static_cast<::physics::Unit>(13), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_W(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_W(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qW(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qW(long double value) {
    return detail::literal<static_cast<::physics::Unit>(14), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_C(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_C(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(15), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_V(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_V(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(16), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_F(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_F(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qF(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qF(long double value) {
    return detail::literal<static_cast<::physics::Unit>(17), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Ohm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Ohm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_ROhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_ROhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_POhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_POhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qOhm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qOhm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(18), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_S(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_S(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_ES(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_ES(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qS(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qS(long double value) {
    return detail::literal<static_cast<::physics::Unit>(19), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Wb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Wb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qWb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qWb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(20), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_T(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_T(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_ET(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_ET(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qT(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qT(long double value) {
    return detail::literal<static_cast<::physics::Unit>(21), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_H(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_H(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qH(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qH(long double value) {
    return detail::literal<static_cast<::physics::Unit>(22), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_degC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_degC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dadegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dadegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_ddegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_ddegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_udegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_udegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_ndegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_ndegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_adegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_adegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ydegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ydegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qdegC(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qdegC(long double value) {
    return detail::literal<static_cast<::physics::Unit>(23), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_lm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_lm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ylm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ylm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Elm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Elm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Plm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Plm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Glm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Glm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_klm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_klm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dalm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dalm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_clm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_clm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ulm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ulm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_plm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_plm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_flm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_flm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_alm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_alm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ylm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ylm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qlm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qlm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(24), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_lx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_lx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ylx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ylx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Elx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Elx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Plx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Plx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Glx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Glx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_klx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_klx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dalx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dalx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_clx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_clx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ulx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ulx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_plx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_plx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_flx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_flx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_alx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_alx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ylx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ylx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qlx(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qlx(long double value) {
    return detail::literal<static_cast<::physics::Unit>(25), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Bq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Bq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qBq(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qBq(long double value) {
    return detail::literal<static_cast<::physics::Unit>(26), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Gy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Gy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qGy(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qGy(long double value) {
    return detail::literal<static_cast<::physics::Unit>(27), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Sv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Sv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_ESv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_ESv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ySv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ySv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qSv(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qSv(long double value) {
    return detail::literal<static_cast<::physics::Unit>(28), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_kat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_kat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ykat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ykat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Ekat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Ekat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dakat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dakat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_ckat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_ckat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ukat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ukat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_akat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_akat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ykat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ykat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qkat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qkat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(29), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_min(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(30), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_min(long double value) {
    return detail::literal<static_cast<::physics::Unit>(30), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_h(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(31), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_h(long double value) {
    return detail::literal<static_cast<::physics::Unit>(31), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_d(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(32), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_d(long double value) {
    return detail::literal<static_cast<::physics::Unit>(32), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_tr(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(33), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_tr(long double value) {
    return detail::literal<static_cast<::physics::Unit>(33), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_deg(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(34), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_deg(long double value) {
    return detail::literal<static_cast<::physics::Unit>(34), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_arcmin(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(35), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_arcmin(long double value) {
    return detail::literal<static_cast<::physics::Unit>(35), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_arcsec(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(36), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_arcsec(long double value) {
    return detail::literal<static_cast<::physics::Unit>(36), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_a(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(37), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_a(long double value) {
    return detail::literal<static_cast<::physics::Unit>(37), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_ha(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(38), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_ha(long double value) {
    return detail::literal<static_cast<::physics::Unit>(38), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_l(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_l(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Ql(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Ql(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Yl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Yl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_El(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_El(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Ml(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Ml(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_ml(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_ml(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ul(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ul(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_al(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_al(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rl(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rl(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_ql(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_ql(long double value) {
    return detail::literal<static_cast<::physics::Unit>(39), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_t(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_t(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Yt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Yt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Et(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Et(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_ht(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_ht(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dat(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dat(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_ct(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_ct(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ut(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ut(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_ft(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_ft(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_at(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_at(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qt(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qt(long double value) {
    return detail::literal<static_cast<::physics::Unit>(40), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Gal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Gal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qGal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qGal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(41), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_bar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_bar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ybar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ybar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Ebar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Ebar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dabar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dabar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ubar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ubar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_abar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_abar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ybar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ybar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qbar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qbar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(42), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_var(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_var(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Yvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Yvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Evar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Evar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_davar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_davar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_avar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_avar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qvar(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qvar(long double value) {
    return detail::literal<static_cast<::physics::Unit>(43), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_cal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_cal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Ycal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Ycal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Ecal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Ecal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dacal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dacal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_ccal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_ccal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ucal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ucal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_ncal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_ncal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_acal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_acal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_ycal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_ycal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qcal(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qcal(long double value) {
    return detail::literal<static_cast<::physics::Unit>(44), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Angstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Angstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qAngstrom(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qAngstrom(long double value) {
    return detail::literal<static_cast<::physics::Unit>(45), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_b(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_b(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Qb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_Qb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_Rb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_Rb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_Yb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_Yb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_Zb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_Zb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_Eb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_Eb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_Pb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_Pb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_Tb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_Tb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_Gb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_Gb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_Mb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_Mb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_dab(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_dab(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_db(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_db(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ub(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ub(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_ab(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_ab(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qb(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qb(long double value) {
    return detail::literal<static_cast<::physics::Unit>(46), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_Da(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Da(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qDa(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qDa(long double value) {
    return detail::literal<static_cast<::physics::Unit>(47), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_eV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_eV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_ReV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_ReV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_keV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_keV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_heV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_heV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_deV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_deV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_ceV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_ceV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_meV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_meV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_ueV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_ueV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_neV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_neV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_peV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_peV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_feV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_feV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_reV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_reV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qeV(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qeV(long double value) {
    return detail::literal<static_cast<::physics::Unit>(48), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_au(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(49), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_au(long double value) {
    return detail::literal<static_cast<::physics::Unit>(49), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_nmi(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(50), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_nmi(long double value) {
    return detail::literal<static_cast<::physics::Unit>(50), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_kn(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(51), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_kn(long double value) {
    return detail::literal<static_cast<::physics::Unit>(51), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_Np(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_Np(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_ENp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_ENp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qNp(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qNp(long double value) {
    return detail::literal<static_cast<::physics::Unit>(52), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_B(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_B(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_QB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(30)>(value);
}
constexpr auto operator""_QB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(30)>(value);
}

constexpr auto operator""_RB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(27)>(value);
}
constexpr auto operator""_RB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(27)>(value);
}

constexpr auto operator""_YB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(24)>(value);
}
constexpr auto operator""_YB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(24)>(value);
}

constexpr auto operator""_ZB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(21)>(value);
}
constexpr auto operator""_ZB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(21)>(value);
}

constexpr auto operator""_EB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(18)>(value);
}
constexpr auto operator""_EB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(18)>(value);
}

constexpr auto operator""_PB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(15)>(value);
}
constexpr auto operator""_PB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(15)>(value);
}

constexpr auto operator""_TB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(12)>(value);
}
constexpr auto operator""_TB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(12)>(value);
}

constexpr auto operator""_GB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(9)>(value);
}
constexpr auto operator""_GB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(9)>(value);
}

constexpr auto operator""_MB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(6)>(value);
}
constexpr auto operator""_MB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(6)>(value);
}

constexpr auto operator""_kB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(3)>(value);
}
constexpr auto operator""_kB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(3)>(value);
}

constexpr auto operator""_hB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(2)>(value);
}
constexpr auto operator""_hB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(2)>(value);
}

constexpr auto operator""_daB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(1)>(value);
}
constexpr auto operator""_daB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(1)>(value);
}

constexpr auto operator""_dB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-1)>(value);
}
constexpr auto operator""_dB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-1)>(value);
}

constexpr auto operator""_cB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-2)>(value);
}
constexpr auto operator""_cB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-2)>(value);
}

constexpr auto operator""_mB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-3)>(value);
}
constexpr auto operator""_mB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-3)>(value);
}

constexpr auto operator""_uB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-6)>(value);
}
constexpr auto operator""_uB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-6)>(value);
}

constexpr auto operator""_nB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-9)>(value);
}
constexpr auto operator""_nB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-9)>(value);
}

constexpr auto operator""_pB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-12)>(value);
}
constexpr auto operator""_pB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-12)>(value);
}

constexpr auto operator""_fB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-15)>(value);
}
constexpr auto operator""_fB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-15)>(value);
}

constexpr auto operator""_aB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-18)>(value);
}
constexpr auto operator""_aB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-18)>(value);
}

constexpr auto operator""_zB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-21)>(value);
}
constexpr auto operator""_zB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-21)>(value);
}

constexpr auto operator""_yB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-24)>(value);
}
constexpr auto operator""_yB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-24)>(value);
}

constexpr auto operator""_rB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-27)>(value);
}
constexpr auto operator""_rB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-27)>(value);
}

constexpr auto operator""_qB(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-30)>(value);
}
constexpr auto operator""_qB(long double value) {
    return detail::literal<static_cast<::physics::Unit>(53), static_cast<::physics::Prefix>(-30)>(value);
}

constexpr auto operator""_percent(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(54), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_percent(long double value) {
    return detail::literal<static_cast<::physics::Unit>(54), static_cast<::physics::Prefix>(0)>(value);
}

constexpr auto operator""_ppm(unsigned long long value) {
    return detail::literal<static_cast<::physics::Unit>(55), static_cast<::physics::Prefix>(0)>(value);
}
constexpr auto operator""_ppm(long double value) {
    return detail::literal<static_cast<::physics::Unit>(55), static_cast<::physics::Prefix>(0)>(value);
}

