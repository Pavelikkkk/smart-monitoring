CREATE TABLE IF NOT EXISTS notifications
(
    id                  BIGSERIAL PRIMARY KEY,

    anomaly_id          BIGINT,

    channel             TEXT NOT NULL,

    recipient           TEXT,

    status              TEXT NOT NULL,

    message             TEXT,

    sent_at             TIMESTAMPTZ NOT NULL DEFAULT NOW()
);