# Penjelasan Paper per Halaman

**Paper:** Single-step Diffusion for Image Compression at Ultra-Low Bitrates (Park, Lee, Ko; arXiv:2506.16572v2)

Dokumen ini menjelaskan isi setiap halaman paper secara berurutan, dengan bahasa akademik yang mudah dipahami. Istilah teknis dijelaskan saat pertama kali muncul.

---

## Halaman 1 — Judul, Abstrak, dan Awal Pendahuluan

### Apa yang ada di halaman ini
Judul, penulis (Sungkyunkwan University, Korea Selatan), Gambar 1, abstrak, dan paragraf pertama Pendahuluan.

### Penjelasan Gambar 1
Gambar 1 adalah perbandingan visual lima kolom pada dua gambar contoh (bunga di jendela dan sungai di pegunungan):
- (a) Ground Truth: gambar asli.
- (b) Metode berbasis VAE: ELIC.
- (c) Metode berbasis GAN: MS-ILLM.
- (d) Metode berbasis diffusion: DiffEIC.
- (e) Metode yang diusulkan.

Di setiap gambar tertulis dua angka: **BPP** (bits per pixel, semakin kecil semakin hemat) dan **FPS** (frame per second, semakin besar semakin cepat). Pada contoh pertama, metode yang diusulkan memakai 0,0401 bpp dengan 5,65 FPS, sedangkan DiffEIC memakai 0,0400 bpp tetapi hanya 0,19 FPS. Artinya, pada ukuran file yang hampir sama, metode baru jauh lebih cepat. Pada contoh kedua, metode baru bahkan memakai bpp paling rendah (0,0310) dengan 5,68 FPS. Kotak merah menunjukkan area yang diperbesar: ELIC tampak buram, MS-ILLM menunjukkan pola tekstur yang aneh, sedangkan metode baru tampak lebih alami.

### Penjelasan Abstrak
Abstrak menyampaikan alur berikut:
1. **Masalah:** codec standar maupun *learned codec* mengalami penurunan kualitas yang parah pada bpp yang sangat rendah.
2. **Kelemahan diffusion:** model diffusion memang memberi hasil generatif yang lebih baik pada bitrate rendah, tetapi kualitas persepsinya terbatas dan decoding-nya lambat karena memerlukan banyak langkah denoising.
3. **Solusi:** model diffusion **satu langkah** dengan dua inovasi:
   - **VQ-Residual training**: representasi laten dipecah menjadi *base code* terstruktur (geometri global) dan *residual* hasil pembelajaran (detail frekuensi tinggi).
   - **Rate-aware noise modulation**: kekuatan denoising disesuaikan dengan bitrate yang diinginkan.
4. **Hasil:** performa kompresi sebanding dengan metode terbaik, tetapi decoding sekitar **50× lebih cepat** dibanding codec diffusion sebelumnya.

### Awal Pendahuluan
Pendahuluan dibuka dengan menjelaskan bahwa kompresi gambar sangat penting untuk komunikasi dan penyimpanan digital. Codec tradisional (JPEG, JPEG2000, BPG) memakai transformasi rancangan manual dan model statistik untuk membuat representasi yang ringkas.

---

## Halaman 2 — Lanjutan Pendahuluan, Gambar 2, dan Awal Related Work

### Lanjutan Pendahuluan
Halaman ini menyusun argumen dalam empat tahap:

