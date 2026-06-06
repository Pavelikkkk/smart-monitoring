CREATE EXTENSION IF NOT EXISTS timescaledb;

CREATE TABLE IF NOT EXISTS sensor_readings
(
    recorded_at         TIMESTAMPTZ NOT NULL,

    device_id          TEXT NOT NULL,

    sensor_type        TEXT NOT NULL,

    numeric_value      DOUBLE PRECISION,

    bool_value         BOOLEAN,

    unit               TEXT CHECK (char_length(unit) <= 20),

    PRIMARY KEY (
        recorded_at,
        device_id,
        sensor_type
    )
);

SELECT create_hypertable(
    'sensor_readings',
    'recorded_at',
    chunk_time_interval => INTERVAL '1 day',
    if_not_exists => TRUE
);

CREATE INDEX IF NOT EXISTS idx_sensor_readings_device_time
ON sensor_readings (
    device_id,
    recorded_at DESC
);

CREATE INDEX IF NOT EXISTS idx_sensor_readings_type_time
ON sensor_readings (
    sensor_type,
    recorded_at DESC
);

ALTER TABLE sensor_readings
SET (
    timescaledb.compress,
    timescaledb.compress_segmentby = 'device_id'
);

SELECT add_compression_policy(
    'sensor_readings',
    INTERVAL '7 days',
    if_not_exists => TRUE
);

SELECT add_retention_policy(
    'sensor_readings',
    INTERVAL '90 days',
    if_not_exists => TRUE
);