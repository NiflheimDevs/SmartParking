CREATE TABLE IF NOT EXISTS "vehicle" (
    "id" SERIAL PRIMARY KEY,
    "owner_name" VARCHAR(100) NOT NULL,
    "owner_contact" VARCHAR(50),
    "plate" VARCHAR(20) UNIQUE,
    "rfid_id" VARCHAR(50) UNIQUE,
    "vehicle_type" VARCHAR(20) DEFAULT 'car',
    "created_at" TIMESTAMP DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS "parking_spot" (
    "id" SERIAL PRIMARY KEY,
    "spot_number" VARCHAR(20) UNIQUE NOT NULL,
    "is_occupied" BOOLEAN DEFAULT FALSE,
    "last_updated" TIMESTAMP DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS "entrance_exit" (
    "id" SERIAL PRIMARY KEY,
    "vehicle_id" INT NOT NULL,
    "entrance_time" TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    "exit_time" TIMESTAMP WITH TIME ZONE,
    "spot_id" INT,
    FOREIGN KEY ("vehicle_id") REFERENCES "vehicle"("id"),
    FOREIGN KEY ("spot_id") REFERENCES "parking_spot"("id")
);