1. **Pendekatan klasik dan learned codec.** Keduanya dirancang berdasarkan prinsip teori informasi, yaitu mengurangi entropi dengan membuang komponen frekuensi tinggi yang hampir tidak terlihat mata. Ketika bitrate sangat dibatasi, informasi yang tersisa terlalu sedikit sehingga gambar tidak dapat direkonstruksi dengan baik (hasil buram, lihat Gambar 1(b)).
2. **GAN.** GAN dapat meningkatkan kualitas persepsi pada bitrate ultra-rendah, tetapi rentan terhadap *mode collapse* (model hanya menghasilkan variasi terbatas) dan sintesis tekstur yang tidak stabil (Gambar 1(c)).
3. **Diffusion.** Model diffusion menghasilkan gambar lewat denoising iteratif. Namun, model ini cenderung memprioritaskan konsistensi **semantik** (isi gambar masuk akal) daripada detail persepsi halus, padahal kompresi menuntut hasil yang tetap mirip dengan gambar asli. Selain itu, sifat iteratifnya membuat komputasi berat dan decoding lambat.
4. **Usulan penulis.** Model diffusion satu langkah dengan:
   - *vector quantization* (VQ) untuk kompresi laten,
   - modul pembangkit residual satu langkah yang mempelajari selisih antara laten terkompresi dan laten asli,
   - *rate-aware noise modulation* yang menyesuaikan kekuatan denoising dengan bitrate.

Pendahuluan ditutup dengan klaim hasil: kinerja sebanding dengan metode terbaik pada bitrate ultra-rendah, mengungguli DiffEIC dalam kualitas visual dan persepsi, dengan ukuran penyimpanan lebih kecil, serta decoding lebih dari 50× lebih cepat. Penulis juga menyebut ukuran model: **210 juta parameter** dibanding **1,4 miliar** pada DiffEIC.

### Gambar 2
Gambar 2 memberi ringkasan arsitektur: citra masuk diubah menjadi kode laten diskret lewat modul VQ-compression dengan codebook yang dapat dipelajari. Saat pelatihan, selisih antara citra asli dan hasil rekonstruksi VQ dimodelkan oleh U-Net yang dikondisikan pada kode laten. U-Net dilatih untuk melakukan denoising satu langkah, dibimbing oleh sinyal residual dan prior semantik dari laten terkompresi.

### Awal Related Work
Bagian 2 menjelaskan bahwa jaringan saraf kini menjadi dasar codec kompresi lossy modern, sering kali mengungguli metode tradisional (BPG, VVC, JPEG2000, HEVC, JPEG). Pendekatan awal memakai autoencoder (umumnya VAE dengan entropy model yang dipelajari) yang dioptimalkan terhadap distorsi per piksel. Akibatnya, hasil cenderung terlalu halus, terutama pada bitrate ultra-rendah.

Sub-bagian 2.1 mulai membahas upaya meningkatkan kualitas persepsi: kerugian adversarial dan perseptual diintegrasikan. Codec berbasis GAN (Agustsson dkk.) menunjukkan bahwa tekstur realistis dapat direkonstruksi dari kode yang sangat terkompresi. HiFiC melanjutkan arah ini dengan GAN dan perceptual loss.

---

## Halaman 3 — Gambar 3, Related Work Lanjutan, dan Awal Metode

### Gambar 3 (kerangka kerja)
Gambar ini menggambarkan keseluruhan pipeline:
1. Citra *X* masuk ke encoder **ℰ** dan menjadi laten *x*.
2. Laten dikuantisasi lewat **VQ-E / VQ-D** dengan **codebook**, menghasilkan *y* (cabang **base**).
3. Selisih antara laten dan hasil kuantisasi membentuk *e* (cabang **residual**).
4. Kedua cabang digabung oleh **Adapter** menjadi *z*, lalu diproses **Denoising U-Net**.
5. Hasilnya *x̂* diubah kembali menjadi citra *X̂* oleh decoder **𝒟**.

Gembok pada ℰ dan 𝒟 menandakan bahwa encoder dan decoder berasal dari model yang sudah dilatih sebelumnya dan bobotnya dibekukan.

