import sqlite3
import os
import time

def get_connection():

    return sqlite3.connect("/Users/pradippokhrel/Desktop/travelmanagementapp/travel/travel.db", timeout=10)

def check_table_exists():

    with get_connection() as conn:
        cursor = conn.cursor()
        cursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='packages';")
        return cursor.fetchone() is not None

def insert_image(image_path, name, contact_number, location, price_per_night, facilities):

    try:
        with get_connection() as conn:
            cursor = conn.cursor()
            with open(image_path, 'rb') as file:
                blob_data = file.read()


            cursor.execute("SELECT COUNT(*) FROM packages;")
            print("Number of entries in packages:", cursor.fetchone()[0])


            cursor.execute("""
                INSERT INTO packages (name, contact_number, location, price_per_night, facilities, image)
                VALUES (?, ?, ?, ?, ?, ?)
            """, (name, contact_number, location, price_per_night, facilities, blob_data))

            conn.commit()
            print(f"Image '{name}' inserted successfully.")
    except sqlite3.OperationalError as e:
        print(f"An error occurred: {e}. Please check the database connection or table schema.")
    except FileNotFoundError:
        print(f"The image file '{image_path}' does not exist. Please provide a valid path.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

if __name__ == "__main__":
    if check_table_exists():
        print("Table 'packages' exists.")
        insert_image(
            "/Users/pradippokhrel/Desktop/package6.jpg",
            "New York 6 Days Package", "976328723", "New York City, USA", "$45",
            "Local Cuisine, Gym Access, City Tours, Spa Services"
        )
    else:
        print("Table 'packages' was not created successfully.")
