# compare.py
# Computes same integrals as in main.cc, Task B, but using `scipy.integrate.quad` for comparison

import numpy as np
import scipy.integrate as integrate

# Compute ∫ 1/sqrt(x) on [0, 1]
res_1, _, info_1 = integrate.quad(lambda x: 1.0 / np.sqrt(x), 0, 1, full_output=1)

# Compute ∫ ln(x)/sqrt(x) on [0, 1]
res_2, _, info_2 = integrate.quad(lambda x: np.log(x) / np.sqrt(x), 0, 1, full_output=1)

# Compute ∫ exp(-x^2) on (-inf, +inf)
res_3, _, info_3 = integrate.quad(lambda x: np.exp(-x**2), -np.inf, np.inf, full_output=1)

# Write results cleanly to a file for C++ to parse
with open("python_results.txt", "w") as f:
    # Format: value error_approx evaluations
    f.write(f"{res_1} {abs(res_1 - 2.0)} {info_1['neval']}\n")
    f.write(f"{res_2} {abs(res_2 - (-4.0))} {info_2['neval']}\n")
    f.write(f"{res_3} {abs(res_3 - np.sqrt(np.pi))} {info_3['neval']}\n")