### Related Work lanjutan: diffusion untuk kompresi
Penulis merangkum perkembangan kompresi berbasis diffusion:
- **CDC**: codec diffusion kondisional dengan laten ringkas dari encoder VAE; kualitas persepsinya lebih baik dibanding decoder GAN pada bitrate rendah.
- **PerCo**: memakai latent diffusion model (LDM) yang dikondisikan pada laten terkuantisasi dan deskripsi teks.
- **DiffEIC dan DiffPC**: menggabungkan VAE kompresif dengan diffusion yang sudah dilatih sebelumnya.
- **HDCompression**: pendekatan hibrida diffusion dengan codec konvensional.
- **RDEIC**: mengurangi langkah denoising lewat strategi *relay residual*.

Kesimpulan bagian ini: diffusion unggul dalam kualitas persepsi dibanding GAN, tetapi beban komputasinya besar, baik karena puluhan langkah denoising maupun karena model yang sangat besar.

### 2.2 Percepatan model diffusion
Karena diffusion bersifat iteratif, banyak strategi percepatan telah diusulkan:
- **DDIM dan DPM-Solver**: melewati langkah perantara atau menyelesaikan persamaan diferensial (ODE), sehingga cukup 10 sampai 20 langkah.
- **Progressive distillation**: berulang kali membagi dua jumlah langkah.
- **DMD (distribution matching distillation)**: melatih generator satu langkah yang meniru distribusi keluaran model diffusion penuh.
- **Consistency models**: memaksa konsistensi lintas level noise sehingga satu langkah inferensi memungkinkan.
- **EDM**: menganalisis ruang desain model diffusion.

### ResShift (dasar teori yang dipakai)
Metode ini awalnya untuk *super-resolution*. Alih-alih proses noise standar, ResShift memakai rantai Markov berbasis **residual** antara citra resolusi tinggi (HR, *x₀*) dan resolusi rendah (LR, *y₀*), dengan *e₀ = x₀ − y₀*.

- **Persamaan (1)** adalah proses maju: *x_t* diperoleh dengan menggeser *x₀* sebesar *η_t · e₀* lalu menambahkan noise berskala κ²η_t. Parameter η_t adalah faktor pergeseran yang bergantung pada waktu, dan κ mengatur varians noise.
- **Persamaan (2) dan (3)** adalah proses mundur dari *y₀* ke *x₀*. Rata-ratanya diparameterisasi oleh jaringan saraf *f_θ*, dengan *α_t = η_t − η_{t−1}*.

Hasilnya, jumlah langkah turun menjadi sekitar 15 dengan kualitas tetap terjaga. Penulis akan memangkas ini lagi menjadi satu langkah.

### Awal Bab 3 (Metode)
Citra RGB *X* ∈ ℝ^(H×W×3) dienkode menjadi laten *x = ℰ(X)*. Setelah kompresi dan satu kali denoising, fitur hasil *x̂* didekode menjadi *X̂ = 𝒟(x̂)*.

---

## Halaman 4 — Gambar 4, Kompresi Laten, dan Denoising Satu Langkah

### Gambar 4 (perbandingan rekonstruksi)
Gambar ini memakai citra burung beo dan membandingkan empat kondisi dengan dua kelompok metrik:
- **Persepsi** (LPIPS dan DISTS, semakin kecil semakin baik).
- **Distorsi** (PSNR dan MS-SSIM, semakin besar semakin baik).

| Kondisi | LPIPS | DISTS | PSNR | MS-SSIM |
|---|---|---|---|---|
| Hanya base branch | 0,3334 | 0,1664 | 23,51 | 0,7631 |
| Hanya residual branch | 0,3514 | 0,1833 | 24,10 | 0,7839 |
| Metode yang diusulkan | **0,2824** | **0,1205** | **25,05** | **0,8026** |

Pesannya jelas: kedua cabang saling melengkapi, dan hanya gabungan keduanya yang terbaik di semua metrik. Perlu diperhatikan bahwa keterangan (caption) di paper agak tertukar dengan label panel pada gambar. Berdasarkan angkanya, base branch unggul di persepsi dan residual branch unggul di distorsi dibanding satu sama lain.

### 3.1 Kompresi fitur laten
Perbedaan utama dari DiffEIC atau RDEIC: kedua metode itu memakai *entropy coding* atau hanya sebagian memakai VQ, sedangkan metode ini membangun bitstream **sepenuhnya lewat VQ**.

