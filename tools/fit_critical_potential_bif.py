import numpy as np
import pandas as pd
from firthmodels import FirthLogisticRegression

FILE = "regimes.csv"

df = pd.read_csv(FILE)

# Parse the common part of descriptions such as
# "T = 300 K;x_Au = 10;E = 0.825 V"
# and sensitivity descriptions such as
# "T = 300 K;x_Au = 10;E = 0.825 Vdissolution_Ag_9 = 0.5".
params = df["description"].str.extract(
    r"T\s*=\s*(?P<T_K>[0-9.]+)\s*K;"
    r"\s*x_Au\s*=\s*(?P<x0>[0-9.]+);"
    r"\s*E\s*=\s*(?P<E_V>[0-9.]+)\s*V"
    r"(?P<suffix>.*)$"
)

if params[["T_K", "x0", "E_V"]].isna().any(axis=None):
    bad = df.loc[params[["T_K", "x0", "E_V"]].isna().any(axis=1), "description"]
    raise ValueError(
        "Could not parse one or more descriptions:\n" + "\n".join(bad.astype(str))
    )

df[["T_K", "x0", "E_V"]] = params[["T_K", "x0", "E_V"]].astype(float)
df["x0"] /= 100.0

# Sensitivity campaigns append "<rate> = <factor>" directly after the voltage.
# Keep ordinary campaigns as a single "reference" group.
suffix = params["suffix"].str.strip()
has_sensitivity = suffix.ne("").any()

if has_sensitivity:
    sensitivity = suffix.str.extract(
        r"^(?P<rate>[A-Za-z]+_[A-Za-z]+_\d+)\s*=\s*"
        r"(?P<factor>[0-9.eE+-]+)$"
    )

    # Empty suffixes are allowed so reference and sensitivity data may coexist
    # in the same CSV.
    reference = suffix.eq("")
    invalid = ~reference & sensitivity["rate"].isna()
    if invalid.any():
        bad = df.loc[invalid, "description"]
        raise ValueError(
            "Could not parse one or more sensitivity descriptions:\n"
            + "\n".join(bad.astype(str))
        )

    df["rate"] = sensitivity["rate"].fillna("reference")
    df["factor"] = pd.to_numeric(sensitivity["factor"], errors="coerce")
    df.loc[reference, "factor"] = 1.0

    group_columns = ["rate", "factor", "T_K", "x0"]
else:
    group_columns = ["T_K", "x0"]

results = []

for key, scan in df.groupby(group_columns, sort=True):
    # Fit only classified terminal outcomes:
    # bifurcated = 1, arrested = 0. "none" is excluded.
    scan = scan[(scan["bifurcated"] + scan["arrested"]) > 0].copy()

    Eref = scan["E_V"].mean()

    X = []
    y = []

    for _, row in scan.iterrows():
        E_mV = (row["E_V"] - Eref) * 1000.0

        X.extend([[E_mV]] * int(row["bifurcated"]))
        y.extend([1] * int(row["bifurcated"]))

        X.extend([[E_mV]] * int(row["arrested"]))
        y.extend([0] * int(row["arrested"]))

    X = np.asarray(X, dtype=float)
    y = np.asarray(y, dtype=int)

    if len(np.unique(y)) < 2:
        print(f"Skipping {dict(zip(group_columns, key if isinstance(key, tuple) else (key,)))}: "
              "only one outcome class.")
        continue

    fit = FirthLogisticRegression().fit(X, y)

    beta0 = float(fit.intercept_)
    beta1 = float(fit.coef_[0])

    # logit(P_bifurcated) = beta0 + beta1 * E_mV
    Ecrit_V = Eref - (beta0 / beta1) / 1000.0
    w_V = 1.0 / beta1 / 1000.0

    if has_sensitivity:
        rate, factor, T, x0 = key
        result = {
            "rate": rate,
            "factor": factor,
            "T_K": T,
            "x0": x0,
            "Ecrit_V": Ecrit_V,
            "w_V": w_V,
        }
    else:
        T, x0 = key
        result = {
            "T_K": T,
            "x0": x0,
            "Ecrit_V": Ecrit_V,
            "w_V": w_V,
        }

    results.append(result)

results = pd.DataFrame(results).sort_values(group_columns)

print(results.to_string(index=False))
results.to_csv("critical_potential_fits.csv", index=False)
