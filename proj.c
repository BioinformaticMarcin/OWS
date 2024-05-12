#pragma warning(disable : 4996)

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4
const int n = 4; //wielkosc mnozonych macierzy //rozmiar tablic
const int PP = 2; //pierwiastek z liczbu procesow //liczba procesorów = PP*PP 
const int P = 4; //liczba procesow

float a[SIZE][SIZE], b[SIZE][SIZE], c[SIZE][SIZE];
float aa[SIZE][SIZE], bb[SIZE][SIZE];
float (*psa)[SIZE], (*psb)[SIZE], (*pra)[SIZE], (*prb)[SIZE];

int koniec;
double startwtime1, startwtime2, endwtime;

void fileSave();
void fileRead();

int main(int argc, char** argv)
{

	FILE* file_out;

	int my_rank, ncpus;
	int row, col, mod = 0;
	int data_received = -1;
	int tag = 101;

	MPI_Status  statRecv[2]; //status odbioru 
	MPI_Request reqSend[2], reqRecv[2];  // identyfikator komunikacji 

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &ncpus);

	if (my_rank == 0)
	{
		printf("obliczenia metod  Cannona dla tablicy %d x %d element w \n", n, n);

		startwtime1 = MPI_Wtime();//czas w sekundach
		fileRead(); //odczyt z pliku do tab a
	}
	else
	{
		MPI_Bcast(&koniec, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (koniec) { MPI_Finalize(); exit(0); }
	}


	if (ncpus != P) {
		if (my_rank == 0) printf("wywolano obliczenia iloczynu macierzy metoda cannona na %d procesach - uruchom mpiexec -n %d matrixmult\n", ncpus, P);
		MPI_Finalize(); 	
		exit(0);
	}

	if (my_rank == 0)
	{
		//odczyt danych wejściowych tablica A
		//rozesłanie tablicy a zgodnie z dystrybucją początkową tablicy A
		//odczyt danych wejściowych tablica B
		//rozesłanie tablicy a zgodnie z dystrybucją początkową tablicy B
		//do uzupełnienia
	}
	else
	{
		MPI_Irecv(a, n * n / PP / PP, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, reqRecv);
		//test konca komunikacji
		MPI_Irecv(b, n * n / PP / PP, MPI_FLOAT, 0, tag, MPI_COMM_WORLD, &reqRecv[1]);
		//test konca komunikacji
	}

	//przygotowanie lokalnej  tablicy wynikowej 
	row = my_rank / PP; col = my_rank % PP;
	for (int i = 0; i < n / PP; i++)
		for (int j = 0; j < n / PP; j++)
		{
			c[i][j] = 0;

		}


	if (my_rank == 0) startwtime2 = MPI_Wtime();//czas w sekundach

	//obliczenia iloczynu macierzy zgodnie z algorytmem Cannona 
	//do uzupenienia

	if (my_rank == 0)
	{
		endwtime = MPI_Wtime();
		printf("Calkowity czas przetwarzania wynosi %f sekund\n", endwtime - startwtime1);
		printf("Calkowity czas obliczen wynosi %f sekund\n", endwtime - startwtime2);

	}

	// test poprawnosci wyniku 

	if (my_rank == 0)
	{
		// obliczenia sekwencyjne mnożenia tablic CSek=A*B
		// odbiór wyników obliczeń równoległych do globalnej tablicy wynikowej Cglob
		// porównanie poprawności obliczeń (Csek, Cglob) przy uwzględniniu progu poprawności 
	}

	fileSave();

	MPI_Finalize();
	return 0;
}

void fileSave() 
{
	FILE* fileOut;

	fileOut = fopen("wynik.txt", "w+");
	if (fileOut == NULL)
	{
		printf("Blad otwarcia pliku \"wynik.txt\"\n");
		exit(0);
	}
	else {
		for (size_t i = 0; i < SIZE; i++)
		{
			for (size_t j = 0; j < SIZE; j++)
			{
				if (c[i][j])
				{
					fprintf(fileOut, "%f", c[i][j]);
				}
				else {
					printf("Blad w tabeli wynikowej na pozycji");
				}
			}
			printf("\n");
		}
		printf("Koniec zapisu pliku\n");
		fclose(fileOut);
		printf("Plik zamknięty poprawnie\n");
	}
}

void fileRead()
{
	//wczytanie danych przez proces rank=0
	FILE* file;
	//int koniec;
	file = fopen("liczby.txt", "r");
	if (file == NULL)
	{
		printf("Blad otwarcia pliku \"liczby.txt\"\n");
		koniec = 1;
		MPI_Bcast(&koniec, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Finalize();
		exit(0);
	}
	else {
		float num;
		for (size_t i = 0; i < SIZE; i++)
		{
			for (size_t j = 0; j < SIZE; j++)
			{
				if (fscanf(file, "%f", &num))
				{
					a[i][j] = num;
					//printf("Tab value [%f] <> file value [%f]\n", a[i][j], num);
					printf("[%.2f]", num);
				}
				else
				{
					printf("EOF\n");
				}
			}
			printf("\n");
		}
		printf("Koniec odczytu pliku\n");
		fclose(file);
		printf("Plik zamknięty poprawnie\n");
		koniec = 0;
		MPI_Bcast(&koniec, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
}