- Encoder menghasilkan laten *x* ∈ ℝ^(h×w×d) (tinggi, lebar, dan kanal).
- Laten didiskretisasi dengan **codebook** 𝒱 = {v[k]}, k = 1..K.
- **Persamaan (4):** setiap vektor *x_{i,j}* dipetakan ke entri codebook terdekat (jarak Euclidean kuadrat). *q_{i,j}* adalah indeksnya, dan *y_{i,j} = v[q_{i,j}]* adalah vektor hasil kuantisasi.

Berbeda dengan VQGAN yang dirancang untuk representasi, di sini **ukuran codebook dan resolusi laten disesuaikan dengan target bpp** supaya kompresi ekstrem dapat dicapai.

- **Persamaan (5):** BPP *B* dihitung dari jumlah −log₂ PMF(*y_{i,j}*) di semua posisi, dibagi *H × W*. PMF adalah fungsi massa probabilitas dari indeks codebook.

Konsekuensinya, laju bit bergantung langsung pada indeks terkuantisasi. Variasi bitrate antar-citra yang biasa muncul pada *entropy coding* menjadi sangat kecil. Bukti eksperimennya ada di Bagian 4.3.

### 3.2 Denoising satu langkah
Penulis memulai dengan menjelaskan mengapa ResShift tidak bisa dipakai begitu saja. Pada super-resolution, masukan LR masih memuat informasi persepsi yang cukup sehingga model dapat belajar membangkitkan detail frekuensi tinggi. Pada kompresi, masukan sudah sangat terkompresi, dan detail persepsi justru yang pertama hilang. Akibatnya, **menambah banyak langkah denoising sering menurunkan kualitas persepsi** (dibuktikan di Tabel 2). Karena itu, pendekatan terbaru memakai diffusion ringan dengan sedikit langkah (misalnya dua).

Berangkat dari temuan ini, penulis memakai **satu langkah saja** dan menyederhanakan Persamaan (2):
- **Persamaan (6)** (proses maju): *x̃* diperoleh dengan menggeser *x* ke arah *y* sebesar η_q(y − x), ditambah noise berskala κ²η_q.
- **Persamaan (7)** (proses mundur): *x̂* dihasilkan dari *f_θ(x̃, y)* dengan noise berskala κ²η_p.

Di sini *x̃* adalah fitur yang diberi noise, *x̂* adalah fitur hasil denoising, dan η_q, η_p adalah skala noise proses maju dan mundur.

Kalimat terakhir halaman ini memperkenalkan trik pelatihan: meskipun pelatihan satu langkah langsung sudah memuaskan, penulis menambah satu langkah ber-noise kecil hanya saat pelatihan agar lebih stabil dan general (dilanjutkan di halaman 5).

---

## Halaman 5 — Residual Fusion U-Net, Fungsi Loss, Rate-aware Noise, dan Awal Eksperimen

### Lanjutan skema 2 langkah
Model dilatih dengan diffusion **2 langkah**:
1. Satu langkah denoising perseptual dengan noise besar. Langkah ini adalah **satu-satunya** yang dipakai saat inferensi.
2. Satu langkah dengan noise sangat kecil, hanya saat pelatihan, supaya pelatihan tahan terhadap distorsi.

Hasilnya: rekonstruksi berkualitas tinggi, sementara inferensi tetap satu langkah (lihat Tabel 2).

### Residual Fusion U-Net
Denoising dari laten terkompresi menjaga semantik, tetapi dapat menimbulkan artefak di level piksel. Solusinya adalah strategi **fusi residual**:
- Laten terkompresi *y* diproses lewat dua cabang: **base** (*y*, konteks tingkat rendah) dan **residual** (*e = x̃ − y*, struktur yang hilang saat kompresi).
- Keduanya digabung oleh adapter *A(·)* menjadi representasi terpadu *z*.
- U-Net *U(·)* memperhalus *z* dan memulihkan detail halus.

