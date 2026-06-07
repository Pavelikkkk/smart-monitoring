CREATE TABLE organizations
(
    id SERIAL PRIMARY KEY,

    name VARCHAR(255) NOT NULL,

    type VARCHAR(50) NOT NULL,

    created_at TIMESTAMP DEFAULT NOW()
);