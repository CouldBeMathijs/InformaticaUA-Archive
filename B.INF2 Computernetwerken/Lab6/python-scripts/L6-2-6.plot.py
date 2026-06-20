import numpy as np
import matplotlib.pyplot as plt

num_runs   = 5
max_stas   = 100
t_hat      = 0.1          # 100ms default interval
tau        = 0.001444     # gemeten transmissieduur (seconden)
p_bits     = 1064 * 8     # 8512 bits

# Lees simulatieresultaten
pure = np.zeros(10)
for run in range(1, num_runs + 1):
    df_pure = np.genfromtxt(f"aloha-pure-{run}.dat", names=None)
    pure += df_pure[:, 1] / num_runs

num_stas = np.array(range(1, max_stas + 1, 10))

# Genormaliseerde load en throughput
G_sim = num_stas * tau / t_hat
T_sim = pure * 1e6 * tau / p_bits

# Analytische curve: uitgebreid tot G=2 zodat maximum zichtbaar is
G_ana = np.linspace(0, 2.0, 500)
T_ana = G_ana * np.exp(-2 * G_ana)

# Maximum van analytische curve
G_max = 0.5
T_max = 0.5 * np.exp(-1)  # = 1/(2e) ≈ 0.184

print(f"G_sim range: {G_sim.min():.4f} to {G_sim.max():.4f}")
print(f"T_sim range: {T_sim.min():.4f} to {T_sim.max():.4f}")
print(f"Analytical maximum: T={T_max:.4f} at G={G_max}")

plt.figure(figsize=[6.0, 4.5])
plt.plot(G_sim, T_sim, label='sim pure Aloha', color='g', marker='+', linestyle='None', markersize=8)
plt.plot(G_ana, T_ana, label='analytical T=G·e^(−2G)', color='b')
plt.axvline(x=G_max, color='r', linestyle='--', alpha=0.5, label=f'G={G_max} (theoretical max)')
plt.axhline(y=T_max, color='r', linestyle=':', alpha=0.5, label=f'T={T_max:.3f} (theoretical max)')
plt.xlim(0, 2.0)
plt.ylim(0, 0.25)
plt.xlabel("Normalized load G")
plt.ylabel("Normalized throughput T")
plt.grid()
plt.legend(loc="upper right", fontsize=8)
plt.tight_layout()
plt.savefig("throughput_normalized.png", dpi=200)
print("Saved throughput_normalized.png")
