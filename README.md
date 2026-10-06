# Naskah Presentasi

**Paper:** Single-step Diffusion for Image Compression at Ultra-Low Bitrates
**Penulis:** Chanung Park, Joo Chan Lee, Jong Hwan Ko (Sungkyunkwan University, Korea Selatan)
**Sumber:** arXiv:2506.16572v2
**Estimasi durasi:** ±15 menit (13 slide)

> Catatan: teks biasa adalah kalimat yang diucapkan. Teks dalam kurung siku [ ] adalah petunjuk untuk presenter.

---

## Slide 1 — Judul (±30 detik)

Selamat pagi/siang, Bapak/Ibu dosen dan teman-teman sekalian.

Pada kesempatan ini saya akan mempresentasikan sebuah paper berjudul **"Single-step Diffusion for Image Compression at Ultra-Low Bitrates"**, karya Chanung Park, Joo Chan Lee, dan Jong Hwan Ko dari Sungkyunkwan University, Korea Selatan.

Secara singkat, paper ini membahas cara memampatkan gambar sampai ukuran yang sangat kecil, tetapi hasilnya tetap enak dilihat dan proses *decoding*-nya cepat.

---

## Slide 2 — Garis Besar Presentasi (±30 detik)

Presentasi ini terdiri dari enam bagian: latar belakang masalah, ide utama yang diusulkan, metode secara rinci, hasil eksperimen, studi ablasi, dan terakhir kesimpulan.

---

## Slide 3 — Latar Belakang: Mengapa Kompresi Gambar Penting? (±1,5 menit)

[Tampilkan Gambar 1 dari paper]

Kompresi gambar adalah fondasi komunikasi digital dan penyimpanan data. Tujuannya sederhana: ukuran data sekecil mungkin, tetapi kualitas visual tetap terjaga.

Ukuran data biasanya diukur dengan **bpp (bits per pixel)**, yaitu rata-rata jumlah bit untuk setiap piksel. Semakin kecil bpp, semakin kecil ukuran file.

Kita sudah mengenal JPEG, JPEG2000, dan BPG. Kemudian muncul *learned codec* yang berbasis jaringan saraf. Namun, ketika bitrate dibuat **sangat rendah**, yaitu di bawah sekitar 0,05 bpp, kualitas gambar turun drastis. Kondisi inilah yang disebut **ultra-low bitrate**.

---

## Slide 4 — Masalah pada Metode yang Ada (±2 menit)

Ada tiga pendekatan utama, dan masing-masing punya kelemahan.

**Pertama, metode berbasis VAE**, misalnya ELIC. Metode ini mengoptimalkan error per piksel, sehingga pada bitrate rendah hasilnya **terlalu halus atau buram**. Tekstur dan tepi objek hilang.

**Kedua, metode berbasis GAN**, misalnya MS-ILLM. Hasilnya lebih tajam secara persepsi. Namun, GAN rentan *mode collapse* dan sintesis teksturnya sering **tidak stabil**.

**Ketiga, metode berbasis diffusion**, misalnya DiffEIC. Kualitas persepsinya bagus, tetapi ada dua masalah besar:
1. Proses *denoising* dilakukan **berulang kali**, sehingga decoding lambat. DiffEIC hanya sekitar 0,19 FPS pada Gambar 1.
2. Modelnya **sangat besar**, sekitar 1,4 miliar parameter.

Jadi pertanyaan penelitiannya adalah: **bisakah kita memperoleh kualitas persepsi seperti diffusion, tetapi dengan decoding yang cepat?**

---

## Slide 5 — Ide Utama dan Kontribusi (±1,5 menit)

Jawaban penulis adalah **diffusion satu langkah (single-step diffusion)**. Gambar direkonstruksi hanya dengan satu kali denoising.

Ada dua inovasi utama:

1. **VQ-Residual Training.** Representasi gambar dipisah menjadi dua bagian: *base code* yang menangkap struktur global, dan *residual* yang menangkap detail frekuensi tinggi.
2. **Rate-aware Noise Modulation.** Kekuatan denoising disesuaikan dengan bitrate yang diinginkan, tanpa menambah jumlah langkah.

Selain itu, seluruh bitstream dibangun dengan **vector quantization (VQ)**, sehingga ukuran output mudah diprediksi.

---

## Slide 6 — Gambaran Umum Arsitektur (±1,5 menit)

[Tampilkan Gambar 2 dan Gambar 3]

Alurnya begini. Gambar masukan *X* masuk ke encoder dan menjadi representasi laten *x*. Representasi ini dikuantisasi dengan VQ menggunakan sebuah *codebook*, menghasilkan laten terkompresi *y*.

Dari sini ada dua cabang:
- **Base branch**, yaitu *y* itu sendiri, yang membawa konteks tingkat rendah dan semantik.
- **Residual branch**, yaitu selisih antara laten berderau dan *y*, yang membawa informasi struktur yang hilang saat kompresi.

