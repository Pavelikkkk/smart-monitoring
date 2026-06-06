CREATE TABLE IF NOT EXISTS rooms
(
    id                  SERIAL PRIMARY KEY,

    room_name           TEXT NOT NULL,

    room_type           TEXT NOT NULL,

    floor_number        INTEGER NOT NULL,

    building_id         INTEGER NOT NULL
        REFERENCES buildings(id)
        ON DELETE CASCADE,

    created_at          TIMESTAMPTZ NOT NULL DEFAULT NOW()
);