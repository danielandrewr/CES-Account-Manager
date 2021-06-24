# Program Account Manager (with Vigenere + Subtitution Box Encryption)
---
## Gambaran Umum Program

*Program Account Manager* adalah program yang berfungsi untuk menyimpan akun pengguna yang berisi:

1. Website dari akun tersebut
2. Email dari akun tersebut
3. Password dari akun tersebut

Jika sebuah data akun disimpan di dalam memori dalam bentuk plaintext, hal tersebut akan membahayakan pemilik karena datanya dapat dengan mudah dicuri oleh orang lain. Oleh karena itu, kami menawarkan solusi untuk mengenkripsi database dari akun tersebut sehingga jika terjadi pencurian, pencuri data tidak bisa mendapatkan informasi mengenai isi dari akun curian tersebut.

Struktur data yang digunakan untuk menyimpan akun master dan akun slave adalah Linked List dan Stack. Multithreading dan parallel programming digunakan saat mengenkripsi dan mendekripsi string serta terdapat library string.h yang telah kami modifikasi menjadi versi paralel dibutuhkan (membuat yang dibutuhkan saja).

---
## Penjelasan mengenai Code pada Program

### 1.  ```parallel_string.h```

Header ``` parallel_string.h``` adalah header modifikasi dari ```<string.h>``` yang kami buat agar

1. Dapat berjalan secara paralel, bukan sekuensial (pada fungsi-fungsi yang aslinya dapat diparalelkan).
2. Mengimpor fungsi (dengan cara membuatnya ulang) ```string.h``` yang hanya dibutuhkan saja sehingga menghemat memori.

Fungsi di bawah berguna untuk menghitung panjang dari sebuah string. Tidak dapat diparalelkan.
```c
//Fungsi strlen. Tidak bisa diparalelkan.
int my_strlen(const unsigned char * string) {
    register int i;
    for (i = 0; string[i] != '\0'; i++);
    return i;
}
```
Fungsi di bawah berguna untuk menyalin string *source* ke string *destination*. Dapat diparalelkan.
```c
//Fungsi strcpy yang diubah menjadi paralel
void my_strcpy(unsigned char * destination,
    const unsigned char * source) { 
    register int i, size_source = my_strlen(source);

    #pragma omp parallel for
    for (i = 0; i < size_source; i++) {
        destination[i] = source[i];
    }
    destination[size_source] = '\0';
}
```
Fungsi di bawah berguna untuk konkatenasi string.
```c
//Fungsi strcat yang diubah menjadi paralel
void my_strcat(unsigned char * destination,
    const unsigned char * source) {
    register int i;
    register int size_source = my_strlen(source);
    register int size_destination = my_strlen(destination);
    register int size_keduanya = size_source + size_destination;

    #pragma omp parallel for
    for (i = size_destination; i < size_keduanya; i++) {
        destination[i] = source[i - size_destination];
    }
    destination[size_keduanya] = '\0';
}
```
Fungsi di bawah berguna untuk membandingkan apakah suatu string memiliki elemen yang sama. Jika sama, maka akan me-*return* nilai 0.
```c
//Fungsi strcmp yang diubah menjadi paralel
int my_strcmp(const unsigned char * str1,
    const unsigned char * str2) {
    register int i, flag = 1;
    register int len_str1 = my_strlen(str1);

    #pragma omp parallel for shared(flag)
    for (i = 0; i < len_str1; i++) {
        if (str1[i] != str2[i]) {
            #pragma omp critical
            flag = 0;
        }
    }
    return !flag;
}
```
Fungsi di bawah berguna untuk melakukan pencocokan terhadap string tanpa harus sama indeks awalnya.
```c
/*
Fungsi strcasecmp, tetapi dapat digunakan jika kata yang dicari tidak persis.
Contohnya, ada "facebook.com", tetapi dengan mencari "Book" dapat ditemukan.
*/
int my_strcasestr(const unsigned char * string,
    const unsigned char * toFind) {
    register int slen = my_strlen(string);
    register int tFlen = my_strlen(toFind);
    register int found = 0, s, t;

    if (slen >= tFlen) {
        for (s = 0, t = 0; s < slen; s++) {
            do {

                if (string[s] == toFind[t] || string[s] == toFind[t] + 32 || string[s] + 32 == toFind[t]) {
                    if (++found == tFlen) return 0;
                    s++;
                    t++;
                } else {
                    s -= found;
                    found = 0;
                    t = 0;
                }

            } while (found);
        }
        return 1;
    } else return -1;
}
```
---
### 2.  ```get_input.h```
Header ``` get_inpuh.h``` adalah header buatan kami yang berguna untuk mendapatkan input input dengan tipe data *integer* dan string bertipe *unsigned char*.

