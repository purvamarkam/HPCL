import matplotlib.pyplot as plt

# Example data (replace with your timings!)
# Format: {processes: execution_time}
times = {
    1: 10.0,
    2: 5.4,
    4: 2.9,
    8: 1.8
}

# Serial time (with 1 process)
T_serial = times[1]

processes = list(times.keys())
execution_time = [times[p] for p in processes]

# Calculate speedup and efficiency
speedup = [T_serial / t for t in execution_time]
efficiency = [s / p for s, p in zip(speedup, processes)]

# ---- Plot Execution Time ----
plt.figure(figsize=(8,5))
plt.plot(processes, execution_time, marker='o', label="Execution Time")
plt.xlabel("Number of Processes")
plt.ylabel("Execution Time (sec)")
plt.title("Execution Time vs Processes (Matrix-Matrix Multiplication)")
plt.grid(True)
plt.legend()
plt.show()

# ---- Plot Speedup ----
plt.figure(figsize=(8,5))
plt.plot(processes, speedup, marker='o', label="Actual Speedup")
plt.plot(processes, processes, linestyle='--', label="Ideal Speedup") # linear
plt.xlabel("Number of Processes")
plt.ylabel("Speedup")
plt.title("Speedup vs Processes")
plt.grid(True)
plt.legend()
plt.show()

# ---- Plot Efficiency ----
plt.figure(figsize=(8,5))
plt.plot(processes, efficiency, marker='o', label="Efficiency")
plt.xlabel("Number of Processes")
plt.ylabel("Efficiency")
plt.title("Efficiency vs Processes")
plt.grid(True)
plt.legend()
plt.show()