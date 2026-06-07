ALTER TABLE sensor_readings
ADD CONSTRAINT fk_sensor_readings_device
FOREIGN KEY (device_id)
REFERENCES devices(device_id);