Code di bawah untuk mendapatkan input berupa *integer* dengan tambahan error handling serta parameter batas bawah dan batas atas yang boleh diinput oleh user.
```c
//Fungsi yang berguna untuk membersihan STDIN.
int clean_stdin() {
    while (getchar() != '\n');
    return 1;
}
//Fungsi khusus integer yang akan mendeteksi jika diinput huruf.
void inputInteger(int * variabel) {
    unsigned char dummy;
    while ((scanf("%d%c", variabel, & dummy) != 2 || dummy != '\n') && clean_stdin()) {
        puts("(* - *)! Hanya boleh memasukkan angka. Silakan masukkan angka\n");
    }
}
//Fungsi yang mendeteksi besarnya input integer di atas.
void inputAngka(int * angka,
    const int batas_bawah,
        const int batas_atas) {
    do {
        inputInteger(angka);
        if ( * angka < batas_bawah) {
            printf("(> o <)! Angka minimal adalah %d. Masukkan angka dengan benar!\n", batas_bawah);
        } else if ( * angka > batas_atas) {
            printf("(> o <)! Angka maksimal adalah %d. Masukkan angka dengan benar!\n", batas_atas);
        }
    } while ( * angka < batas_bawah || * angka > batas_atas);
}
```
Lalu, code di bawah berfungsi untuk mendapatkan input string bertipe *unsigned char* dengan panjang minimum dan maksimum yang boleh diinput oleh user. *Overflow Handling* juga telah diterapkan.

```c
//Fungsi khusus untuk mendapatkan input string dengan batas bawah dan batas atas.
void inputString(unsigned char * string,
    const unsigned int batas_bawah,
        const unsigned int batas_atas) {
    register unsigned short int i, count = 0;
    unsigned char * input = malloc(256 * sizeof(unsigned char));
    do {
        scanf(" %255[^\n]hhu", input);
        fflush(stdin);
        count = my_strlen(input);
        if (count < batas_bawah) {
            printf("Input yang diberikan minimal %d karakter. Silakan input lagi\n\n", batas_bawah);
        } else if (count > batas_atas) {
            printf("Input yang diberikan maksimal %d karakter. Silakan input lagi\n\n", batas_atas);
        } else {
            my_strcpy(string, input);
        }
    } while (count < batas_bawah || count > batas_atas);
    free(input);
    input = NULL;
}
```
### 3.  ```md5.h```
Header ```md5.h``` kami dapatkan dari [MD5 by Manuel Rigger](https://github.com/pod32g/MD5/blob/master/md5.c). Kami edit sedikit agar memakai  ```parallel_string.h``` yang telah kami buat dan memparalelkan bagian yang menurut kami dapat diparalelkan. Kami juga mengedit tipe datanya sehingga tidak memerlukan library ```<stdint.h>```.

---

### 4. ```subbytes.h```
Header ```subbytes.h``` berisi konstanta yang berasal dari Rijndael S-box. Berguna untuk memperkuat enkripsi. Penjelasan mengenai Rijndael S-box dapat dilihat pada link [ini](https://en.wikipedia.org/wiki/Rijndael_S-box).

---

### 5. ```encipher.h```
Header ```encipher.h``` berisi fungsi yang berguna untuk mengenkripsi dan mendekripsi string dengan algoritma Vigenère Cipher. Penjelasan mengenai Vigenère Cipher dapat dilihat pada link [ini](https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher). Terdapat paralelisasi pada fungsi-fungsi di dalam header ini.

---
### 6. ```filehandler.h```
Header ```filehandler.h``` berisi ungsi untuk mencetak file database (tentunya yang telah terenkripsi ke dalam file .txt yang nantinya dapat digunakan lagi. 

---

Final Advanced Programming Project

This project is made by group 1 of Proglan 2 which consists of:

1. Josephus Daniel Andrew Roong (2006577284)
2. Kemas Rafly Omar Thoriq (2006577422)
3. Michael Harditya (2006577265)
4. Muhammad Naufal Faza (2006577252)

as final programming assignment in Even Semester 2020/2021 for course Advance Programming + Lab. 
(ENCE602003) in Undergraduate of Computer Engineering study program, Department of Electrical Engineering, Faculty of Engineering, Universitas Indonesia.
