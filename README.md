Program ini digunakan untuk sistem **bel cerdas cermat otomatis** dengan beberapa mode permainan, yang bisa digunakan saat lomba kuis antar tim.
Setiap peserta memiliki tombol sendiri, dan yang tercepat menekan tombol akan ditampilkan di LCD dengan bunyi buzzer serta nyala LED indikator.

---

## ⚙️ **KOMPONEN YANG DIGUNAKAN**

| No | Komponen                  | Jumlah | Keterangan                                  |
| -- | ------------------------- | ------ | ------------------------------------------- |
| 1  | Arduino Uno / Mega        | 1      | Pusat kontrol sistem                        |
| 2  | LCD I2C 16x4              | 1      | Menampilkan status mode, peserta, dan timer |
| 3  | Tombol Push Button        | 10     | Tombol peserta (1–10)                       |
| 4  | LED                       | 10     | Indikator peserta (1–10)                    |
| 5  | Tombol Reset              | 1      | Mengatur ulang sistem                       |
| 6  | Tombol Mode               | 1      | Mengganti mode permainan                    |
| 7  | Tombol Start              | 1      | Menjalankan timer di Mode 3                 |
| 8  | Buzzer                    | 2      | Satu untuk peserta, satu untuk operator     |
| 9  | Kabel jumper + Breadboard | -      | Untuk pengkabelan                           |

---

## 🧠 **PENJELASAN LOGIKA PROGRAM**

### 1. **Inisialisasi Awal**

Pada `setup()`, semua komponen diinisialisasi:

* LCD diaktifkan (`lcd.init()`, `lcd.backlight()`).
* Semua tombol peserta diatur sebagai **input pull-up** (`INPUT_PULLUP`), artinya tombol aktif bila ditekan (LOW).
* Semua LED diatur sebagai output.
* Ditampilkan pesan pembuka di LCD.

---

### 2. **Pengaturan Mode**

Ada **3 mode permainan**, dapat diubah dengan tombol **Mode (pin 13)**:

| Mode | Nama           | Fungsi                                                              |
| ---- | -------------- | ------------------------------------------------------------------- |
| 1    | **1 Pemenang** | Hanya peserta tercepat yang diterima (yang pertama menekan tombol). |
| 2    | **3 Pemenang** | Sistem menyimpan urutan 3 peserta tercepat.                         |
| 3    | **Timer Mode** | Mode penghitung waktu (waktu berbicara / menjawab).                 |

Setiap kali tombol Mode ditekan, LCD akan menampilkan mode aktif:

```
Mode: 1
1 Pemenang
```

---

### 3. **Reset Sistem**

Tombol **Reset (pin 12)** digunakan untuk:

* Mematikan semua LED peserta.
* Menghapus data peserta aktif.
* Menghentikan timer.
* Menampilkan teks “Sistem direset!” di LCD.

---

### 4. **Mekanisme Tekan Tombol Peserta**

Setiap peserta punya tombol dan LED masing-masing.
Jika tombol ditekan:

* Sistem cek apakah peserta sudah tercatat (`pesertaSudahAktif()`).
* Jika belum, peserta disimpan dalam array `pesertaAktif[]`.
* LED peserta menyala.
* Buzzer peserta berbunyi singkat (`tone(buzzerPeserta, 1200, 200)`).
* LCD menampilkan:

  ```
  Peserta 3 Tekan!
  ```

Kemudian:

* Jika mode 1 → hanya peserta pertama diterima.
* Jika mode 2 → tiga peserta pertama diterima.
* Setelah batas terpenuhi → Buzzer operator berbunyi (`tone(buzzerOperator, 2000, 400)`).

---

### 5. **Mode Timer (Mode 3)**

* Aktifkan mode 3 dengan tombol Mode.
* Tekan tombol **Start (A2)** untuk memulai timer.
* LCD menampilkan “Timer dimulai...”
* Selama timer berjalan:

  * LCD baris ke-4 menunjukkan waktu tersisa:

    ```
    Sisa: 8 detik
    ```
  * Saat waktu habis, buzzer operator berbunyi panjang dan LCD menampilkan:

    ```
    Waktu Habis!
    ```

---

### 6. **Fungsi Tambahan**

#### 🔹 `pesertaSudahAktif()`

Mengecek apakah peserta sudah menekan tombol sebelumnya agar tidak dihitung dua kali.

#### 🔹 `resetSystem()`

Mengatur ulang sistem ke kondisi awal.

#### 🔹 `tampilkanMode()`

Menampilkan mode yang sedang aktif ke LCD setiap kali mode berubah.

---

## 🔌 **PENJELASAN WIRING / RANGKAIAN**

Berikut pembagian pin Arduino dan sambungan komponennya:

| Komponen            | Pin Arduino | Keterangan                                            |
| ------------------- | ----------- | ----------------------------------------------------- |
| Tombol Peserta 1–10 | D2 – D11    | Tombol dihubungkan ke GND (gunakan INPUT_PULLUP)      |
| LED Peserta 1–10    | D22 – D31   | Katoda ke GND, anoda ke resistor 220Ω lalu ke pin LED |
| Tombol Reset        | D12         | Dihubungkan ke GND                                    |
| Tombol Mode         | D13         | Dihubungkan ke GND                                    |
| Tombol Start        | A2          | Dihubungkan ke GND                                    |
| Buzzer Peserta      | A0          | Aktif saat peserta menekan tombol                     |
| Buzzer Operator     | A1          | Aktif untuk tanda mode selesai atau waktu habis       |
| LCD I2C SDA         | A4          | Jalur data I2C                                        |
| LCD I2C SCL         | A5          | Jalur clock I2C                                       |
| VCC LCD             | 5V          | Daya                                                  |
| GND LCD             | GND         | Ground sistem                                         |

---

## 💡 **CARA KERJA SINGKAT (ALUR LOGIKA)**

1. Arduino menyala → LCD menampilkan “Bel Cerdas Cermat”.
2. Operator pilih mode (1, 2, atau 3).
3. Peserta menekan tombol → sistem deteksi siapa tercepat.
4. LED peserta menyala → buzzer berbunyi → nama peserta tampil di LCD.
5. Jika sudah cukup pemenang (mode 1: 1 orang, mode 2: 3 orang) → sistem kunci tombol lain.
6. Tombol reset untuk memulai ulang.
7. Pada mode timer, tekan tombol Start → hitung mundur waktu tampil di LCD.
8. Saat waktu habis → buzzer operator bunyi panjang.

---

## 📘 **Kesimpulan**

Program ini dirancang agar:

* Dapat digunakan untuk lomba cepat-tepat dengan 10 peserta.
* Bisa memilih mode sesuai kebutuhan (1 pemenang, 3 pemenang, atau timer).
* Tampilan jelas di LCD 16x4.
* Dilengkapi bunyi buzzer dan indikator LED.
