CREATE TABLE IF NOT EXISTS anomalies
(
    id                  BIGSERIAL,

    recorded_at         TIMESTAMPTZ NOT NULL,

    device_id           TEXT NOT NULL,

    sensor_type         TEXT NOT NULL,

    numeric_value       DOUBLE PRECISION,

    bool_value          BOOLEAN,

    unit                TEXT,

    anomaly_type        TEXT NOT NULL,

    severity            TEXT NOT NULL,

    score               DOUBLE PRECISION DEFAULT 0,

    description         TEXT,

    detected_at         TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    acknowledged        BOOLEAN NOT NULL DEFAULT FALSE,

    acknowledged_by     INTEGER,

    acknowledged_at     TIMESTAMPTZ,

    PRIMARY KEY (
        id,
        detected_at
    )
);

CREATE INDEX IF NOT EXISTS idx_anomalies_detected_at
ON anomalies (detected_at DESC);

CREATE INDEX IF NOT EXISTS idx_anomalies_device
ON anomalies (
    device_id,
    detected_at DESC
);

CREATE INDEX IF NOT EXISTS idx_anomalies_type
ON anomalies (
    anomaly_type
);

SELECT create_hypertable(
    'anomalies',
    'detected_at',
    chunk_time_interval => INTERVAL '1 day',
    if_not_exists => TRUE
);

SELECT add_retention_policy(
    'anomalies',
    INTERVAL '180 days',
    if_not_exists => TRUE
);