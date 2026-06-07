ALTER TABLE buildings
ADD COLUMN organization_id INTEGER;

ALTER TABLE buildings
ADD CONSTRAINT fk_buildings_organization
FOREIGN KEY (organization_id)
REFERENCES organizations(id);