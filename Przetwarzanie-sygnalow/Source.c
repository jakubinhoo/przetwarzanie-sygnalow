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
double read(double* result, int s); //funkcja odczytujaca tablice z pliku
int files(double* tab, int s, double min, double max, int type); //funkcja zapisujaca do pliku
void gnoise(double* result, double* noise, int s); //funkcja  generujaca szum
int countlines(char* filename); //funkcja liczaca liczbe linijek w pliku
void noifil(double* noise, double* filter, int s); // funkcja filtrujaca szum

int main()
{
	//setlocale(LC_ALL, "polish_poland");
	int i = 0, s = 0, loop = 1;
	char menu;
	double* result, * noise, * filter, parameters[3], min = 0, max = 0;

	result = calloc(1, sizeof(double));
	noise = calloc(1, sizeof(double));
	filter = calloc(1, sizeof(double));

	printf("----------------------\n       PROJEKT 1\nPRZETWARZANIE SYGNALOW\n----------------------\n\n\n");

	while (loop == 1)
	{
		printf("1. Generuj tablice\n2. Odczytaj tablice\n3. Generuj szum\n4. Zapisz\n5. Filtruj\n6. Wyjdz\n");
		(void)scanf("%c", &menu);
		switch (menu)
		{
		case '1':
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

		case '2':
			system("CLS");
			samp(&s, 1);
			result = calloc(s, sizeof(double));
			noise = calloc(s, sizeof(double));
			if(read(noise, s) == -1)
			{
				system("CLS");
				printf("BLAD Z OTWARCIEM PLIKU.");
				loop = 0;
				break;
			}
			break;

		case '3':
			system("CLS");
			if (s != 0)
			{
				gnoise(result, noise, s);
				printf("Wygenerowano szum.\n\n");
			}
			else
				printf("Brak tablicy z wynikami ktora mozna zaszumic.\n\n");
			break;

		case '4':
			system("CLS");
			printf("Wybierz, ktora tablice chcesz zapisac:\n1. Zwykla\n2. Zaszumiona\n3. Odfiltrowana\n");
			int choice;
			(void)scanf("%d", &choice);
			if (choice == 1)
			{
				if (files(result, s, min, max, 1) == -1)
				{
					system("CLS");
					printf("BLAD Z OTWARCIEM PLIKU.");
					loop = 0;
					break;
				}
				printf("Zapisano tablice zwykla do pliku restab.csv.\n\n");
			}
			else if (choice == 2)
			{
				if (files(noise, s, min, max, 0) == -1)
				{
					system("CLS");
					printf("BLAD Z OTWARCIEM PLIKU.");
					loop = 0; 
					break;
				}
				printf("Zapisano tablice zaszumiona do pliku noitab.csv.\n\n");
			}
			else if (choice == 3)
			{
				if (files(filter, s, min, max, 2) == -1)
				{
					system("CLS");
					printf("BLAD Z OTWARCIEM PLIKU.");
					loop = 0;
					break;
				}
				printf("Zapisano tablice odfiltrowana do pliku filtab.csv.\n\n");
			}
			break;
		case '5':
			system("CLS");
			if (s != 0)
			{
				filter = calloc(s, sizeof(double));
				noifil(noise, filter, s);
			}
			else
				printf("Brak tablicy z szumem ktora mozna odfiltrowac.\n\n");
			break;
		case '6':
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
	free(filter);
	printf("\nZwalniam pamiec i opuszczam program. Do widzenia!\n");
	return 0;
}

int samp(int* s, int type)
{
	int sx;
	if (type == 0)
	{
		printf("Prosze podac ilosc probek: ");

		(void)scanf("%d", &sx);
		if (sx <= 0)
		{
			return -1;
		}
		*s = sx;
		return *s;
	}
	else
	{
		if (countlines("noitba.csv") == -1)
			return -1;
		else
			sx = countlines("noitab.csv");
		*s = sx;
		return *s;
	}
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

double read(double* noise, int s)
{
	FILE* filer;

	filer = fopen("noitab.csv", "r");


	if (filer == NULL)
		return -1;

	char line[200];
	double* temptab;
	temptab = calloc(s, sizeof(double));

	int i = 0;
	while (fgets(line, sizeof(line), filer)) {
		const char* token;
		token = strpbrk(line, " ");
		printf("%s", token);
		temptab[i] = atof(token);
		i++;
	}

	for (i = 0; i < s; i++)
		noise[i] = temptab[i];
	free(temptab);
	printf("\n Odczytano zaszumiona tablice z pliku noitab.csv. Mozesz ja teraz odszumic\n\n");

	return *noise;

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

int files(double* tab, int s, double min, double max, int type)
{
	int i = 0;
	double diff = max - min;
	double samp = diff / s;

	char restab[11] = "restab.csv";
	char noitab[11] = "noitab.csv";
	char filtab[11] = "filtab.csv";
	char name[11];
	if (type == 1)
	{
		strcpy(name, restab);
	}
	else if (type == 0)
	{
		strcpy(name, noitab);
	}
	else
	{
		strcpy(name, filtab);
	}

	FILE* srtf;
	srtf = fopen(name, "w");

	if (type == 1)
	{
		if (srtf != NULL)
		{
			for (i = 0; i < s; i++)
				fprintf(srtf, "%f, %lf\n", min + (i * samp), tab[i]);
		}
		else
			return -1;
	}
	else
	{
		if (srtf != NULL)
		{
			for (i = 0; i < s; i++)
				fprintf(srtf, "%d, %lf\n", i + 1, tab[i]);
		}
		else
			return -1;
	}

	fclose(srtf);

	system("CLS");

}

int countlines()
{
	FILE* clines = fopen("noitab.csv", "r");

	int ch = 0;
	int lines = 0;

	if (clines == NULL)
		return -1;

	while (!feof(clines))
	{
		ch = fgetc(clines);
		if (ch == '\n')
		{
			lines++;
		}
	}
	return lines;

	fclose(clines);
}

double bubble(double med[])
{
	int i, j;
	double swap;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4 - i; j++)
		{
			if (med[j] > med[j + 1])
			{
				swap = med[j];
				med[j] = med[j + 1];
				med[j + 1] = swap;
			}
		}
	}
	return med[2];
}

void noifil(double* noise, double* filter, int s)
{
	double med[] = { 0, 0, 0, 0, 0 };
	int i, j, k, swap = 0;

	filter[0] = noise[0];
	filter[1] = noise[1];
	filter[s - 1] = noise[s - 1];
	filter[s - 2] = noise[s - 2];

	for (i = 2; i < s - 2; i++)
	{
		k = 0;
		for (j = i; j < i + 4; j++)
		{
			med[k] = noise[j];
			k++;
		}
		filter[i] = bubble(med);
	}

	for (i = 0; i < s; i++)
	{
		printf("%lf\n", filter[i]);
	}

	system("CLS");

	printf("Odfiltrowano szum.\n\n");
}