CREATE TABLE users
(
    id SERIAL PRIMARY KEY,

    username TEXT NOT NULL UNIQUE,

    email TEXT NOT NULL UNIQUE,

    password_hash TEXT NOT NULL,

    first_name TEXT,

    last_name TEXT,

    role TEXT NOT NULL DEFAULT 'USER',

    telegram_chat_id TEXT,

    preferred_notification_channel TEXT DEFAULT 'TELEGRAM',

    is_active BOOLEAN NOT NULL DEFAULT TRUE,

    last_login_at TIMESTAMPTZ,

    created_at TIMESTAMPTZ NOT NULL DEFAULT NOW(),

    updated_at TIMESTAMPTZ NOT NULL DEFAULT NOW()
);