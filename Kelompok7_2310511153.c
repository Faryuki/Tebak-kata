#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_WORD_LENGTH 500
#define HISTORY_FILE "riwayat.txt"
#define WORDS_FILE "kata.txt"

typedef struct {
    char *kata;
    char *kategori;
} Kata;

Kata *daftarKata = NULL;
int jumlahKata = 0;

void acakKata(char *kata) {
    int len = strlen(kata);
    for (int i = 0; i < len; i++) {
        int j = rand() % len;
        char temp = kata[i];
        kata[i] = kata[j];
        kata[j] = temp;
    }
}

int binarySearchKata(char *kata) {
    int kiri = 0;
    int kanan = jumlahKata - 1;
    while (kiri <= kanan) {
        int mid = kiri + (kanan - kiri) / 2;
        int cmp = strcmp(daftarKata[mid].kata, kata);
        if (cmp == 0) {
            return mid;
        } else if (cmp < 0) {
            kiri = mid + 1;
        } else {
            kanan = mid - 1;
        }
    }
    return -1;
}

void insertionSortKata() {
    for (int i = 1; i < jumlahKata; i++) {
        Kata kunci = daftarKata[i];
        int j = i - 1;
        while (j >= 0 && strcmp(daftarKata[j].kata, kunci.kata) > 0) {
            daftarKata[j + 1] = daftarKata[j];
            j--;
        }
        daftarKata[j + 1] = kunci;
    }
}

int hitungAnagram(char *kata1, char *kata2) {
    int n = strlen(kata1);
    int m = strlen(kata2);
    int dp[MAX_WORD_LENGTH + 1][MAX_WORD_LENGTH + 1] = {0};

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (kata1[i - 1] == kata2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
            }
        }
    }
    return dp[n][m];
}

void tulisRiwayatMain(char *namaFile) {
    FILE *file = fopen(namaFile, "a");
    if (file == NULL) {
        printf("Tidak dapat membuka file %s untuk menulis.\n", namaFile);
        return;
    }
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(file, "Game dimainkan pada %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fclose(file);
}

void tambahKata(char *kata, char *kategori) {
    if (daftarKata == NULL) {
        daftarKata = (Kata *)malloc(sizeof(Kata));
    } else {
        daftarKata = (Kata *)realloc(daftarKata, (jumlahKata + 1) * sizeof(Kata));
    }
    daftarKata[jumlahKata].kata = (char *)malloc((strlen(kata) + 1) * sizeof(char));
    daftarKata[jumlahKata].kategori = (char *)malloc((strlen(kategori) + 1) * sizeof(char));
    strcpy(daftarKata[jumlahKata].kata, kata);
    strcpy(daftarKata[jumlahKata].kategori, kategori);
    jumlahKata++;
}

void simpanKataKeFile(char *namaFile) {
    FILE *file = fopen(namaFile, "w");
    if (file == NULL) {
        printf("Tidak dapat membuka file %s untuk menulis.\n", namaFile);
        return;
    }
    for (int i = 0; i < jumlahKata; i++) {
        fprintf(file, "%s,%s\n", daftarKata[i].kata, daftarKata[i].kategori);
    }
    fclose(file);
}

void bacaKataDariFile(char *namaFile) {
    FILE *file = fopen(namaFile, "r");
    if (file == NULL) {
        return;
    }
    char baris[MAX_WORD_LENGTH * 2];
    while (fgets(baris, sizeof(baris), file) != NULL) {
        baris[strcspn(baris, "\n")] = '\0';
        char *kata = strtok(baris, ",");
        char *kategori = strtok(NULL, ",");
        if (kata && kategori) {
            tambahKata(kata, kategori);
        }
    }
    fclose(file);
    insertionSortKata();
}

void dealokasiDaftarKata() {
    if (daftarKata != NULL) {
        for (int i = 0; i < jumlahKata; i++) {
            free(daftarKata[i].kata);
            free(daftarKata[i].kategori);
        }
        free(daftarKata);
    }
}

void tampilkanDaftarKataTerurut() {
    if (jumlahKata == 0) {
        printf("Kata Kosong, silahkan tambah kata\n");
        return;
    }
    printf("Daftar Kata Terurut Sesuai Abjad:\n");
    for (int i = 0; i < jumlahKata; i++) {
        printf("%d. %s (%s)\n", i + 1, daftarKata[i].kata, daftarKata[i].kategori);
    }
}

void cariKata() {
    if (jumlahKata == 0) {
        printf("Kata Kosong, silahkan tambah kata\n");
        return;
    }

    char kata[MAX_WORD_LENGTH];
    printf("Masukkan kata yang ingin dicari: ");
    scanf(" %[^\n]s", kata);
    int index = binarySearchKata(kata);
    if (index != -1) {
        printf("Kata '%s' ditemukan pada list kata dan indeks ke-%d.\n", kata, index);
    } else {
        printf("Kata '%s' tidak ditemukan.\n", kata);
    }
}

void pilihKategori(char *kategoriDipilih) {
    printf("Pilih kategori:\n");
    printf("1. Benda\n");
    printf("2. Hewan\n");
    printf("3. Makanan\n");
    printf("4. Buah\n");
    printf("5. Negara\n");
    printf("Masukkan pilihan kategori: ");
    int pilihan;
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1:
            strcpy(kategoriDipilih, "benda");
            break;
        case 2:
            strcpy(kategoriDipilih, "hewan");
            break;
        case 3:
            strcpy(kategoriDipilih, "makanan");
            break;
        case 4:
            strcpy(kategoriDipilih, "buah");
            break;
        case 5:
            strcpy(kategoriDipilih, "negara");
            break;
        default:
            printf("Pilihan tidak valid. Menggunakan kategori default: benda\n");
            strcpy(kategoriDipilih, "benda");
    }
}

