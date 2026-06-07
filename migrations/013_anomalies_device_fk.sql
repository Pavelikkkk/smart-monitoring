ALTER TABLE anomalies
ADD CONSTRAINT fk_anomalies_device
FOREIGN KEY (device_id)
REFERENCES devices(device_id);