Secara matematis:
- **Persamaan (8):** *f_θ(x̃, y) = U(z)*.
- **Persamaan (9):** *z = A(x̃, y) = conv(concat(x̃ − y, y))*, yaitu penggabungan (concatenate) lalu satu lapisan konvolusi.

Keluaran akhirnya masuk ke decoder untuk menghasilkan *X̂ = 𝒟(x̂)*. Desain ini menyeimbangkan efisiensi kompresi, akurasi semantik, dan kualitas persepsi.

### Fungsi loss (Persamaan 10)
Model dilatih *end-to-end* dengan empat suku:
1. ‖X − X̂‖²: error rekonstruksi di ruang piksel.
2. λ · LPIPS(X, X̂): loss persepsi untuk meningkatkan kualitas visual.
3. ‖sg(x) − y‖²: menarik codebook/hasil kuantisasi *y* mendekati laten *x*.
4. β · ‖sg(y) − x‖²: menarik laten *x* mendekati hasil kuantisasi *y*.

Operator **sg(·)** (*stop-gradient*) menahan aliran gradien pada salah satu sisi, mirip dengan cara pelatihan VQ-VAE. Suku 3 dan 4 disebut loss struktural pada representasi semantik dan terkompresi.

### 3.3 Rate-aware Noise Modulation
**Masalah:** pada diffusion konvensional, jadwal noise tetap untuk semua bitrate, sehingga untuk bitrate rendah jumlah langkah harus ditambah. Itu meningkatkan biaya komputasi dan memperlambat decoding.

**Temuan (Gambar 5):** Grafik kiri memperlihatkan LPIPS terhadap parameter η untuk ukuran codebook 64, 256, 2048, dan 8192 (codebook lebih besar berarti bpp lebih tinggi). Grafik kanan memperlihatkan bahwa nilai η optimal (yang meminimalkan LPIPS) **bergeser turun** ketika codebook membesar. Alasannya, pada bitrate tinggi galat kuantisasi lebih kecil sehingga denoising yang diperlukan lebih lemah.

**Hubungan empiris (Persamaan 11):** η_q ∝ 1/B. Nilai η optimal berbanding terbalik dengan bpp.

**Cara pakai:** saat inferensi, η_q diatur menurut bitrate. Pada bitrate rendah (informasi sedikit), noise yang lebih kuat disuntikkan sehingga model melakukan koreksi satu langkah yang lebih besar. Jadi, kompensasi detail dilakukan tanpa menambah iterasi.

### Awal Bab 4 (Eksperimen)
Pelatihan memakai **ImageNet** dengan *random crop* 256×256.

---

## Halaman 6 — Perbandingan dengan Metode Terkini (Kuantitatif)

### Gambar 6 (kurva rate-distortion)
Empat panel menampilkan LPIPS dan DISTS pada dataset Kodak dan CLIC2020 terhadap bpp, untuk BPG, ELIC, HiFiC, MS-ILLM, CDC, DiffEIC, DiffPC, RDEIC-2, RDEIC-5, dan metode yang diusulkan. Untuk kedua metrik, semakin kecil dan semakin ke kiri semakin baik. Kurva metode yang diusulkan (bintang merah) berada paling bawah pada bitrate rendah.

### Pengaturan evaluasi
- **Dataset:** Kodak dan CLIC2020. Untuk CLIC2020, sisi terpendek gambar diubah menjadi 768 piksel lalu dipotong tengah (*center crop*).
- **Metrik:** LPIPS (backbone VGG, konfigurasi sama dengan pelatihan) dan DISTS (dari pustaka PyIQA).
- **Pembanding:** BPG, ELIC, HiFiC, MS-ILLM, CDC, DiffEIC, DiffPC, dan RDEIC. Semua dijalankan ulang dengan implementasi resmi bila tersedia. Pengecualian: DiffPC memakai angka dari paper aslinya karena kodenya tidak tersedia publik.

