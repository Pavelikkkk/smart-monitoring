-- ACCOUNT TYPE

ALTER TABLE users
ADD COLUMN IF NOT EXISTS account_type TEXT NOT NULL
DEFAULT 'PERSONAL';

-- SUBSCRIPTIONS

ALTER TABLE subscriptions
ALTER COLUMN organization_id DROP NOT NULL;

ALTER TABLE subscriptions
ADD COLUMN IF NOT EXISTS user_id INTEGER;

DO $$
BEGIN
    IF NOT EXISTS (
        SELECT 1
        FROM pg_constraint
        WHERE conname = 'fk_subscription_user'
    ) THEN
        ALTER TABLE subscriptions
        ADD CONSTRAINT fk_subscription_user
        FOREIGN KEY (user_id)
        REFERENCES users(id)
        ON DELETE CASCADE;
    END IF;
END $$;

ALTER TABLE subscriptions
ADD COLUMN IF NOT EXISTS max_buildings INTEGER NOT NULL DEFAULT 0;

ALTER TABLE subscriptions
ADD COLUMN IF NOT EXISTS max_rooms INTEGER NOT NULL DEFAULT 0;

ALTER TABLE subscriptions
ADD COLUMN IF NOT EXISTS max_devices INTEGER NOT NULL DEFAULT 0;