Keduanya digabung oleh sebuah *adapter*, lalu diproses oleh **U-Net** dalam satu langkah denoising. Hasilnya dikembalikan menjadi gambar oleh decoder.

---

## Slide 7 — Metode 1: Kompresi Laten dengan VQ (±1,5 menit)

Pada kebanyakan codec, bitstream dibuat dengan *entropy coding*. Masalahnya, ukuran output bisa **berbeda-beda** tergantung kompleksitas gambar.

Penulis memakai pendekatan berbeda: seluruh bitstream dibangun dari **indeks codebook VQ**. Setiap vektor laten dipetakan ke entri codebook terdekat. Ukuran codebook dan resolusi laten disesuaikan dengan target bpp.

Karena jumlah bit hanya bergantung pada indeks, bitrate akhir **sangat dekat dengan target** dan variansnya kecil. Ini penting untuk aplikasi dengan keterbatasan bandwidth, misalnya sistem nirkabel.

---

## Slide 8 — Metode 2: Single-step Denoising (±2 menit)

Penulis mengadaptasi **ResShift**, metode diffusion yang efisien untuk *super-resolution*. Namun, menerapkannya langsung pada kompresi tidak mudah. Pada super-resolution, masukan masih memuat banyak informasi persepsi. Pada kompresi, masukan sudah sangat terkompresi, dan detail halus biasanya hilang lebih dulu.

Temuan menarik di paper ini: **menambah banyak langkah denoising justru dapat menurunkan kualitas persepsi** (Tabel 2). Karena itu, cukup satu langkah.

Agar pelatihan stabil, mereka memakai trik: model dilatih dengan **dua langkah**.
- Satu langkah dengan **noise besar** untuk kualitas persepsi. Langkah inilah satu-satunya yang dipakai saat inferensi.
- Satu langkah dengan **noise sangat kecil**, hanya saat pelatihan, agar model tahan terhadap distorsi.

Jadi inferensi tetap satu langkah, tetapi pelatihannya lebih kaya.

---

## Slide 9 — Metode 3: Residual Fusion U-Net dan Fungsi Loss (±1,5 menit)

Denoising dari laten terkompresi memang menjaga semantik, tetapi dapat menimbulkan artefak di level piksel. Karena itu digunakan **residual fusion**: laten terkompresi *y* dan residual (*x̃ − y*) digabung lewat satu lapisan konvolusi, lalu diperhalus oleh U-Net.

Pada Gambar 4, terlihat bahwa:
- hanya base branch: kualitas persepsi baik, tetapi distorsi lebih buruk (PSNR 23,51);
- hanya residual branch: PSNR lebih baik, tetapi persepsi lebih buruk;
- gabungan keduanya: **terbaik di keduanya** (LPIPS 0,2824, PSNR 25,05).

Model dilatih *end-to-end* dengan *loss* yang menggabungkan error piksel, loss persepsi LPIPS, serta dua loss struktural untuk menyelaraskan laten asli dan laten terkuantisasi.

---

## Slide 10 — Metode 4: Rate-aware Noise Modulation (±1,5 menit)

[Tampilkan Gambar 5]

Pada diffusion biasa, bitrate rendah biasanya membutuhkan lebih banyak langkah denoising. Akibatnya decoding makin lambat.

Penulis mengamati bahwa untuk setiap ukuran codebook, ada nilai noise η yang memberi LPIPS terbaik. Semakin besar codebook (bitrate naik), nilai η optimal semakin **kecil**. Secara sederhana, hubungannya adalah η sebanding dengan 1/B.

Intuisinya: pada bitrate rendah informasi yang tersisa sedikit, sehingga perlu **noise lebih kuat** agar model dapat melakukan koreksi yang lebih besar dalam satu langkah. Dengan demikian, kekuatan denoising diatur lewat noise, **bukan** lewat jumlah langkah.

---

## Slide 11 — Hasil Eksperimen (±2 menit)

[Tampilkan Gambar 6, Tabel 1, dan Gambar 7]

**Pengaturan eksperimen.** Pelatihan memakai ImageNet dengan *random crop* 256×256. Evaluasi memakai dataset **Kodak** dan **CLIC2020**, dengan metrik persepsi **LPIPS** dan **DISTS** (semakin kecil semakin baik). Pembandingnya antara lain BPG, ELIC, HiFiC, MS-ILLM, CDC, DiffEIC, DiffPC, dan RDEIC.

**Kualitas.** Pada bitrate di bawah 0,05 bpp, metode ini konsisten lebih baik daripada semua pembanding. Pada rentang bitrate lain, kualitasnya sebanding. Pada Tabel 1, BD-Rate LPIPS adalah **−45,65%**, yang terbaik. Pada DISTS, hasilnya **−48,23%**, terbaik kedua setelah RDEIC-5 (−50,84%).

**Kecepatan.** Waktu decoding hanya **0,253 detik**, dibandingkan 12,502 detik pada DiffEIC. Itu sekitar **50 kali lebih cepat**. Parameternya **210 juta**, dibandingkan 1,4 miliar pada DiffEIC.

