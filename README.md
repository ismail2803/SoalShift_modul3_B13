
[Soal 1](#soal-1)  
[Soal 2](#soal-2)  
[Soal 3](#soal-3)  
[Soal 4](#soal-4)  
[Soal 5](#soal-5)  


## Soal 1
Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
./faktorial 5 3 4  
3! = 6  
4! = 24  
5! = 120  

#### Penyelesaian
```
int arc = argc-1;
pthread_t tid[arc];
int n[100];
```
inisialisasi variabel
```
for (int i=1; i<argc; i++){
    n[i-1] = atoi(argv[i]);
}
```
Menyimpan setiap argumen yang ada ke dalam array of integer n
```
for (int i=0; i<arc-1; i++){
    for (int j=0; j<arc-i-1; j++){
        if(n[j] > n[j+1]){
            int temp = n[j];
            n[j] = n[j+1];
            n[j+1] = temp;
        }
    }
}
```
mengurutkan isi dari array n
```
while(t<arc){
   err=pthread_create(&(tid[t]),NULL,&factorial,(void*) &n[t]);
   pthread_join(tid[t],NULL);
   t++;
}
```
Membuat thread sebanyak argumen/isi dari array n. setiap argumen akan ditangani oleh 1 thread. Thread berikutnya akan dibuat ketika thread sebelumnya telah selesai berjalan. saat thread dibuat fungsi factorial akan dipanggil dan nilai dari array ke n akan dipassingkan.
```
void *factorial(void *args){
    int *n = (int *) args;
    int c;
    long long int result=1;

    for (c=1; c<=*n; c++)
        result = result*c;

    printf("%d! = %lld\n", *n, result);
}
```
fungsi factorial
```
int *n = (int *) args;
```
Mengubah tipe data args dari void* menjadi int* dan menyimpan nilainya dalam varibel integer *n
```
for (c=1; c<=*n; c++)
    result = result*c;
``` 
Menghitung nilai factorial dari *n
```
printf("%d! = %lld\n", *n, result);
```
Print hasil factorial *n

## Soal 2
Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
1.  Terdapat 2 server: server penjual dan server pembeli
2.  1 server hanya bisa terkoneksi dengan 1 client
3.  Server penjual dan server pembeli memiliki stok barang yang selalu sama
4.  Client yang terkoneksi ke server penjual hanya bisa menambah stok    
- Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
5.  Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
-  Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1
6.  Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
-  Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
-  Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
7.  Server penjual akan mencetak stok saat ini setiap 5 detik sekali
8.  Menggunakan thread, socket, shared memory

#### Penyelesaian
Untuk Soal ini terdapat 4 program file yaitu file penjual untuk server penjual, file pembeli untuk server pembeli, file cpembeli untuk client yang akan konek ke server pembeli, dan file cpenjual untuk client yang akan konek ke server penjual.

**Server**
```
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
}
```
Membuat socket
```
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
	perror("setsockopt");
	exit(EXIT_FAILURE);
}
```
Membantu dalam memanipulasi opsi untuk socket yang dirujuk oleh file descriptor sofckd. membantu dalam penggunaan kembali alamat dan port (opsional).
```
if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
}
```
fungsi bind mengikat soket ke alamat dan nomor port yang ditentukan dalam addr (struktur data khusus).
```
if (listen(server_fd, 3) <  0) {
	perror("listen");
	exit(EXIT_FAILURE);
}
```
membuat socket server dalam mode pasif di mana ia menunggu klien untuk mendekati server untuk membuat koneksi.
```
if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
    perror("accept");
    exit(EXIT_FAILURE);
}
```
mengekstrak permintaan koneksi pertama pada antrian koneksi yang tertunda untuk socket listen, server_fd, membuat soket baru, dan mengembalikan deskriptor file baru mengacu pada soket itu.

**Klien**
```
if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
}
```
Mengkonversi alamat IPv4 dan IPv6 dari text ke binary form
```
if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    return -1;
}
```
Panggilan sistem connect () menghubungkan soket yang dirujuk oleh file descriptor sock ke alamat yang ditentukan oleh serv_addr.  Alamat dan port server ditentukan dalam serv_addr.

**file server penjual**
```
key_t key = 1234;

int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stok = shmat(shmid, NULL, 0);
*stok = 0;
```
membuat shared memori yang nanti akan digunakan bersama dengan server pembeli
```
int err;
pthread_t thread1;
err=pthread_create(&thread1, NULL, &print_stok, NULL);
```
membuat thread yang akan menjalankan fungsi print_stok
```
void *print_stok(void *args){
    while(1){
        printf("Stok saat ini %d\n", *stok);
        sleep(5);
    }
}
```
fungsi print_stok digunakan untuk print jumlah stok setiap 5 detik. fungsi ini akan berjalan dalam sebuah thread.
```
while(1){
    char buffer[1024] = {0};
    valread = read( new_socket , buffer, 1024);
    if(strcmp(buffer, "tambah")==0)
        *stok = *stok + 1;
}
```
looping untuk membaca jika ada pesan yang dikrim oleh client, jika client mengirim pesan tambah maka stok akan bertambah 1.

**file server pembeli**
```
key_t key = 1234;
int *stok;

int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stok = shmat(shmid, NULL, 0);
```
shared memori yang digunakan bersama dengan server penjual
```
while(1){
    char buffer[1024] = {0};
    valread = read( new_socket , buffer, 1024);
    if(*stok == 0)
    {
        send(new_socket , habis , strlen(habis) , 0 );
    }
    else
    {
        if(strcmp(buffer, "beli")==0){
            *stok = *stok - 1;
            send(new_socket , ada , strlen(ada) , 0 );
        }
    }   
}
```
looping untuk membaca jika ada pesan dari client pembeli. jika stok 0 maka server akan mengirimkan pesan(habis) yang berisi Transaksi Gagal. jika stok lebih dari 0 maka pesan dari client akan dicek lagi, jika pesannya adalah "beli" maka stok berkurang 1 dan mengirimkan pesan "Transaksi Berhasil".

**file client cpenjual**
```
while(1){
	scanf("%s", pesan);
	send(sock , pesan , strlen(pesan) , 0 );
}
```
looping untuk menunggu input dan mengirimkannya ke server penjual.

**file client cpembeli**
```
while(1){
    char buffer[1024] = {0};
    scanf("%s", pesan);
    send(sock , pesan , strlen(pesan) , 0 );
    valread = read( sock , buffer , 1024);
    printf("%s\n", buffer);
}
```
looping untuk menunggu input dan mengirimkannya ke server, dalam loop ini juga terdapat valread untuk membaca pesan yang dikrim oleh server pembeli

## Soal 3
Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
1.  Terdapat 2 karakter Agmal dan Iraj    
2.  Kedua karakter memiliki status yang unik
-  Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
-	Iraj memiliki Spirit_Status,  di awal program memiliki status 100
-	Terdapat 3 Fitur utama    
	- All Status, yaitu menampilkan status kedua sahabat
Ex: Agmal WakeUp_Status = 75
Iraj Spirit_Status = 30
	- “Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
	- “Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
-  Terdapat Kasus yang unik dimana:
	- Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
	- Jika Fitur “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
-  Program akan berhenti jika Salah Satu :
	- WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)  
	- Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)
-  Syarat Menggunakan Lebih dari 1 Thread

#### Penyelesaian
```
int fitur, status;
int wakeup_status=0;
int spirit_status=100;
int Agmal3=0;
int Iraj3=0;
```
inisialisasi varibael global
```
pthread_t thread1, thread2;
int  iret1, iret2;

iret1 = pthread_create( &thread1, NULL, &Agmal, NULL);
iret2 = pthread_create( &thread2, NULL, &Iraj, NULL);
```
inisialisasi variabel dan membuat 2 thread yang akan menghandle fungsi Agmal dan Iraj
```
if(wakeup_status >= 100)
{
    printf("\nAgmal Terbangun,mereka bangun pagi dan berolahraga\n");
    break;
}
if(spirit_status <= 0)
{
    printf("\nIraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
    break;
}
``` 
Mengecek apakah nilai wakeup_status >= 100 atau spirit_status <= 0, jika salah satu dari syarat tersebut terpenuhi maka program akan berhenti
```
printf("\nFitur :\n");
printf("1. All Status\n");
printf("2. Agmal Ayo bangun\n");
printf("3. Iraj Ayo Tidur\n");
scanf("%d", &fitur);
```
Meminta input berupa integer sesuai fitur
```
if(fitur == 1)
{
    printf("\nAgmal WakeUp_Status = %d\n", wakeup_status);
    printf("Iraj Spirit_Status = %d\n", spirit_status);
    status = 1;
}
```
Mengecek apakah inputan fitur adalah 1, jika iya maka nilai wakeup_status dan spirit_status akan diprint
```
while(status ==  0)
{

}
```
looping untuk menunggu program sebelumnya telah selesai dijalankan
```
void *Agmal(void *args){
    while(1)
    {
        if(fitur == 2)
        {
            wakeup_status = wakeup_status + 15;
            printf("\nwakeup_status Agmal %d\n", wakeup_status);
            Agmal3 = Agmal3 + 1;
            Iraj3 = 0;
            fitur = 0;
            status = 1;
        }
        if(Iraj3 == 3)
        {
            printf("\nAgmal Ayo Bangun disabled 10 s\n");
            sleep(10);
            Iraj3 = 0;
        }
    }
}
```
setelah fungsi ini dipanggil maka akan terus berjalan pada thread.
jika inputan fitur adalah 2 maka wakeup_status Agmal akan bertambah 15, nilai fitur akan diset menjadi 0 agar program if ini tidak berjalan terus menerus hingga nilai fitur telah di inputkan kembali. variabel Agmal3 untuk mengecek jika fitur ini telah dijalankan 3 kali maka fungsi Iraj akan disleep selama 10 detik.
```
void *Iraj(void *args){
    while(1)
    {
        if(fitur == 3)
        {
            spirit_status = spirit_status - 20;
            printf("\nspirit_status Iraj %d\n", spirit_status);
            Iraj3 = Iraj3 + 1;
            Agmal3 = 0;
            fitur = 0;
            status = 1;
        }
        if(Agmal3 == 3)
        {
            printf("\nFitur Iraj Ayo Tidur disabled 10 s\n");
            sleep(10);
            Agmal3 = 0;
        }
    }
}
```
cara kerja fungsi Iraj kurang lebih hampir sama seperti fungsi Agmal

## Soal 4
Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip

Dengan Syarat :
-  Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
-  Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
-  Ketika Mengekstrak file .zip juga harus secara bersama-sama
-  Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
-  Wajib Menggunakan Multithreading
-  Boleh menggunakan system

#### Penyelesaian
```
pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
int  iret1, iret2, iret3, iret4, iret5, iret6;
int proses1 = 1;
int proses2 = 2;
```
inisialisasi variabel
```
iret1 = pthread_create( &thread1, NULL, &ps, (void*) &proses1);
iret2 = pthread_create( &thread2, NULL, &ps, (void*) &proses2);
pthread_join(thread1,NULL);
pthread_join(thread2,NULL);
```
membuat 2 thread yang memanggil fungsi yang sama(fungsi ps) dengan argumen berbeda. pthread join akan membuat program dibawahnya belum dijalankan selama threadnya belum selesai.
```
void *ps(void *args){
    int *proses = (int *) args;
    if(*proses == 1)
        system("ps -aux | head -10 > /home/ismail/Documents/FolderProses1/SimpanProses1.txt");
    if(*proses == 2)
    system("ps -aux | head -10 > /home/ismail/Documents/FolderProses2/SimpanProses2.txt");
}
```
fungsi ini akan dijalankan oleh 2 thread yang telah dibuat sebelumnya. masing-masing thread akan menjalankan program yang berbeda sesuai dengan argumen yang telah dipassing. kedua thread ini berjalan secara bersamaan. fungsi ini untuk mendapatkan 10 list proses dan memasukkannya ke dalam sebuah file.

```
iret3 = pthread_create( &thread3, NULL, &zip, (void*) &proses1);
iret4 = pthread_create( &thread4, NULL, &zip, (void*) &proses2);
pthread_join(thread3,NULL);
pthread_join(thread4,NULL);
```
setelah thread sebelumnya telah selesai berjalan maka program akan membuat 2 thread baru lagi untuk memanggil fungsi zip dengan argumen yang berbeda dan menambahkan join thread agar program dibawahnya dijalankan setelah thread ini selesai.
```
void *zip(void *args){
    int *proses = (int *) args;
    if(*proses == 1)
        system("zip -jm /home/ismail/Documents/FolderProses1/KompresProses1.zip /home/ismail/Documents/FolderProses1/SimpanProses1.txt");
    if(*proses == 2)
        system("zip -jm /home/ismail/Documents/FolderProses2/KompresProses2.zip /home/ismail/Documents/FolderProses2/SimpanProses2.txt");
    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);
}
```
fungsi zip digunakan untuk mengzip file yang telah dibuat pada thread yang pertama kali dijalankan. 2 thread yang telah dibuat akan menjalankan program sesuai dengan argumennya.
```
iret5 = pthread_create( &thread5, NULL, &unzip, (void*) &proses1);
iret6 = pthread_create( &thread6, NULL, &unzip, (void*) &proses2);
pthread_join(thread5,NULL);
pthread_join(thread6,NULL);
```
membuat 2 thread terakhir untuk memanggil fungsi unzip dengan argumen yang berbeda.
```
void *unzip(void *args){
    int *proses = (int *) args;
    if(*proses == 1)
        system("unzip /home/ismail/Documents/FolderProses1/KompresProses1.zip -d /home/ismail/Documents/FolderProses1/");
    if(*proses == 2)
        system("unzip /home/ismail/Documents/FolderProses2/KompresProses2.zip -d /home/ismail/Documents/FolderProses2/");
}
```
fungsi ini akan dijalankan oleh 2 thread dan akan mengunzip file sesuai dengan argumen yang dipassing.

## Soal 5
Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta.
1.  Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat memberi nama pada monsternya.
2.  Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.
3.  Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).
4.  Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.
5.  Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian.
6.  Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
-   Pembeli (terintegrasi dengan game)
	- Dapat mengecek stok makanan yang ada di toko.
	- Jika stok ada, pembeli dapat membeli makanan.
-   Penjual (terpisah)
	- Bisa mengecek stok makanan yang ada di toko.
	- Penjual dapat menambah stok makanan.

Spesifikasi program:
1.  Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)
2.  Program terdiri dari 3 scene yaitu standby, battle, dan shop.
3.  Pada saat berada di standby scene, program selalu menampilkan health status, hunger status,  hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. Contoh :
Standby Mode
Health : [health status]
Hunger : [hunger status]
Hygiene : [hygiene status]
Food left : [your food stock]
Bath will be ready in [cooldown]s
Choices
	- Eat
	- Bath
	- Battle
	- Shop
	- Exit
4.  Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :
Battle Mode
Monster’s Health : [health status]
Enemy’s Health : [enemy health status]
Choices
	- Attack
	- Run
5.  Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. Contoh :
Shop Mode
Shop food stock : [shop food stock]
Your food stock : [your food stock]
Choices
	- Buy
	- Back
6.  Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :
Shop
Food stock : [shop food stock]
Choices
	- Restock
	- Exit
7.  Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))