### 4.2 Hasil kuantitatif
Metode ini **konsisten mengungguli** semua pembanding pada bitrate ultra-rendah (di bawah 0,05 bpp). Pada rentang bitrate lain, kualitasnya **sebanding**, bukan lebih unggul. Hasil PSNR dan MS-SSIM ada di Gambar 8.

### Tabel 1 (BD-Rate dan waktu)
BD-Rate menyatakan penghematan bitrate rata-rata pada kualitas yang sama. Nilai negatif berarti lebih hemat dibanding acuan (CDC).

| Model | Parameter | BD-Rate LPIPS | BD-Rate DISTS | Encoding (s) | Decoding (s) |
|---|---|---|---|---|---|
| DiffEIC | 1,4B | −25,22 | −43,04 | 0,801 | 12,502 |
| RDEIC-5 | 1,4B | −39,54 | −50,84 | 0,965 | 1,248 |
| **Ours** | **210M** | **−45,65** | −48,23 | 0,136 | **0,253** |

Poin penting:
- BD-Rate LPIPS **terbaik** (−45,65%), dan DISTS **terbaik kedua** setelah RDEIC-5.
- Dibanding DiffEIC, decoding sekitar **50× lebih cepat** (12,502 s menjadi 0,253 s).
- Dalam kelompok VAE/GAN, MS-ILLM masih sedikit lebih cepat dalam decoding (0,234 s).
- Waktu diukur pada NVIDIA TITAN RTX.

### Hasil kualitatif
Gambar 1 dan Gambar 7 menunjukkan bahwa metode berbasis VAE seperti ELIC menghasilkan gambar yang terlalu halus. Dibanding MS-ILLM (GAN) dan DiffEIC, metode ini lebih setia pada gambar asli, dengan struktur dan detail persepsi yang lebih kaya pada bpp yang sama atau lebih rendah.

### Awal 4.3 (Analisis bitrate-langkah)
Penulis menganalisis hubungan antara varians bitrate keluaran dan jumlah langkah diffusion yang optimal. Subbagian pertama: **bitrate keluaran dapat diprediksi** (dilanjutkan di halaman 7).

---

## Halaman 7 — Gambar 7, Tabel 2, dan Analisis Bitrate-Langkah

### Gambar 7 (contoh kualitatif CLIC2020)
Dua contoh (mesin espresso dan fasad bangunan) dengan kotak merah sebagai area perbesaran:
- Contoh espresso: Ours 0,2657 LPIPS / 0,0300 bpp / 3,88 fps, lebih baik dan lebih hemat dibanding ELIC (0,3198 / 0,0465 bpp), MS-ILLM (0,2878 / 0,0467 bpp), dan DiffEIC (0,3004 / 0,0354 bpp, hanya 0,075 fps).
- Contoh fasad: Ours 0,1349 LPIPS / 0,0379 bpp / 3,91 fps, sedikit di bawah DiffEIC secara LPIPS (0,1424 / 0,0373 bpp) tetapi puluhan kali lebih cepat (0,079 fps pada DiffEIC).

### Tabel 2 (jumlah langkah denoising pada 0,0294 bpp)
| Metode | LPIPS | DISTS | PSNR (dB) | MS-SSIM |
|---|---|---|---|---|
| 1 langkah | 0,312 | 0,127 | 22,03 | 0,752 |
| 2 langkah | 0,316 | 0,130 | 22,21 | 0,758 |
| 5 langkah | 0,317 | 0,129 | 22,17 | 0,753 |
| 15 langkah | 0,326 | 0,138 | 22,15 | 0,751 |
| 30 langkah | 0,323 | 0,136 | 22,10 | 0,749 |
| 50 langkah | 0,319 | 0,134 | 21,94 | 0,742 |
| **Ours** | **0,309** | **0,122** | 22,19 | **0,767** |

