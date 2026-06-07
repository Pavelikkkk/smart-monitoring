ALTER TABLE anomalies
ADD CONSTRAINT fk_anomalies_ack_user
FOREIGN KEY (acknowledged_by)
REFERENCES users(id);