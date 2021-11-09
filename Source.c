#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

int samp(int* s, int type); //funkcja bioraca wielkosc probkowania
int domain(double* min, double* max); //funkcja sluzaca do wprowadzenia dziedziny
double param(double* parameters); //funkcja sluzaca do wprowadzenia parametrow
void generate(double* parameters, double min, double max, double* result, int s); //funkcja generujaca Y
void read(double *result, int s); //funkcja odczytujaca tablice z pliku
void files(double* tab, int s, double min, double max, int type); //funkcja zapisujaca do pliku
void gnoise(double* result, double* noise, int s); //funkcja  generujaca szum

int main()
{
	//setlocale(LC_ALL, "polish_poland");
	int i = 0, s = 0, menu, loop = 1;
	double  * result, * noise, parameters[3], min = 0, max = 0;

	result = calloc(1, sizeof(double));
	noise = calloc(1, sizeof(double));

	printf("----------------------\n       PROJEKT 1\nPRZETWARZANIE SYGNALOW\n----------------------\n\n\n");

	while (loop == 1)
	{
		printf("1. Generuj tablice\n2. Odczytaj tablice\n3. Generuj szum\n4. Zapisz\n5. Wyjdz\n");
		(void)scanf("%d", &menu);
		switch (menu)
		{
		case 1:
			system("CLS");
			printf("Wzor funkcji: y = A + B*sin(x) - cos(x)/C\n");
			samp(&s, 0);
			if (s <= 1)
			{
				system("CLS");
				printf("\nWystapil blad przy wprowadzaniu danych. Bledna ilosc probek\n");
				loop = 0;
				break;
			}

			result = calloc(s, sizeof(double));
			noise = calloc(s, sizeof(double));

			if (domain(&min, &max) == -1)
			{
				system("CLS");
				printf("Wystapil blad przy wprowadzaniu danych. Ponownie uruchom program\n\n");
				loop = 0;
				break;
			}

			if (param(parameters) == -2)
			{
				system("CLS");
				printf("Wystapil blad przy wprowadzaniu danych.\n\n");
				loop = 0;
				break;
			}


			generate(parameters, min, max, result, s);
			printf("Pomyslnie wygenerowano tablice.\n\n");
			break;

		case 2:
			system("CLS");

			//result = calloc(s, sizeof(double));
			//noise = calloc(s, sizeof(double));
			read(result, s);
			break;

		case 3:
			system("CLS");
			if (s != 0)
			{
				gnoise(result, noise, s);
				printf("Wygenerowano szum.\n\n");
			}
			else
				printf("Brak tablicy z wynikami ktora mozna zaszumic.\n\n");
			break;

		case 4:
			system("CLS");
			printf("Wybierz, ktora tablice chcesz zapisac:\n1. Zwykla\n2. Zaszumiona\n");
			int choice;
			(void)scanf("%d", &choice);
			if (choice == 1)
			{
				files(result, s, min, max, 1);
				printf("Zapisano tablice zwykla do pliku restab.csv.\n\n");
			}
			else if (choice == 2)
			{
				files(noise, s, min, max, 0);
				printf("Zapisano tablice zwykla do pliku noitab.csv.\n\n");
			}
			else
				printf("Bledne wprowadzenie");

			break;
		case 5:
			system("CLS");
			loop = 0;
			break;

		default:
			system("CLS");
			printf("Wybrana zla opcje\n\n");
			break;
		}


	}

	free(result);
	free(noise);
	printf("\nZwalniam pamiec i opuszczam program. Do widzenia!\n");
	return 0;
}

int samp(int* s, int type)
{
	int sx;
	if (type == 0)
		printf("Prosze podac ilosc probek: ");
	else
		printf("Prosze podac ilosc wynikow znajdujacych sie w pliku: ");

	(void)scanf("%d", &sx);
	if (sx <= 0)
	{
		return -1; // do zmiany na return -1 
	}
	*s = sx;
	return *s;
}

int domain(double* min, double* max)
{
	double minval = 0;
	double maxval = 0;

	printf("Prosze podac lewy przedzial : ");
	(void)scanf("%lf", &minval);
	*min = minval;

	printf("Prosze podac prawy przedzial: ");
	(void)scanf("%lf", &maxval);

	*min = minval;
	*max = maxval;

	if (*max <= *min)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

double param(double* parameters)
{
	printf("\nParametr A: ");
	if (scanf("%lf", &parameters[0]) != 1)
	{
		return -2;
	}

	printf("\nParametr B: ");
	if (scanf("%lf", &parameters[1]) != 1)
	{
		return -2;
	}

	printf("\nParametr C: ");
	if (scanf("%lf", &parameters[2]) != 1)
	{
		return -2;
	}

	if (parameters[2] == 0)
	{
		return -2;
	}

	return *parameters;
}

void generate(double* parameters, double min, double max, double* result, int s)
{
	int i = 0;
	double diff = max - min;
	double samp = diff / s;
	for (i = 0; i < s; i++)
	{
		result[i] = parameters[0] + (parameters[1] * sin(min + samp * i)) - cos(min + samp * i / parameters[2]);
		printf("f(%.2lf): %lf\n", min + (i * samp), result[i]);
	}
	system("CLS");
}

void read(double* result, int s)
{
	int i = 0;
	s = 0;
	char filename[255];

	FILE* filer;

	do {
		printf("Podaj nazwe pliku: ");
		(void)scanf("%s", &filename);
		filer = fopen(filename, "r");
		if (filer != NULL)
		{
			while (!feof(filer))
			{
				i = fgetc(filer);
				if (i == '\n')
				{
					s++;
				}
			}
		}
	} while (filer == NULL);

	//struct read_tab temptab[9999];
	char temptab[1024];

	printf("%d\n", s);

	if (filer != NULL)
	{
		for (; i < s; ++i)
		{
			fgets(temptab, 1024, filer);
		}
	}

	fclose(filer);
	for (i = 0; i < s; i++)
		printf("%c\n", temptab[i]);
}

void gnoise(double* result, double* noise, int s)
{
	srand(time(NULL));
	int i = 0;
	double random;
	{
		for (i = 0; i < s; i++)
		{
			if (rand() % 3 == 1)
			{
				random = (double)rand() / RAND_MAX * result[0] - result[0] / 2;
			}
			else
				random = 0;
				//printf("%f\n", random);
				noise[i] = result[i] + random;
				printf("\ndla %d: %lf ", i + 1, noise[i]);
		}
	}

	system("CLS");
}

void files(double* tab, int s, double min, double max, int type)
{
	int i = 0;
	double diff = max - min;
	double samp = diff / s;

	char restab[11] = "restab.csv";
	char noitab[11] = "noitab.csv";
	char name[11];
	if (type == 1)
	{
		strcpy(name, restab);
	}
	else
	{
		strcpy(name, noitab);
	}

	FILE* srtf;
	srtf = fopen(name, "w");
	if (srtf != NULL) {
		for (i = 0; i < s; i++)
			fprintf(srtf, "%f, %lf\n", min + (i * samp), tab[i]);
	}

	fclose(srtf);
	
	system("CLS");

}