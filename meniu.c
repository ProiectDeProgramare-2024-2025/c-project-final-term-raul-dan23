#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define Reset "\033[0m"
#define Red "\033[0;31m"
#define Purple "\033[35m"
#define Blue "\033[0;34m"
#define White "\033[0;37m"
#define Yellow "\033[1;33m"

typedef struct {
    char nume[100];
    char categorie[100];
    char memorie[50];
    char tip[50];
    char pret[50];
    char rating[10];
} Aplicatie;

void afisare_cu_efect(const char *caracter, int delay) {
    while (*caracter) {
        printf("%c", *caracter);
        fflush(stdout);
        usleep(delay);
        caracter++;
    }
}

void curata_buffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void afisare_meniu() {
    system("clear");
    afisare_cu_efect(Blue "======= MENIU =======\n" Reset, 20000);
    afisare_cu_efect(Purple "\n1. Sign up / Sign in\n", 20000);
    afisare_cu_efect(Purple "2. Instalare aplicatie\n", 20000);
    afisare_cu_efect(Purple "3. Vizualizare aplicatii\n", 20000);
    afisare_cu_efect(Purple "4. Stergere aplicatii\n", 20000);
    afisare_cu_efect(Purple "5. Statistici aplicatii\n", 20000);
    afisare_cu_efect(Purple "6. Help\n", 20000);
    afisare_cu_efect(Red "7. Iesire\n", 20000);
}

void optiune1_conectare() {
    char email[100], parola[100], parola_confirma[100];
    FILE *f = fopen("utilizator.txt", "r");
    if (f == NULL) {
        do {
            printf("Introduceti un email valid (ex: user@email.com): ");
            scanf("%99s", email);
            curata_buffer();
        } while (strchr(email, '@') == NULL);

        do {
            printf("Creati o parola (minim 6 caractere): ");
            scanf("%99s", parola);
            curata_buffer();
        } while (strlen(parola) < 6);

        do {
            printf("Confirmati parola: ");
            scanf("%99s", parola_confirma);
            curata_buffer();
        } while (strcmp(parola, parola_confirma) != 0);

        FILE *fw = fopen("utilizator.txt", "w");
        if (fw) {
            fprintf(fw, "%s\n%s\n", email, parola);
            fclose(fw);
        }
        afisare_cu_efect("Cont creat cu succes!\n", 20000);
    } else {
        char email_salvat[100], parola_salvata[100];
        fgets(email_salvat, sizeof(email_salvat), f);
        fgets(parola_salvata, sizeof(parola_salvata), f);
        fclose(f);
        email_salvat[strcspn(email_salvat, "\n")] = '\0';
        parola_salvata[strcspn(parola_salvata, "\n")] = '\0';

        bool autentificat = false;
        for (int i = 0; i < 3 && !autentificat; i++) {
            printf("Email: ");
            scanf("%99s", email);
            curata_buffer();
            printf("Parola: ");
            scanf("%99s", parola);
            curata_buffer();

            if (strcmp(email, email_salvat) == 0 && strcmp(parola, parola_salvata) == 0) {
                afisare_cu_efect("Autentificare reusita!\n", 20000);
                autentificat = true;
            } else {
                afisare_cu_efect("Email sau parola incorecta.\n", 20000);
            }
        }
        if (!autentificat) {
            afisare_cu_efect("Prea multe incercari. Reveniti mai tarziu.\n", 20000);
        }
    }
    usleep(1000000);
}

void optiune2_instalare_aplicatie() {
    Aplicatie a;
    char raspuns[4];

    printf("Introduceti numele aplicatiei: ");
    scanf("%99s", a.nume);
    printf("Introduceti categoria: ");
    scanf("%99s", a.categorie);
    printf("Memorie utilizata (ex: 150MB): ");
    scanf("%49s", a.memorie);
    printf("Tipul aplicatiei (ex: joc): ");
    scanf("%49s", a.tip);
    printf("Pret (ex: gratuit, 5.99): ");
    scanf("%49s", a.pret);
    printf("Rating (1-5): ");
    scanf("%9s", a.rating);

    printf("Doriti sa instalati aplicatia? (Da/Nu): ");
    scanf("%3s", raspuns);

    if (strcmp(raspuns, "Da") == 0) {
        FILE *f_app = fopen("aplicatie.txt", "a");
        FILE *f_stat = fopen("statistici.txt", "a");
        if (f_app && f_stat) {
            fprintf(f_app, "%s\n", a.nume);
            fprintf(f_stat, "%s,%s,%s,%s,%s\n", a.memorie, a.tip, a.pret, a.rating, a.categorie);
            fclose(f_app);
            fclose(f_stat);
        }
        afisare_cu_efect("Aplicatia a fost instalata cu succes.\n", 20000);
    } else {
        afisare_cu_efect("Instalare anulata.\n", 20000);
    }
    usleep(1000000);
}

