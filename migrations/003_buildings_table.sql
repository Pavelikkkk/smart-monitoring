CREATE TABLE IF NOT EXISTS buildings
(
    id              SERIAL PRIMARY KEY,

    name            TEXT NOT NULL,

    address         TEXT,

    description     TEXT,

    created_at      TIMESTAMPTZ NOT NULL DEFAULT NOW()
);