Pelajaran dari tabel:
- Menambah langkah **tidak** memperbaiki kualitas persepsi, bahkan LPIPS memburuk dari 1 langkah ke 15 langkah.
- Skema "Ours" (latihan 2 langkah, inferensi 1 langkah) unggul di LPIPS, DISTS, dan MS-SSIM. PSNR-nya hampir setara dengan 2 langkah (22,19 vs 22,21).

### 4.3 Bitrate keluaran dapat diprediksi
Pada Gambar 9(a), DiffEIC memiliki varians bitrate keluaran yang jauh lebih besar daripada metode ini untuk target bitrate yang sama. Penyebabnya, DiffEIC memakai entropy coding, sehingga bitrate aktual berfluktuasi sesuai kompleksitas gambar. Varians besar ini mempersulit penerapan pada skenario dengan bandwidth terbatas (misalnya sistem nirkabel). VQ membatasi bitrate dengan ketat sehingga bpp akurat diprediksi.

### Diffusion satu langkah yang adaptif
Pada DiffEIC, gambar-gambar dalam satu dataset jatuh ke rentang bitrate rendah, menengah, atau tinggi tergantung kompleksitas isinya (DiffEIC-L, -M, -H, semuanya dari model yang sama). Jumlah langkah terbaik berbeda: sekitar 20 langkah untuk L, 30 untuk M, dan 50 untuk H. Dengan demikian, jumlah langkah harus disesuaikan per gambar.

*Catatan:* paper menyimpulkan bahwa langkah optimal bertambah seiring turunnya bitrate. Padahal angka yang dilaporkan (L = 20, H = 50 langkah) tampak menunjukkan arah sebaliknya, sehingga bagian ini sebaiknya dikutip dengan hati-hati, atau cukup disimpulkan bahwa langkah optimal berbeda antar-gambar.

---

## Halaman 8 — Gambar 8 dan 9, Studi Ablasi, dan Kesimpulan

### Lanjutan 4.3
DiffEIC-All (rata-rata semua subset) baru mencapai performa terbaik pada 50 langkah. Itu berarti langkah tambahan yang tidak perlu untuk gambar yang sederhana, dan dapat menghasilkan kompresi yang kurang optimal. Sebaliknya, metode ini memakai **denoising satu langkah yang konsisten**, dengan tingkat noise disesuaikan menurut target bitrate. Pipeline inferensi lebih sederhana, dan hasilnya lebih baik dari DiffEIC pada target bitrate yang sama (Gambar 9(b)).

### Gambar 8 (PSNR dan MS-SSIM pada Kodak)
Grafik ini melengkapi Gambar 6 dengan metrik distorsi. Kurva metode ini kompetitif dan berada dalam kelompok atas pada bitrate rendah.

### Gambar 9
- **(a) Varians bitrate keluaran:** distribusi bitrate (digeser rata-ratanya) DiffEIC melebar, sedangkan distribusi metode ini sangat runcing, tanda bitrate yang terkendali.
- **(b) Langkah diffusion optimal:** kurva LPIPS terhadap jumlah langkah untuk DiffEIC-L, -M, -H, dan -All, dibandingkan titik "Ours-1step" yang mencapai LPIPS lebih rendah hanya dengan satu langkah. Tanda centang menunjukkan titik terbaik tiap subset.

### 4.4 Studi ablasi (Gambar 10, pada Kodak)
Ablasi menguji kontribusi tiap komponen:
1. **VQ-Residual Training.** Jika dinonaktifkan, performa turun signifikan, terutama pada bitrate rendah, dengan LPIPS naik tajam. Ini menunjukkan pentingnya representasi residual yang koheren secara struktural.
2. **Residual Branch.** Jika dihapus, LPIPS lebih buruk di semua bitrate. Cabang ini penting untuk merekonstruksi struktur tingkat tinggi yang sering hilang saat kompresi.
3. **Base Branch.** Jika dihapus, performa turun terutama pada bitrate menengah sampai tinggi. Ini menunjukkan bahwa penghalusan persepsi lewat denoising penting untuk detail dan fidelitas visual.

