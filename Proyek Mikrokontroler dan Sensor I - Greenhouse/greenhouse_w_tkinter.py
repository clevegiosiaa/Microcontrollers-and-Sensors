import sqlite3
import threading
from tkinter import ttk

import serial
import datetime
import time
import tkinter as tk


def create_cursor(database_path):
    # membuat koneksi ke database
    conn = sqlite3.connect(database_path, check_same_thread=False)
    # membuat cursor
    cursor = conn.cursor()
    # mengembalikan cursor
    return cursor


def update_label(label, text):
    # Mengupdate teks pada label
    label.config(text=text)


def update_gui(tab1, tab2, cursor, ser):
    # tab 1 labels
    temp_text = tk.Label(tab1, text="Temperature", font=("Arial", 16), anchor="w", width=25)
    temp_text.grid(row=0, column=0)
    colon = tk.Label(tab1, text=":", font=("Arial", 16))
    colon.grid(row=0, column=1)
    temp_label = tk.Label(tab1, text="N/A", font=("Arial", 16), anchor="w")
    temp_label.grid(row=0, column=2)

    hum_text = tk.Label(tab1, text="Humidity", font=("Arial", 16), anchor="w", width=25)
    hum_text.grid(row=1, column=0)
    colon = tk.Label(tab1, text=":", font=("Arial", 16))
    colon.grid(row=1, column=1)
    hum_label = tk.Label(tab1, text="N/A", font=("Arial", 16), anchor="w")
    hum_label.grid(row=1, column=2)

    light_text = tk.Label(tab1, text="light", font=("Arial", 16), anchor="w", width=25)
    light_text.grid(row=2, column=0)
    colon = tk.Label(tab1, text=":", font=("Arial", 16))
    colon.grid(row=2, column=1)
    light_label = tk.Label(tab1, text="N/A", font=("Arial", 16), anchor="w")
    light_label.grid(row=2, column=2)

    # tab 2 labels
    water_text = tk.Label(tab2, text="Water", font=("Arial", 16), anchor="w", width=25)
    water_text.grid(row=0, column=0)
    colon = tk.Label(tab2, text=":", font=("Arial", 16))
    colon.grid(row=0, column=1)
    water_label = tk.Label(tab2, text="N/A", font=("Arial", 16), anchor="w")
    water_label.grid(row=0, column=2)

    lamp_text = tk.Label(tab2, text="Lamps", font=("Arial", 16), anchor="w", width=25)
    lamp_text.grid(row=1, column=0)
    colon = tk.Label(tab2, text=":", font=("Arial", 16))
    colon.grid(row=1, column=1)
    lamp_label = tk.Label(tab2, text="N/A", font=("Arial", 16), anchor="w")
    lamp_label.grid(row=1, column=2)

    ac_text = tk.Label(tab2, text="Air Conditioner", font=("Arial", 16), anchor="w", width=25)
    ac_text.grid(row=2, column=0)
    colon = tk.Label(tab2, text=":", font=("Arial", 16))
    colon.grid(row=2, column=1)
    ac_label = tk.Label(tab2, text="N/A", font=("Arial", 16), anchor="w")
    ac_label.grid(row=2, column=2)

    def update():
        # Membaca data dari serial
        data = ser.readline().decode('latin-1').strip()
        values = data.split(',')

        # Extract the values
        temp = (values[0].split(':')[1])
        light = (values[1].split(':')[1])
        moist = (values[2].split(':')[1])
        lamps = (values[4].split(':')[1])
        ac = (values[3].split(':')[1])
        tap = (values[5].split(':')[1])

        print(temp, light, moist, ac, lamps, tap)
        now = datetime.datetime.now()

        tanggal = now.date()
        waktu = now.time()

        # Mendapatkan jam, menit, dan detik dari waktu
        jam = waktu.hour
        menit = waktu.minute
        detik = waktu.second

        timenow = str(jam) + ":" + str(menit) + ":" + str(detik)

        # Menyimpan data ke database
        cursor.execute(
            "INSERT INTO monitoring (tanggal, time, kelembapan, suhu, cahaya, status_keran, status_ac, "
            "status_lampu)"
            "VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
            (tanggal, timenow, moist, temp, light, tap, ac, lamps))

        # Commit perubahan ke database
        cursor.connection.commit()

        # Update GUI dengan nilai terbaru
        update_label(temp_label, "{}".format(temp))
        update_label(hum_label, "{}".format(moist))
        update_label(light_label, "{} lux".format(light))

        update_label(water_label, "{}".format(tap))
        update_label(lamp_label, "{}".format(lamps))
        update_label(ac_label, "{}".format(ac))

        # Schedule the update to be called again after 1 second
        tab1.after(1000, update)

    # Schedule the initial update to be called after 1 second
    tab1.after(1000, update)


def main():
    # Membuat koneksi ke database
    cursor = create_cursor('greenhouse_project')

    ser = serial.Serial('COM5', 9600)

    # Membuat tabel jika belum ada
    cursor.execute('''CREATE TABLE IF NOT EXISTS monitoring (tanggal DATE, time TEXT, kelembapan INT, suhu INT, 
    cahaya INT, status_keran TEXT, status_ac TEXT, status_lampu TEXT)''')

    root = tk.Tk()
    root.title("Greenhouse Monitoring System")
    root.geometry("500x200")

    # create a notebook
    notebook = ttk.Notebook(root)

    greenhouse_label = tk.Label(root, text="Greenhouse System", font=("Arial", 20), anchor="center")
    greenhouse_label.grid(row=0, columnspan=3)

    # global tab1, tab2
    tab1 = ttk.Frame(notebook, width=500, height=180)
    tab2 = ttk.Frame(notebook, width=500, height=180)

    notebook.add(tab1, text="Information")
    notebook.add(tab2, text="Status")
    notebook.grid(row=2, column=0, columnspan=2, sticky="nsew")

    # Start a new thread to update the GUI
    gui_thread = threading.Thread(target=update_gui, args=(tab1, tab2, cursor, ser))
    gui_thread.start()

    try:
        # Start the main loop
        root.mainloop()
    except KeyboardInterrupt:
        # Handle the KeyboardInterrupt exception
        print("KeyboardInterrupt received. Stopping...")
        # Cleanup resources
        cursor.close()
        ser.close()
        # Wait for the GUI thread to finish
        gui_thread.join()


if __name__ == '__main__':
    main()