void tambahKataBaru() {
    int jumlahKataTebak;
    printf("Masukkan jumlah kata yang ingin dimasukkan: ");
    scanf("%d", &jumlahKataTebak);

    if (jumlahKataTebak > MAX_WORD_LENGTH) {
        printf("Jumlah kata melebihi batas maksimum.\n");
        return;
    }

    char kategori[MAX_WORD_LENGTH];
    pilihKategori(kategori);

    for (int i = 0; i < jumlahKataTebak; i++) {
        char kata[MAX_WORD_LENGTH];
        printf("Masukkan kata ke-%d: ", i + 1);
        scanf(" %[^\n]s", kata);
        tambahKata(kata, kategori);
    }

    insertionSortKata();
    simpanKataKeFile(WORDS_FILE);
    printf("Kata-kata telah ditambahkan dan daftar kata telah diurutkan.\n");
}

void mainTebakKata() {
    if (jumlahKata == 0) {
        printf("Kata Kosong, silahkan tambah kata\n");
        return;
    }
    char kategoriDipilih[MAX_WORD_LENGTH];
    pilihKategori(kategoriDipilih);
    int *indeksKataKategori = (int *)malloc(jumlahKata * sizeof(int));
    int jumlahKataKategori = 0;
    for (int i = 0; i < jumlahKata; i++) {
        if (strcmp(daftarKata[i].kategori, kategoriDipilih) == 0) {
            indeksKataKategori[jumlahKataKategori++] = i;
        }
    }
    if (jumlahKataKategori == 0) {
        printf("Tidak ada kata dalam kategori '%s'\n", kategoriDipilih);
        free(indeksKataKategori);
        return;
    }
    int randomIndex = indeksKataKategori[rand() % jumlahKataKategori];
    free(indeksKataKategori);
    char *kataDimasukkan = malloc((strlen(daftarKata[randomIndex].kata) + 1) * sizeof(char));
    strcpy(kataDimasukkan, daftarKata[randomIndex].kata);
    int tebakanBenar = 0;
    while (!tebakanBenar) {
        printf("\nClue tebak kata:\n");
        printf("Jumlah huruf: %d\n", (int)strlen(kataDimasukkan));
        printf("Satu huruf acak: %c\n", kataDimasukkan[rand() % strlen(kataDimasukkan)]);
        char tebakan[MAX_WORD_LENGTH];
        printf("Tebak kata: ");
        scanf(" %[^\n]s", tebakan);
        if (strcmp(kataDimasukkan, tebakan) == 0) {
            printf("Tebakan benar! Kata yang Anda tebak adalah: %s\n", tebakan);
            tebakanBenar = 1;
        } else {
            int anagramSimilarity = hitungAnagram(kataDimasukkan, tebakan);
            printf("Tebakan salah. Silakan coba lagi.\n");
            printf("Petunjuk tambahan: Kata Anda memiliki %d huruf yang cocok dengan kata yang benar.\n", anagramSimilarity);
        }
    }
    free(kataDimasukkan);
}

int main() {
    srand(time(0));
    bacaKataDariFile(WORDS_FILE);
    int mainLagi = 1;
    while (mainLagi) {
        int pilihan;
        printf("\nMenu:\n");
        printf("1. Main tebak kata\n");
        printf("2. Cari kata\n");
        printf("3. Tambah kata baru\n");
        printf("4. Tampilkan daftar kata terurut sesuai abjad\n");
        printf("5. Keluar\n");
        printf("Pilih opsi: ");
        scanf("%d", &pilihan);
        switch (pilihan) {
            case 1:
                mainTebakKata();
                break;
            case 2:
                cariKata();
                break;
            case 3:
                tambahKataBaru();
                insertionSortKata();
                simpanKataKeFile(WORDS_FILE);
                break;
            case 4:
                tampilkanDaftarKataTerurut();
                break;
            case 5:
                mainLagi = 0;
                break;
            default:
                printf("Pilihan tidak valid. Silakan coba lagi.\n");
        }
    }
    tulisRiwayatMain(HISTORY_FILE);
    dealokasiDaftarKata();
    printf("\nTerima Kasih Telah Mencoba Program Ini ^_^\n");
    return 0;
}
