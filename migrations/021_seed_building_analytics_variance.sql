DO $$
DECLARE
    admin_org_id INTEGER;
    north_building_id INTEGER;
    south_building_id INTEGER;
    lab_room_id INTEGER;
    kitchen_room_id INTEGER;
BEGIN
    SELECT organization_id
    INTO admin_org_id
    FROM users
    WHERE email = 'admin@dorm.energy'
    LIMIT 1;

    IF admin_org_id IS NULL THEN
        RETURN;
    END IF;

    SELECT id INTO north_building_id
    FROM buildings
    WHERE organization_id = admin_org_id
      AND name = 'North Dormitory'
    LIMIT 1;

    SELECT id INTO south_building_id
    FROM buildings
    WHERE organization_id = admin_org_id
      AND name = 'South Dormitory'
    LIMIT 1;

    SELECT id INTO lab_room_id
    FROM rooms
    WHERE building_id = north_building_id
      AND room_name = 'Study Lab'
    LIMIT 1;

    SELECT id INTO kitchen_room_id
    FROM rooms
    WHERE building_id = south_building_id
      AND room_name = 'Shared Kitchen'
    LIMIT 1;

    IF lab_room_id IS NULL OR kitchen_room_id IS NULL THEN
        RETURN;
    END IF;

    INSERT INTO devices
    (
        device_id,
        device_name,
        device_model,
        firmware_version,
        room_id,
        is_online,
        last_seen_at
    )
    VALUES
        ('admin-lab-power-02', 'Study Lab Secondary Power Meter', 'ESP32-PWR-v2', '2.1.1', lab_room_id, TRUE, NOW() - INTERVAL '3 minutes'),
        ('admin-kitchen-power-02', 'Kitchen Appliance Power Meter', 'ESP32-PWR-v2', '2.1.1', kitchen_room_id, TRUE, NOW() - INTERVAL '4 minutes')
    ON CONFLICT (device_id)
    DO UPDATE SET
        device_name = EXCLUDED.device_name,
        device_model = EXCLUDED.device_model,
        firmware_version = EXCLUDED.firmware_version,
        room_id = EXCLUDED.room_id,
        is_online = TRUE,
        last_seen_at = EXCLUDED.last_seen_at;

    DELETE FROM anomalies
    WHERE device_id IN (
        'admin-lab-power-02',
        'admin-kitchen-power-02'
    );

    INSERT INTO sensor_readings
    (
        recorded_at,
        device_id,
        sensor_type,
        numeric_value,
        bool_value,
        unit
    )
    SELECT
        date_trunc('hour', NOW()) - (point_index || ' hours')::INTERVAL,
        'admin-lab-power-02',
        'power',
        55 + point_index,
        NULL,
        'W'
    FROM generate_series(0, 23) AS point_index
    ON CONFLICT (recorded_at, device_id, sensor_type)
    DO UPDATE SET numeric_value = EXCLUDED.numeric_value;

    INSERT INTO sensor_readings
    (
        recorded_at,
        device_id,
        sensor_type,
        numeric_value,
        bool_value,
        unit
    )
    SELECT
        date_trunc('hour', NOW()) - (point_index || ' hours')::INTERVAL,
        'admin-kitchen-power-02',
        'power',
        145 + point_index * 5,
        NULL,
        'W'
    FROM generate_series(0, 23) AS point_index
    ON CONFLICT (recorded_at, device_id, sensor_type)
    DO UPDATE SET numeric_value = EXCLUDED.numeric_value;

    INSERT INTO anomalies
    (
        recorded_at,
        device_id,
        sensor_type,
        numeric_value,
        bool_value,
        unit,
        anomaly_type,
        severity,
        score,
        description,
        detected_at
    )
    VALUES
        (
            date_trunc('hour', NOW()) - INTERVAL '5 hours',
            'admin-lab-power-02',
            'power',
            115.0,
            NULL,
            'W',
            'Evening Usage Drift',
            'INFO',
            0.49,
            'Study Lab shows a mild evening consumption drift.',
            date_trunc('hour', NOW()) - INTERVAL '5 hours' + INTERVAL '5 minutes'
        ),
        (
            date_trunc('hour', NOW()) - INTERVAL '3 hours',
            'admin-kitchen-power-02',
            'power',
            290.0,
            NULL,
            'W',
            'Appliance Overuse',
            'WARNING',
            0.77,
            'Kitchen appliance power is higher than the normal profile.',
            date_trunc('hour', NOW()) - INTERVAL '3 hours' + INTERVAL '5 minutes'
        ),
        (
            date_trunc('hour', NOW()) - INTERVAL '1 hours',
            'admin-kitchen-power-02',
            'power',
            340.0,
            NULL,
            'W',
            'Appliance Overuse',
            'CRITICAL',
            0.88,
            'Kitchen appliance power reached a critical range.',
            date_trunc('hour', NOW()) - INTERVAL '1 hours' + INTERVAL '5 minutes'
        );
END $$;
