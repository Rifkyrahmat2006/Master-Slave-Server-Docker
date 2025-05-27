# Master Slave Server
Remote server menggunakan program C ke server mongodb dengan konfigurasi master dan slave. Berikut langkah-langkahnya:

## 1. Siapkan Resources:
* Windows (10/11) sebagai Host
* Virtual Box
* ISO Ubuntu Server sebagai Primary
* ISO Ubuntu Server sebagai Secondary
* [MSYS2](https://github.com/msys2/msys2-installer/releases/download/2025-02-21/msys2-x86_64-20250221.exe)
* [CMake](https://github.com/Kitware/CMake/releases/download/v4.0.2/cmake-4.0.2-windows-x86_64.msi)
* [Mongo C Driver](https://github.com/mongodb/mongo-c-driver/releases/download/2.0.1/mongo-c-driver-2.0.1.tar.gz)

## 2. Instalasi
### Ubuntu Server

Untuk OS Linux yang support: 
- Ubuntu Server Oracular 24.10
- Ubuntu Server Noble 24.04 (LTS)
- Ubuntu Server Jammy 22.04 (LTS)

Gunakan Ubuntu Server Jammy 22.04 (LTS). Setup menggunakan VirtualBox dengan konfigurasi jaringan ter-Bridge

### Install MSYS2 MINGW64 (dengan GCC 15.1.0).
### Install Mongo C Driver
Unduh dan Ekstrak Source Code MongoDB C Driver:
- Ekstrak ke direktori di Windows, misalnya C:\mongo-c-driver-2.0.1.
- Buka MSYS2 MINGW64 Shell di Windows Anda.

Navigasi ke Direktori Source Code Driver:
```
cd /c/mongo-c-driver-2.0.1
```
Buat Direktori Build Bersih:
```
mkdir cmake-build
cd cmake-build
```
Jalankan CMake untuk Mengkonfigurasi Build:
```
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=/c/mongodb_c_driver ..
```
Kompilasi Driver:
```
mingw32-make
```
Instal Driver:
```
mingw32-make install
```
## Menyiapkan Proyek Program C di Windows
1. Buat dan Masuk ke Direktori Proyek Baru(Anda bisa menggunakan direktori lainnya):
Buka MSYS2 MINGW64 Shell dan masuk. Contoh:
```
cd /c/Users/rifky/Documents/ProyekMongoReplicaSet
```
2. Clone Repo Ini
```
git clone 
```
## Mengkompilasi Program C
1. Pastikan Anda Masih Berada di Direktori Proyek. Contoh:
(/c/Users/rifky/Documents/ProyekMongoReplicaSet) di MSYS2 MINGW64 shell.
2. Jalankan Perintah Kompilasi GCC:
```
gcc kirim_data_final.c -o kirim_data_final.exe \
    -I/c/mongodb_c_driver/include/mongoc-2.0.1 \
    -I/c/mongodb_c_driver/include/bson-2.0.1 \
    -L/c/mongodb_c_driver/lib \
    -lmongoc2 -lbson2 \
    -lws2_32 -lsecur32 -lcrypt32 -lbcrypt -ldnsapi -lzstd
```
Jika berhasil, 'kirim_data_final.exe' akan dibuat.

## Menyiapkan File DLL yang Diperlukan
Pastikan anda masih di direktori yang sama (/c/Users/rifky/Documents/ProyekMongoReplicaSet) 
```
# Cek nama file DLL yang benar di /c/mongodb_c_driver/bin/
cp /c/mongodb_c_driver/bin/libmongoc2.dll . 
cp /c/mongodb_c_driver/bin/libbson2.dll .
# Salin DLL dependensi dari MinGW
cp /mingw64/bin/libzstd.dll .
cp /mingw64/bin/zlib1.dll .
```






 