Model lengkap mencapai LPIPS terbaik di semua bitrate, sehingga kombinasi kedua cabang plus VQ-Residual training terbukti krusial.

Penulis juga mengulas Tabel 2: skema 2 langkah yang diusulkan mengungguli denoising satu langkah naif dan pendekatan 15 langkah yang mahal. Model 2 langkah murni meningkatkan distorsi (PSNR, MS-SSIM) tetapi mengorbankan persepsi (LPIPS dan DISTS naik). Metode ini menyatukan tujuan distorsi dan persepsi dalam satu langkah inferensi.

### Bab 5 (Kesimpulan)
Paper mengusulkan diffusion satu langkah untuk kompresi gambar perseptual pada bitrate ultra-rendah. Kerangkanya menggabungkan VQ-Residual training (rekonstruksi detail) dan rate-aware noise modulation. Berkat varians bitrate VQ yang rendah, dihasilkan bitrate yang dapat diprediksi dan rekonstruksi berkualitas tinggi dengan proses satu langkah tetap. Fidelitas persepsi kompetitif dengan decoding lebih dari 50× lebih cepat dibanding codec diffusion sebelumnya, sehingga lebih praktis untuk aplikasi nyata yang dibatasi bandwidth.

---

## Halaman 9 dan 10 — Daftar Pustaka

Dua halaman terakhir berisi 44 referensi. Kelompok yang paling relevan untuk memahami paper:

| Kelompok | Contoh referensi | Peran dalam paper |
|---|---|---|
| Codec tradisional | JPEG [38], JPEG2000 [9], BPG [4], HEVC [34], VVC [6] | Pembanding klasik |
| Learned codec berbasis VAE | Ballé dkk. [2, 3], Minnen dkk. [25], Cheng dkk. [8], ELIC [14] | Pendekatan yang menghasilkan gambar terlalu halus |
| Codec berbasis GAN | Agustsson dkk. [1], HiFiC [24], MS-ILLM [27] | Meningkatkan persepsi, tetapi tekstur kadang tidak stabil |
| Codec berbasis diffusion | CDC [41], PerCo [7], DiffEIC [20], DiffPC [40], RDEIC [21], HDCompression [23] | Pembanding utama |
| Dasar diffusion dan percepatannya | DDPM [15], DDIM [32], DPM-Solver [22], progressive distillation [31], DMD [42], consistency models [33], EDM [19] | Latar belakang teori |
| Dasar teknik yang dipakai | ResShift [43], VQ (van den Oord dkk.) [37], VQGAN [13], latent diffusion [30] | Dasar rancangan metode |
| Dataset dan metrik | ImageNet [10], Kodak [12], CLIC2020 [36], LPIPS [44], DISTS [11], MS-SSIM [39], BD-Rate [5] | Pelatihan dan evaluasi |

*Catatan:* referensi [26] dan [27] pada daftar pustaka berisi karya yang sama (Muckley dkk., ICML 2023), dan di teks keduanya merujuk ILLM. Ini hanya duplikasi entri, tidak memengaruhi isi paper.

---

## Ringkasan Satu Paragraf

Paper ini menjawab masalah bahwa codec diffusion memberi kualitas persepsi bagus pada bitrate ultra-rendah tetapi sangat lambat. Penulis memakai VQ untuk menghasilkan bitstream dengan ukuran yang dapat diprediksi, memisahkan informasi menjadi cabang base dan residual yang digabung dalam U-Net, melatih model dengan skema dua langkah tetapi hanya memakai satu langkah saat inferensi, dan mengatur kekuatan noise menurut bitrate. Hasilnya: kualitas persepsi kompetitif (terbaik pada bpp di bawah 0,05), model lebih kecil (210M vs 1,4B parameter), dan decoding sekitar 50× lebih cepat daripada DiffEIC.
