import numpy as np
import pandas as pd

import torch
import torch.nn as nn

from sklearn.preprocessing import StandardScaler

# ============================================================
# LOAD DATA
# ============================================================

print("Loading test dataset...")

df = pd.read_csv("../data/test_dataset.csv"
)

print(f"Rows: {len(df)}")

# ============================================================
# PIVOT
# ============================================================

pivot = (
    df.pivot_table(
        index=["timestamp", "device_id"],
        columns="sensor_type",
        values="value",
        aggfunc="first"
    )
    .reset_index()
)

print(f"States: {len(pivot)}")

# ============================================================
# FEATURES
# ============================================================

pivot["timestamp"] = pd.to_datetime(pivot["timestamp"]
)

pivot["hour"] = pivot["timestamp"].dt.hour

pivot["hour_sin"] = np.sin(2 * np.pi * pivot["hour"] / 24.0
)

pivot["hour_cos"] = np.cos(2 * np.pi * pivot["hour"] / 24.0
)

features = pivot[
    [
        "motion",
        "power",
        "light",
        "hour_sin",
        "hour_cos"
    ]
].fillna(0)

# ============================================================
# NORMALIZATION
# ============================================================

scaler = StandardScaler()

X = scaler.fit_transform(features)

X_tensor = torch.tensor(X, dtype=torch.float32
)

# ============================================================
# MODEL
# ============================================================

class Autoencoder(nn.Module):

    def __init__(self): 
        super().__init__()

        self.encoder = nn.Sequential(
            nn.Linear(5, 32),
            nn.ReLU(),

            nn.Linear(32, 16),
            nn.ReLU(),

            nn.Linear(16, 8)
        )

        self.decoder = nn.Sequential(
            nn.Linear(8, 16),
            nn.ReLU(),

            nn.Linear(16, 32),
            nn.ReLU(),

            nn.Linear(32, 5)
        )

    def forward(self, x):
        z = self.encoder(x)
        return self.decoder(z)

# ============================================================
# LOAD MODEL
# ============================================================

model = Autoencoder()

model.load_state_dict(torch.load("models/model.pth"))

model.eval()

# ============================================================
# RECONSTRUCTION ERROR
# ============================================================

with torch.no_grad():

    reconstructed = model(X_tensor)

    errors = torch.mean((X_tensor - reconstructed) ** 2,dim=1)

print()
print("Error statistics")
print("----------------")

print("Mean:",errors.mean().item()
)

print("Std:",errors.std().item()
)

print("Min:",errors.min().item()
)

print("Max:",errors.max().item()
)

# ============================================================
# THRESHOLD
# ============================================================

with open("models/threshold.txt"
) as f:

    threshold = float(f.read())

print()
print("Threshold:", threshold)

anomalies = (errors > threshold)

count = anomalies.sum().item()

print()
print(f"Detected anomalies: "f"{count}")

print(f"Percentage: "f"{100 * count / len(errors):.2f}%")