CREATE TABLE subscriptions
(
    id                  SERIAL PRIMARY KEY,

    organization_id     INTEGER NOT NULL,

    plan                TEXT NOT NULL,

    status              TEXT NOT NULL DEFAULT 'ACTIVE',

    started_at          TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    expires_at          TIMESTAMPTZ,

    created_at          TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    CONSTRAINT fk_subscription_org
        FOREIGN KEY (organization_id)
        REFERENCES organizations(id)
);