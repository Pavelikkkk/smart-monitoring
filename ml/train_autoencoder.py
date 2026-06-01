import numpy as np
import pandas as pd
import json

from sklearn.preprocessing import StandardScaler

import torch
import torch.nn as nn

# ============================================================
# LOAD DATA
# ============================================================

print("Loading dataset...")

df = pd.read_csv("../data/training_dataset.csv")

print(f"Rows: {len(df)}")

# ============================================================
# PIVOT TO ROOM STATE
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
# FEATURE ENGINEERING
# ============================================================

pivot["timestamp"] = pd.to_datetime(
    pivot["timestamp"]
)

pivot["hour"] = pivot["timestamp"].dt.hour

# cyclic time encoding

pivot["hour_sin"] = np.sin(
    2 * np.pi * pivot["hour"] / 24.0
)

pivot["hour_cos"] = np.cos(
    2 * np.pi * pivot["hour"] / 24.0
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

X_tensor = torch.tensor(
    X,
    dtype=torch.float32
)

print(features.describe())

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
        latent = self.encoder(x)
        reconstructed = self.decoder(latent)
        return reconstructed


model = Autoencoder()

criterion = nn.MSELoss()

optimizer = torch.optim.Adam(
    model.parameters(),
    lr=0.001
)

# ============================================================
# TRAINING
# ============================================================

epochs = 100

print("Training...")

for epoch in range(epochs):

    optimizer.zero_grad()

    reconstructed = model(X_tensor)

    loss = criterion(
        reconstructed,
        X_tensor
    )

    loss.backward()

    optimizer.step()

    if epoch % 5 == 0:
        print(
            f"Epoch {epoch} "
            f"Loss={loss.item():.6f}"
        )

print("Training finished")

torch.save(
    model.state_dict(),
    "models/model.pth"
)

scaler_data = {
    "mean": scaler.mean_.tolist(),
    "scale": scaler.scale_.tolist()
}

with open(
    "models/scaler.json",
    "w"
) as file:
    json.dump(
        scaler_data,
        file,
        indent=4
    )

# ============================================================
# THRESHOLD CALCULATION
# ============================================================

model.eval()

with torch.no_grad():

    reconstructed = model(X_tensor)

    errors = torch.mean(
        (X_tensor - reconstructed) ** 2,
        dim=1
    )

    threshold = (
        errors.mean() +
        3 * errors.std()
    )

print()
print(f"Threshold: {threshold.item()}")

# ============================================================
# SAVE THRESHOLD
# ============================================================

with open(
    "models/threshold.txt",
    "w"
) as file:

    file.write(
        str(
            threshold.item()
        )
    )

# ============================================================
# EXPORT ONNX
# ============================================================

dummy_input = torch.randn(
    1,
    5
)

torch.onnx.export(
    model,
    dummy_input,
    "models/anomaly_autoencoder.onnx",
    input_names=["input"],
    output_names=["output"],
    opset_version=18
)

print()
print("ONNX saved:")
print("models/anomaly_autoencoder.onnx")