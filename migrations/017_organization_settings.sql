CREATE TABLE organization_settings
(
    id                  SERIAL PRIMARY KEY,

    organization_id     INTEGER NOT NULL UNIQUE,

    anomaly_threshold   DOUBLE PRECISION DEFAULT 0.8,

    notifications_enabled BOOLEAN DEFAULT TRUE,

    created_at          TIMESTAMPTZ DEFAULT NOW(),

    CONSTRAINT fk_org_settings
        FOREIGN KEY (organization_id)
        REFERENCES organizations(id)
);