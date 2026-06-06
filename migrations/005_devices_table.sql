CREATE TABLE IF NOT EXISTS devices
(
    device_id           TEXT PRIMARY KEY,

    device_name         TEXT NOT NULL,

    device_model        TEXT,

    firmware_version    TEXT,

    room_id             INTEGER NOT NULL
        REFERENCES rooms(id)
        ON DELETE CASCADE,

    is_online           BOOLEAN NOT NULL DEFAULT FALSE,

    last_seen_at        TIMESTAMPTZ,

    installed_at        TIMESTAMPTZ NOT NULL DEFAULT NOW()
);