void optiune3_vizualizare() {
    FILE *f = fopen("aplicatie.txt", "r");
    if (!f) {
        printf("Nu exista aplicatii instalate.\n");
        return;
    }
    char linie[100];
    afisare_cu_efect("Aplicatii instalate:\n", 20000);
    while (fgets(linie, sizeof(linie), f)) {
        linie[strcspn(linie, "\n")] = '\0';
        printf("- " Blue "%s" Reset "\n", linie);
    }
    fclose(f);
    usleep(1000000);
}

void optiune4_stergere_aplicatie() {
    FILE *f = fopen("aplicatie.txt", "w");
    if (f) fclose(f);
    f = fopen("statistici.txt", "w");
    if (f) fclose(f);
    afisare_cu_efect("Aplicatiile au fost sterse.\n", 20000);
    usleep(1000000);
}

void optiune5_statistici() {
    FILE *f_app = fopen("aplicatie.txt", "r");
    FILE *f_stat = fopen("statistici.txt", "r");
    if (!f_app || !f_stat) {
        printf("Nu exista statistici disponibile.\n");
        return;
    }
    char app[100], linie[256];
    int index = 1;
    while (fgets(app, sizeof(app), f_app) && fgets(linie, sizeof(linie), f_stat)) {
        app[strcspn(app, "\n")] = '\0';
        linie[strcspn(linie, "\n")] = '\0';

        char *memorie = strtok(linie, ",");
        char *tip = strtok(NULL, ",");
        char *pret = strtok(NULL, ",");
        char *rating = strtok(NULL, ",");
        char *categorie = strtok(NULL, ",");

        printf("Aplicatia %d: " Yellow "%s" Reset "\n", index++, app);
        printf("  - Memorie: %s\n", memorie);
        printf("  - Tip: %s\n", tip);
        printf("  - Pret: " Red "%s" Reset "\n", pret);
        printf("  - Rating: " Blue "%s" Reset "\n", rating);
        printf("  - Categorie: " Purple "%s" Reset "\n\n", categorie);
    }
    fclose(f_app);
    fclose(f_stat);
    usleep(1000000);
}

void optiune6_info() {
    afisare_cu_efect("Cum functioneaza meniul:\n", 20000);
    afisare_cu_efect("1. Introduceti numarul optiunii si apasati Enter.\n", 20000);
    afisare_cu_efect("2. Fiecare optiune executa o actiune specifica.\n", 20000);
    afisare_cu_efect("3. Apasati Enter dupa fiecare actiune pentru a reveni la meniu.\n", 20000);
    usleep(500000);
}

void optiune_submeniu() {
    printf("Apasa Enter pentru a reveni la meniul principal...\n");
    getchar();
}

int main() {
    int optiune;
    do {
        afisare_meniu();
        printf("Alegeti o optiune: ");
        scanf("%d", &optiune);
        curata_buffer();
        system("clear");
        switch (optiune) {
            case 1: optiune1_conectare(); break;
            case 2: optiune2_instalare_aplicatie(); break;
            case 3: optiune3_vizualizare(); break;
            case 4: optiune4_stergere_aplicatie(); break;
            case 5: optiune5_statistici(); break;
            case 6: optiune6_info(); break;
            case 7: afisare_cu_efect("Iesire din program...\n", 20000); break;
            default: printf("Optiune invalida. Incercati din nou.\n"); break;
        }
        if (optiune != 7) optiune_submeniu();
    } while (optiune != 7);
    return 0;
}