**Visual.** Dibandingkan ELIC yang buram, dan DiffEIC yang lambat, hasil metode ini lebih tajam pada bpp yang sama atau lebih rendah.

---

## Slide 12 — Analisis Bitrate dan Studi Ablasi (±2 menit)

[Tampilkan Gambar 9, Gambar 10, dan Tabel 2]

**Bitrate yang dapat diprediksi.** Pada Gambar 9(a), variansi bitrate keluaran DiffEIC jauh lebih besar daripada metode ini, karena DiffEIC memakai entropy coding. Dengan VQ, bitrate keluaran terkendali.

**Jumlah langkah.** Pada DiffEIC, jumlah langkah optimal berbeda-beda untuk tiap kelompok gambar (sekitar 20 sampai 50 langkah). Pada metode ini, jumlah langkah selalu satu, dan hasilnya tetap lebih baik pada target bitrate yang sama.

**Ablasi.** Tiga komponen diuji, dan menghapus salah satunya menurunkan LPIPS:
1. Tanpa VQ-Residual Training, performa turun tajam, terutama pada bitrate rendah.
2. Tanpa residual branch, LPIPS lebih buruk di semua bitrate.
3. Tanpa base branch, performa menurun, terutama pada bitrate menengah ke tinggi.

**Skema pelatihan 2-langkah.** Pada Tabel 2 (0,0294 bpp), metode ini mencapai LPIPS 0,309 dan DISTS 0,122, lebih baik daripada diffusion 1 sampai 50 langkah, dengan PSNR 22,19 dB dan MS-SSIM 0,767.

---

## Slide 13 — Kesimpulan (±1 menit)

Sebagai kesimpulan:

1. Paper ini mengusulkan **diffusion satu langkah** untuk kompresi gambar pada ultra-low bitrate.
2. **VQ-Residual Training** menjaga struktur dan detail, sedangkan **rate-aware noise modulation** menyesuaikan kekuatan denoising dengan bitrate.
3. Hasilnya adalah kualitas persepsi yang kompetitif, bitrate yang **dapat diprediksi**, dan decoding **sekitar 50× lebih cepat** daripada codec diffusion sebelumnya.

Dengan begitu, codec generatif menjadi lebih praktis untuk aplikasi nyata yang dibatasi bandwidth.

**Catatan kritis (opsional untuk diskusi):** keunggulan paling jelas ada di bawah 0,05 bpp; pada bitrate lain hasilnya sebanding. Pada DISTS, metode ini belum yang terbaik. Selain itu, MS-ILLM masih sedikit lebih cepat dalam decoding (0,234 detik).

Demikian presentasi saya. Terima kasih, dan saya siap menerima pertanyaan.

---

## Lampiran A — Istilah Penting

| Istilah | Penjelasan sederhana |
|---|---|
| **bpp (bits per pixel)** | Rata-rata bit per piksel; makin kecil, file makin kecil. |
| **Ultra-low bitrate** | Bitrate sangat rendah, sekitar di bawah 0,05 bpp. |
| **VAE / GAN / Diffusion** | Tiga keluarga model: autoencoder variasional, jaringan adversarial, dan model yang membuang noise bertahap. |
| **Vector Quantization (VQ)** | Mengganti tiap vektor dengan entri terdekat dari sebuah "kamus" (codebook). |
| **Residual** | Selisih antara versi asli dan versi terkompresi. |
| **LPIPS / DISTS** | Metrik kemiripan persepsi; makin kecil makin baik. |
| **PSNR / MS-SSIM** | Metrik distorsi; makin besar makin baik. |
| **BD-Rate** | Penghematan bitrate rata-rata pada kualitas yang sama; negatif berarti lebih hemat. |

## Lampiran B — Antisipasi Pertanyaan

**1. Mengapa cukup satu langkah?**
Pada kompresi, detail halus sudah hilang di masukan. Paper menunjukkan bahwa banyak langkah tidak membantu dan bahkan dapat menurunkan kualitas persepsi (Tabel 2).

**2. Apa kelebihan VQ dibanding entropy coding?**
Ukuran output hanya bergantung pada indeks codebook, sehingga variansi bitrate kecil dan mudah diprediksi.

**3. Apa fungsi dua langkah saat pelatihan?**
Langkah ber-noise kecil membuat pelatihan lebih stabil dan model lebih tahan distorsi. Saat inferensi hanya langkah ber-noise besar yang dipakai.

**4. Mengapa noise lebih besar pada bitrate rendah?**
Informasi yang tersisa lebih sedikit, jadi model memerlukan koreksi yang lebih kuat dalam satu langkah.

**5. Apa keterbatasannya?**
Keunggulan terbesar ada di bpp < 0,05; pada bitrate lain hanya sebanding. Pada DISTS, RDEIC-5 masih sedikit lebih baik.
