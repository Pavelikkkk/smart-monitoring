DO $$
DECLARE
    user_row RECORD;
    workspace_id INTEGER;
BEGIN
    FOR user_row IN
        SELECT
            id,
            username,
            COALESCE(account_type, 'PERSONAL') AS account_type
        FROM users
        WHERE organization_id IS NULL
    LOOP
        INSERT INTO organizations
        (
            name,
            type
        )
        VALUES
        (
            user_row.username || ' workspace',
            CASE
                WHEN user_row.account_type = 'BUSINESS' THEN 'BUSINESS'
                ELSE 'PERSONAL'
            END
        )
        RETURNING id INTO workspace_id;

        UPDATE users
        SET organization_id = workspace_id
        WHERE id = user_row.id;

        INSERT INTO subscriptions
        (
            organization_id,
            user_id,
            plan,
            status,
            max_buildings,
            max_rooms,
            max_devices
        )
        SELECT
            workspace_id,
            user_row.id,
            CASE
                WHEN user_row.account_type = 'BUSINESS' THEN 'BUSINESS'
                ELSE 'STANDARD'
            END,
            'ACTIVE',
            CASE
                WHEN user_row.account_type = 'BUSINESS' THEN 25
                ELSE 0
            END,
            CASE
                WHEN user_row.account_type = 'BUSINESS' THEN 250
                ELSE 5
            END,
            CASE
                WHEN user_row.account_type = 'BUSINESS' THEN 1200
                ELSE 20
            END
        WHERE NOT EXISTS (
            SELECT 1
            FROM subscriptions
            WHERE user_id = user_row.id
        );
    END LOOP;
END $$;
