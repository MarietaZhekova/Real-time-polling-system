#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct 
{
    char ime[50];
    int glasove;
} 
Kandidat;

void sortirane_rezultati(Kandidat *k, int broi);
//za podrejdane na kandidatite, kato tozi s nai-mnogo glasove e na purvo mqsto, a tozi s nai-malko na posledno
void zapisvane_statistika(Kandidat *k, int broi);
int zarejdane_statistika(Kandidat **k);
//da moje da se proveri vuv faila "glasuvali_spisak" koi e glasuval
int proverka_glasuvane(char *egn);
void dobavqne_v_spisaka(char *ime, char *egn);

int main(void) 
{
    Kandidat *kandidati = NULL;
    int broi_kandidati = zarejdane_statistika(&kandidati);
    //proverka v "glasuvali_spisak"

    if (broi_kandidati == 0) 
    {
        printf("Konfiguraciq: Kolko kandidati shte ima? ");
        if (scanf("%d", &broi_kandidati) != 1) return 1;
        //funkciqta se izpulnqva samo pri purviq put, kogato failut e prazen i vse oshte nqma glasove
        //trqbva da se zapishe chislo, ako potrebitelqt napishe neshto razlichno ot tova - greshka (return 1)


        kandidati = malloc(broi_kandidati * sizeof(Kandidat));
        if (kandidati == NULL) return 1;
        //zadelqne na pamet s malloc
        //ako komputurut nqma pamet, programata spira

        for (int i = 0; i < broi_kandidati; i++) {
            
            printf("Ime na kandidat %d: ", i + 1);
            scanf("%s", kandidati[i].ime);

            kandidati[i].glasove = 0;
            //vseki kandidat zapochva s 0 glasove
        }
    }

     char ime_glasuvhasht[50], egn_glasuvhasht[20];
     //zadelq se mqsto v pametta za imenata na potrebitelite + tqhnoto egn
    int izbor;

    while (true) {
        //while (true) cikul, za da moje programata da ne se restartira sled vseki glasuvasht, a da e bezkraina
        printf("\n-Terminal za glasuvane-\n");
        printf("Vavedete vasheto Ime (ili 'izhod'): ");
        scanf("%s", ime_glasuvhasht);
        
        //pri vuvejdane na "izhod" cikulut spira, inache e vechen
         if (strcmp(ime_glasuvhasht, "izhod") == 0) break;

       // vuvejdane na EGN
        do {
            printf("Vavedete vasheto EGN: ");
            scanf("%s", egn_glasuvhasht);

            // EGN-to trqbva zaduljitelno da e s 10 cifri, pri vuvejdane na po-malko ili poveche, se izprashta suobshtenie za povtoren opit
            if (strlen(egn_glasuvhasht) != 10) {
                printf("Greshka! EGN-to trqbva da sudurja tochno 10 cifri. Opitaite otnovo.\n");
            }
        } while (strlen(egn_glasuvhasht) != 10);

         //proverqva se dali ima chovek sus sushtoto EGN - ako da, to toi veche e glasuval
        if (proverka_glasuvane(egn_glasuvhasht)) {
            printf("Greshka! Liceto s EGN %s veche e glasuvalo!\n", egn_glasuvhasht);
            continue;
        }

        printf("\nIzberete za kogo da glasuvate:\n");
        for (int i = 0; i < broi_kandidati; i++) {
            printf("%d. %s\n", i + 1, kandidati[i].ime);
        }

        
        do {
            printf("Vashiqt izbor (1-%d): ", broi_kandidati);

            if (scanf(" %d", &izbor) != 1) 
                {
                    printf("Greshka! Molq vuvedete chislo.\n");

                    while (getchar() != '\n');
                    izbor = -1;
                    continue;
                }

            if (izbor < 1 || izbor > broi_kandidati) {
                //ako se vuvede greshno chislo(izvun vuzmojnite opcii) se pokazva greshka na potrebitelq
                printf("Greshka! Nevaliden nomer. Opitaite otnovo.\n");
            }
        } while (izbor < 1 || izbor > broi_kandidati);

        // sled kato izborut e validen, se izvurshvat
        kandidati[izbor - 1].glasove++;
        zapisvane_statistika(kandidati, broi_kandidati);
        //glasovete se dobavqt v rezultati
        dobavqne_v_spisaka(ime_glasuvhasht, egn_glasuvhasht);
        
        printf("Blagodarim, %s! Glasut vi e zapisan uspeshno.\n", ime_glasuvhasht);
    }

    sortirane_rezultati(kandidati, broi_kandidati);

    printf("\n-Finalno Klasirane-\n");
    for (int i = 0; i < broi_kandidati; i++) 
    {
        printf("%s: %d glasove\n", kandidati[i].ime, kandidati[i].glasove);
    }

    free(kandidati);
    return 0;
}

//sortirane po izbor (selection sort)
void sortirane_rezultati(Kandidat *k, int n) {
    for (int i = 0; i < n - 1; i++) 
    {

        int max_indeks = i;

        for (int j = i + 1; j < n; j++) {
            if (k[j].glasove > k[max_indeks].glasove) {
                max_indeks = j;
            }
        }
        Kandidat vremenen = k[max_indeks];
        k[max_indeks] = k[i];
        k[i] = vremenen;
    }
}


void zapisvane_statistika(Kandidat *k, int n) {

    //celta e da se prezapishat novite rezultati
    FILE *f = fopen("rezultati.txt", "w");

    if (f == NULL) return;
    fprintf(f, "%d\n", n);

    for (int i = 0; i < n; i++) {
        fprintf(f, "%s %d\n", k[i].ime, k[i].glasove);
    }
    fclose(f);
}



int zarejdane_statistika(Kandidat **k) {
    //da se vuzstanovqt dannite ot faila i da se zadeli pamet spored broq kandiidati
    FILE *f = fopen("rezultati.txt", "r");

    if (f == NULL) return 0;

    int n;

    if (fscanf(f, "%d", &n) != 1) {
        fclose(f);
        return 0;
    }

    *k = malloc(n * sizeof(Kandidat));
    
    for (int i = 0; i < n; i++) {
        fscanf(f, "%s %d", (*k)[i].ime, &((*k)[i].glasove));
    }
    fclose(f);
    return n;
}

//da se proveri dali chovek e glasuval veche, za da ne moje da glasuva otnovo
int proverka_glasuvane(char *egn) {
    FILE *f = fopen("glasuvali_spisak.txt", "r");

    if (f == NULL) return 0;

    char tekushto_egn[20], ime[50];

    while (fscanf(f, "%s %s", ime, tekushto_egn) != EOF) 
    {

        if (strcmp(egn, tekushto_egn) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void dobavqne_v_spisaka(char *ime, char *egn) {
    FILE *f = fopen("glasuvali_spisak.txt", "a");
    if (f != NULL) {
        fprintf(f, "%s %s\n", ime, egn);
        fclose(f);
    }
}