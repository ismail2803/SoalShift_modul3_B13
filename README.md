
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
int fac[100];
long long int res[100];
int x = 0;
```
inisialisasi variabel global
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
int t = 0;
while(t<arc){
   err=pthread_create(&(tid[t]),NULL,&factorial,(void*) &n[t]);
   t++;
}
```
Membuat thread sebanyak argumen/isi dari array n. setiap argumen akan ditangani oleh 1 thread.
```
int tj = 0;
while(tj<arc){
   pthread_join(tid[tj],NULL);
   tj++;
}
```
join dengan thread yang telah dibuat agar program menunggu hingga semua thread telah selesai kemudian lanjut ke program dibawahnya
```
void *factorial(void *args){
    int *n = (int *) args;
    int c;
    long long int result=1;

    for (c=1; c<=*n; c++)
        result = result*c;
    
    fac[x] = *n;
    res[x] = result;
    x++;
}
```
fungsi factorial
```
int *n = (int *) args;
```
Mengubah tipe data args dari void* menjadi int* dan menyimpan nilainya dalam varibel integer *n
```
    fac[x] = *n;
    res[x] = result;
    x++;
```
menyimpan nilai factorial yang akan dicari dan hasilnya pada variabel global agar dapat diakses dari fungsi main
```
for (int i=0; i<arc-1; i++){
    for (int j=0; j<arc-i-1; j++){
        if(fac[j] > fac[j+1]){
             int temp = fac[j];
             fac[j] = fac[j+1];
             fac[j+1] = temp;
               
             long long int temps = res[j];
             res[j] = res[j+1];
             res[j+1] = temps; 
        }
    }
}
```
Mengurutkan nilai factorial yang ada di variabel array fac dan hasil factorialnya pada variabel array res
```
int pr = 0;
while(pr<arc){
   printf("%d! = %lld\n", fac[pr], res[pr]);
   pr++;
}
```
Print semua nilai factorial yang dicari dan hasilnya

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
            Agmal3 = Agmal3 + 1;
            fitur = 0;
            status = 1;
        }
        if(Iraj3 == 3)
        {
	    Iraj3 == 0;
            printf("\nAgmal Ayo Bangun disabled 10 s\n");
	    status = 1;
            sleep(10);
        }
    }
}
```
setelah fungsi ini dipanggil maka akan terus berjalan pada thread.
jika inputan fitur adalah 2 maka wakeup_status Agmal akan bertambah 15, nilai fitur akan diset menjadi 0 agar program if ini tidak berjalan terus menerus hingga nilai fitur telah di inputkan kembali. nilai status diubah menjadi 1 untuk menandakan bahwa program telah selesai berjalan. variabel Agmal3 untuk mengecek jika fitur ini telah dijalankan 3 kali maka fungsi Iraj akan disleep selama 10 detik.
```
void *Iraj(void *args){
    while(1)
    {
        if(fitur == 3)
        {
            spirit_status = spirit_status - 20;
            Iraj3 = Iraj3 + 1;
            fitur = 0;
            status = 1;
        }
        if(Agmal3 == 3)
        {
	    Agmal3 = 0;
            printf("\nFitur Iraj Ayo Tidur disabled 10 s\n");
	    status = 1;
            sleep(10);
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
int status1 = 0; 
int status2 = 0;
```
inisialisasi variabel global
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
iret3 = pthread_create( &thread3, NULL, &zip, (void*) &proses1);
iret4 = pthread_create( &thread4, NULL, &zip, (void*) &proses2);
iret5 = pthread_create( &thread5, NULL, &unzip, (void*) &proses1);
iret6 = pthread_create( &thread6, NULL, &unzip, (void*) &proses2);
```
membuat 6 thread yang akan berjalan secara bersamaan. 2 thread yang pertama akan menjalankan fungsi ps dengan argumen yang berbeda, 2 thread kedua akan menjalankan fungsi zip dengan argumen yang berbeda, 2 thread terakhir akan menjalankan fungsi unzip dengan argumen yang berbeda.
```
pthread_join(thread3,NULL);
pthread_join(thread4,NULL);
pthread_join(thread1,NULL);
pthread_join(thread2,NULL);
pthread_join(thread5,NULL);
pthread_join(thread6,NULL);
```
join dengan semua thread yang telah dibuat agar program ini menunggu hingga semua thread selesai berjalan
```
void *ps(void *args){
    int *proses = (int *) args;
    if(*proses == 1){
        system("ps -aux | head -10 > /home/ismail/Documents/FolderProses1/SimpanProses1.txt");
	status1 = 1;
    }
    if(*proses == 2){
    	system("ps -aux | head -10 > /home/ismail/Documents/FolderProses2/SimpanProses2.txt");
    	status2 = 1;
    }
}
```
fungsi ini akan dijalankan oleh 2 thread yang telah dibuat sebelumnya. masing-masing thread akan menjalankan program yang berbeda sesuai dengan argumen yang telah dipassing. kedua thread ini berjalan secara bersamaan. fungsi ini untuk mendapatkan 10 list proses dan memasukkannya ke dalam sebuah file. setelah thread ini selesai berjalan maka akan mengubah nilai status1 atau status 2 menjadi 1
```
void *zip(void *args){
    int *proses = (int *) args;
    
    while(status1 != 1 || status2 != 1){

    }
    
    if(*proses == 1 && status1 == 1){
        system("zip -jm /home/ismail/Documents/FolderProses1/KompresProses1.zip /home/ismail/Documents/FolderProses1/SimpanProses1.txt");
	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
        sleep(15);
	status1 = 2;
    }
    if(*proses == 2 && status2 == 1){
        system("zip -jm /home/ismail/Documents/FolderProses2/KompresProses2.zip /home/ismail/Documents/FolderProses2/SimpanProses2.txt");
	sleep(15);
        status2 = 2;
    }
}
```
fungsi zip digunakan untuk mengkompres ke zip(setelah dikompres file asli akan langsung dihapus), file txt yang telah dibuat pada thread yang pertama kali dijalankan. 2 thread yang telah dibuat akan menjalankan program sesuai dengan argumennya. Thread ini akan terus berjalan hingga nilai status1 atau status2 telah berubah menjadi 1. Setelah thread ini selesai berjalan akan mengubah nilai status1 atau status2 menjadi 2
```
void *unzip(void *args){
    int *proses = (int *) args;
    
    while(status1 != 2 || status2 != 2){
        
    }
    
    if(*proses == 1 && status1 == 2)
        system("unzip /home/ismail/Documents/FolderProses1/KompresProses1.zip -d /home/ismail/Documents/FolderProses1/");
    if(*proses == 2 && status2 == 2)
        system("unzip /home/ismail/Documents/FolderProses2/KompresProses2.zip -d /home/ismail/Documents/FolderProses2/");
}
```
fungsi ini akan dijalankan oleh 2 thread dan akan mengunzip file sesuai dengan argumen yang dipassing. Thread ini akan terus berjalan hingga nilai status1 atau status2 berubah menjadi 2

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

#### Penyelesaian
**Program Pembeli Terintegrasi Game**
```
int hunger_status = 7;
int hygiene_status = 100;
int healt_status = 300;
int healt_NPC = 100;
int fitur = 0;
int *stok;
int mystok = 0;
int act_standby = 0;
int mandi;
int act_food = 0;
int act_battle = 0;
int mati = 0;
```
inisialisasi variabel global
```
char nama[20];
pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8;
int iret1, iret2, iret3, iret4, iret5, iret6, iret7, iret8;
```
inisialisasi variabel
```
key_t key = 1234;
int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stok = shmat(shmid, NULL, 0);
```
shared memory agar nilai stok pada program penjual/game selalu sama dengan program penjual
```
printf("Berikan nama pada monstermu : \n");
scanf("%s", nama);
```
meminta input nama yang akan diberikan kepada monster
```
iret1 = pthread_create( &thread1, NULL, &hgr_status, NULL);
iret2 = pthread_create( &thread2, NULL, &hgn_status, NULL);
iret3 = pthread_create( &thread3, NULL, &hlt_status, NULL);
iret4 = pthread_create( &thread4, NULL, &battle, NULL);
iret5 = pthread_create( &thread5, NULL, &bath, NULL);
iret6 = pthread_create( &thread6, NULL, &standby, NULL);
iret7 = pthread_create( &thread7, NULL, &shop, NULL);
```
membuat thread yang akan menjalankan fungsi tertentu
```
fitur = 1
```
fitur yang akan dijelankan pertama kali adalah 1(standbye mode)
```
iret8 = pthread_create( &thread8, NULL, &input, NULL);
```
membuat thread yang akan menjalankan fungsi input
```
while(mati == 0){

}
```
program utama tidak akan berhenti hingga nilai mati berubah
```
void *hgr_status(void *args){
    while(1){
        if(fitur != 2)
        {
            sleep(10);
            hunger_status = hunger_status - 5;

            if(hunger_status <= 0)
            {
                printf("Hunger Anda dibawah 0\n");
                mati = 1;
            }
        }
    }
}
```
fungsi hgr_status akan selalu berjalan (dijalankan oleh thread). nilai hunger_status akan selalu berkurang 5 setiap 10 detik selama fitur yang berjalan saat ini bukan 2(fitur battle) dan fungsi ini akan mengubah nilai mati menjadi 1 ketika hunger_status kurang dari sama dengan 0
```
void *hgn_status(void *args){
    while(1){
        if(fitur != 2)
        {
            sleep(30);
            hygiene_status = hygiene_status - 10;

            if(hygiene_status <= 0)
            {
                printf("Hygiene Anda dibawah 0\n");
                mati = 1;
            }
        }
    }
}
```
fungsi hgn_status akan selalu berjalan (dijalankan oleh thread). nilai hygiene_status akan selalu berkurang 10 setiap 30 detik selama fitur yang berjalan saat ini bukan 2(fitur battle) dan fungsi ini akan mengubah nilai mati menjadi 1 ketika hygiene_status kurang dari sama dengan 0
```
void *hlt_status(void *args){
    while(1){
        if(fitur != 2)
        {
	    sleep(10);
            healt_status = healt_status + 5;
        }
    }
}
```
fungsi hlt_status akan selalu berjalan (dijalankan oleh thread). nilai health_status akan selalu bertambah 5 setiap 10 detik selama fitur yang berjalan saat ini bukan 2(fitur battle)
```
void *bath(void *args){
    while(1){
        if(act_standby == 2 && mandi == 1)
        {
            hygiene_status = hygiene_status + 30;
            mandi = 0;
            act_standby = 0;
            sleep(30);
        }
        mandi = 1;
    }
}
```
fungsi bath akan selalu berjalan (dijalankan oleh thread). nilai hygiene status akan bertambah 30 ketika nilai act_standbye sama dengan 1 dan mandi sama dengan 1. setelah nilai hygiene bertambah nilai mandi akan dirubah menjadi 0, setelah 30 detik nilai mandi akan dikembalikan menjadi 1.
```
void *standby(void *args){
    while(1){
        if(fitur ==  1)
        {
            printf("Standby Mode\n");
            printf("Health : [ %d ]\n", healt_status);
            printf("Hunger : [ %d ]\n", hunger_status);
            printf("Hygiene : [ %d ]\n", hygiene_status);
            printf("Food left : [ %d ]\n", mystok);
            if(mandi == 1)
                printf("Bath is ready\n");
            else if(mandi == 0)
                printf("Bath is not ready\n");
            // printf("Bath will be ready in [cooldown]s\n");
            printf("Choices\n");
            printf("1. Eat\n");
            printf("2. Bath\n");
            printf("3. Battle\n");
            printf("4. Shop\n");
            printf("5. Exit\n");

            if(mystok > 0 && act_standby == 1)
            {
                hunger_status = hunger_status + 15;
                act_standby = 0;
                mystok = mystok - 1;
                if(hunger_status > 200){
                    hunger_status = 200;
                }
            }
            if(act_standby == 3)
            {
                fitur = 2;
            }
            if(act_standby == 4)
            {
                fitur = 3;
            }
            if(act_standby == 5)
            {
                mati = 1;
            }
            sleep(1);
            system("clear");
        }
    }
}
```
fungsi standbye akan selalu berjalan (dijalankan oleh thread). ketika fitur yang dipilih adalah 1(standbye mode) menu stanbye akan ditampilkan dan diupdate setiap detik. pada fungsi ini kita dapat memilih beberapa menu, menu 1(eat) akan membuat hunger status bertambah 15 dengan syarat mystok lebih dari 0, menu 2(bath) akan membuat program fungsi Bath berjalan sepenuhnya, menu 3(battle) akan membuat mode berpindah dari stanbye(fungsi standbye) menjadi battle(fungsi battle), menu 4(shop) akan membuat mode berpindah dari stanbye(fungsi standbye) menjadi shop(fungsi shop), menu 5(exit) akan membuat program berhenti dengan mengubah nilai mati menjadi 1
```
void *battle(void *args){
    while(1){
        if(fitur == 2){
            printf("Battle Mode\n");
            printf("Monter's Health : [ %d ]\n", healt_status);
            printf("Enemy's Health : [ %d ]\n", healt_NPC);
            printf("Choices\n");
            printf("1. Attack\n");
            printf("2. Run\n");

            if(act_battle == 1)
            {
                healt_status = healt_status - 20;
                healt_NPC = healt_NPC - 20;
                act_battle = 0;
            }
            else if(act_battle == 2)
            {
                fitur = 1;
            }
            if(healt_status <= 0)
            {
                printf("You Kalah\n");
                mati = 1;
            }
            if(healt_NPC <= 0){
                printf("You menang\n");
                healt_NPC = 100;
                fitur = 1;
            }
            sleep(1);
            system("clear");
        }
    }
}
```
fungsi battle akan selalu berjalan (dijalankan oleh thread). ketika fitur menjadi 2, menu battle mode ditampilkan yang akan selalu diupdate setiap 1 detik. jika memilih menu 1 maka healt_status (monster kita) dan healt_npc berkurang 20. jika memilih menu 2 maka kita akan kembali ke standbye mode(fungsi standbye)
```
void *shop(void *args){
    while(1){
        if(fitur == 3)
        {
            printf("Shop Mode\n");
            printf("Shop food stock : [ %d ]\n", *stok);
            printf("Your food stock : [ %d ]\n", mystok);
            printf("Choices\n");
            printf("1. Buy\n");
            printf("2. Back\n");

            if(act_food == 1 && *stok > 0)
            {
                mystok = mystok + 1;
                *stok = *stok -1;
                act_food = 0;
            }
            if(act_food == 2)
            {
                fitur = 1;
            }
            sleep(1);
            system("clear");
        }
    }
}
```
fungsi shop akan selalu berjalan (dijalankan oleh thread). ketika fitur menjadi 3, menu battle mode ditampilkan yang akan selalu diupdate setiap 1 detik. jika memilih menu 1 nilai mystok akan bertambah 1 dan stok berkurang 1 pada kedua program(program penjual dan pembeli(game)), jika memilih fitur 2 maka akan kembali ke standbye mode
```
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
```
fungsi ini dipanggil ketika keyboard ditekan, dengan fungsi ini setiap keyboard ditekan akan langsung dibaca sebagai input(tanpa key press).
```
void *input(void *args){
    while(1){
        if(fitur == 1){
            while(!kbhit());
            act_standby = getchar() - '0';
        }
        else if(fitur == 2){
            while(!kbhit());
            act_battle = getchar() - '0';
        }
        else if(fitur == 3){
            while(!kbhit());
            act_food = getchar() - '0';
        }
    }
}
```
fungsi input akan selalu berjalan (dijalankan oleh thread). fungsi ini akan mendeteksi input berupa key press(Program bisa berjalan tanpa perlu menekan tombol enter)

**Program Penjual**
```
pthread_t thread1;
int iret1;
iret1 = pthread_create( &thread1, NULL, &input, NULL);
```
inisialisasi dan pembuatan thread untuk menjalankan fungsi input
```
int *stok;
key_t key = 1234;
    
int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stok = shmat(shmid, NULL, 0);
*stok = 10;
```
shared memory agar nilai stok selalu sama dengan program penjual
```
while(1){
    printf("Shop\n");
    printf("Food stock : [ %d ]\n", *stok);
    printf("Choices\n");
    printf("1. Restock\n");
    printf("2. Exit\n");

    if(action == 1){
        *stok = *stok + 1;
        action = 0;
    }
    else if(action == 2)
    {
        break;
    }
    sleep(1);
    system("clear");
}

```
program akan selalu mencetak menu dan update setiap 1 detik. jika memilih menu 1 maka stok akan bertambah 1, jika memilih menu 2 maka program akan berhenti
```
void *input(void *args)
{
    while(1){
        while(!kbhit());
        action = getchar() - '0';
    }
}
```
fungsi ini akan selalu berjalan(dijalankan oleh thread). fungsi ini untuk meminta input
