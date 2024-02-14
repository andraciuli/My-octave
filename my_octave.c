#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//functie pt evitarea overflow-ului
int mod(int count)
{
	if (count < 0)
		return count % 10007 + 10007;
	else
		return count % 10007;
}

//functie pt a aloca memoriei unei matrice

int **alloc_matrix(int r, int c)
{
	int **mat = (int **)malloc(r * sizeof(int *));
	if (!mat) {
		fprintf(stderr, "malloc() for matrix failed\n");
		return NULL;
	}
	for (int i = 0; i < r; i++) {
		mat[i] = malloc(c * sizeof(int));
		if (!mat[i]) {
			fprintf(stderr, "malloc() for matrix failed\n");
			return NULL;
			free(mat[i]);
		}
	}
	return mat;
}

//functie pt alocarea memoriei a vect de matrici, nr de linii,
//nr de coloane

void alloc(int ****array_mat, int **r, int **c)
{
	*array_mat = malloc(sizeof(int **));
	*r = malloc(sizeof(int));
	*c = malloc(sizeof(int));
}

//functie pt realocarea memoriei a vect de matrici, nr de linii,
//nr de coloane
void realloc_mat(int ****array_mat, int **r, int **c, int count)
{
	*array_mat = realloc(*array_mat, (count + 1) * sizeof(int **));
	*r = realloc(*r, (count + 1) * sizeof(int));
	*c = realloc(*c, (count + 1) * sizeof(int));
}

//functie pt citirea matricei

void read_matrix(int r, int c, int **mat)
{
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j)
			scanf("%d", &mat[i][j]);
	}
}

//functie pt afisarea matricei

void print_matrix(int r, int c, int **mat)
{
	for (int i = 0; i < r; ++i) {
		for (int j = 0; j < c; ++j)
			printf("%d ", mat[i][j]);
		printf("\n");
	}
}

//functie pt eliberarea memoriei unei matrice

void free_matrix(int r, int **mat)
{
	for (int i = 0; i < r; i++)
		free(mat[i]);
	free(mat);
}

//functie de redimensionare a matricei cu un index citit

void C_function(int ****array_mat, int **index_r,
				int **index_c, int count)
{
	int index;
	scanf("%d", &index);
	if (index > count - 1 || index < 0) {
		printf("No matrix with the given index\n");
//se citesc dimensiunile noi chiar daca indexul nu exista
		int nr_r, nr_c;
		scanf("%d", &nr_r);
		for (int i = 0 ; i < nr_r ; ++i)
			scanf("%d", &nr_c);
		scanf("%d", &nr_r);
		for (int i = 0 ; i < nr_r ; ++i)
			scanf("%d", &nr_c);
	} else {
		int nr_r, nr_c;
		scanf("%d", &nr_r);
//se aloca dinamic dimensiunile
		int *new_r = malloc(nr_r * sizeof(int));
		if (!new_r) {
			fprintf(stderr, "malloc() failed\n");
			exit(-1);
		}
		for (int i = 0; i < nr_r; ++i)
			scanf("%d", &new_r[i]);
		scanf("%d", &nr_c);
		int *new_c = malloc(nr_c * sizeof(int));
		if (!new_c) {
			fprintf(stderr, "malloc() failed\n");
			exit(-1);
		}
		for (int i = 0; i < nr_c; ++i)
			scanf("%d", &new_c[i]);
		int **mat = malloc(nr_r * sizeof(int *));
		if (!mat) {
			fprintf(stderr, "malloc() failed\n");
			exit(-1);
		} else {
			for (int i = 0; i < nr_r; ++i) {
				mat[i] = malloc(nr_c * sizeof(int));
				if (!mat[i])
					free(mat[i]);
			}
//se construieste matricea
				for (int i = 0; i < nr_r; ++i)
					for (int j = 0; j < nr_c; ++j)
						mat[i][j] = (*array_mat)[index][new_r[i]][new_c[j]];
				free_matrix((*index_r)[index], (*array_mat)[index]);
//valorile din vector se restabilesc dupa noile dimensiuni ale matricii
				(*array_mat)[index] = mat;
				(*index_r)[index] = nr_r;
				(*index_c)[index] = nr_c;
//memoria alocata pt linii si coloane este eliberata
				free(new_r);
				free(new_c);
			}
		}
}

void M_function(int ****array_mat, int **index_r,
				int **index_c, int *count)
{
	int ind1, ind2;
	scanf("%d%d", &ind1, &ind2);
	if (ind1 > (*count - 1) || ind1 < 0 || ind2 > (*count - 1) || ind2 < 0) {
		printf("No matrix with the given index\n");
	} else {
		//verificam daca se poate face inmultirea
		//(daca nr de linii de la a doua matrice este egal cu nr de coloane
		//de la prima matrice)
		if ((*index_r)[ind2] == (*index_c)[ind1]) {
			//alocam memorie pt matricea rezultata
			//si crestem nr de matrici din vector
			//(*count) = (*count) + 1;
			*array_mat = (int ***)realloc(*array_mat,
										  (*count + 1) * sizeof(int **));
			if ((*array_mat) == NULL) {
				fprintf(stderr, "Realloc failed\n");
				return;
			}
			*index_r = (int *)realloc(*index_r, (*count + 1) * sizeof(int));
			if ((*index_r) == NULL) {
				fprintf(stderr, "Realloc failed\n");
				return;
			}
			*index_c = (int *)realloc(*index_c, (*count + 1) * sizeof(int));
			if ((*index_c) == NULL) {
				fprintf(stderr, "Realloc failed\n");
				return;
			}
			//facem algorimul de inmultire a matricilor
			int **mat_1 = (*array_mat)[ind1];
			int **mat_2 = (*array_mat)[ind2];
			int **multi = alloc_matrix((*index_r)[ind1], (*index_c)[ind2]);
			for (int i = 0; i < (*index_r)[ind1]; ++i)
				for (int j = 0; j < (*index_c)[ind2]; ++j) {
					multi[i][j] = 0;
					for (int k = 0; k < (*index_r)[ind2]; ++k) {
						multi[i][j] = multi[i][j] + (mat_1[i][k] * mat_2[k][j]);
						multi[i][j] = mod(multi[i][j]);
					}
					multi[i][j] = mod(multi[i][j]);
					(*array_mat)[*count] = multi;
					(*index_r)[*count] = (*index_r)[ind1];
					(*index_c)[*count] = (*index_c)[ind2];
				}
			*count = *count + 1;
		} else {
			printf("Cannot perform matrix multiplication\n");
		}
	}
}

void T_function(int **index_r, int **index_c, int ****array_mat, int count)
{
	int **tr, index;
	scanf("%d", &index);
//verificam daca indexul citit exista
	if (index > count - 1 || index < 0) {
		printf("No matrix with the given index\n");
	} else {
//alocam memorie pt transpusa
//folosim algoritmul pt a inversa liniile si coloanele
		tr = alloc_matrix((*index_c)[index], (*index_r)[index]);
		for (int i = 0; i < (*index_r)[index]; i++)
			for (int j = 0; j < (*index_c)[index]; j++)
				tr[j][i] = (*array_mat)[index][i][j];
		free_matrix((*index_r)[index], (*array_mat)[index]);
		(*array_mat)[index] = tr;
//inversam nr de linii si coloane
		int aux = (*index_r)[index];
		(*index_r)[index] = (*index_c)[index];
		(*index_c)[index] = aux;
	}
}

void O_function(int **index_r, int **index_c, int ****array_mat, int count)
{
//alocam memorie vectorului care contine suma elem fiecarei matrice
	int *srt = malloc((count) * sizeof(int));
//construim vectorul
	for (int i = 0; i < count; ++i) {
		srt[i] = 0;
		for (int j = 0; j < (*index_r)[i]; ++j)
			for (int k = 0; k < (*index_c)[i]; ++k)
				srt[i] = mod(srt[i] + (*array_mat)[i][j][k]);
	}
//ordonam vectorul
	for (int i = 0; i < count; ++i)
		for (int j = i + 1; j < count; ++j) {
			if (srt[i] > srt[j]) {
				int aux = srt[i];
				srt[i] = srt[j];
				srt[j] = aux;
				int **mat = (*array_mat)[i];
				(*array_mat)[i] = (*array_mat)[j];
				(*array_mat)[j] = mat;
				aux = (*index_r)[i];
				(*index_r)[i] = (*index_r)[j];
				(*index_r)[j] = aux;
				aux = (*index_c)[i];
				(*index_c)[i] = (*index_c)[j];
				(*index_c)[j] = aux;
			}
		}
	free(srt);
}

//functie pt matricea unitate

void unitate(int ***rez, int dim)
{
	for (int i = 0 ; i < dim ; i++)
		for (int j = 0 ; j < dim ; j++) {
			if (i == j)
				(*rez)[i][j] = 1;
			else
				(*rez)[i][j] = 0;
		}
}

//functie pt inmultirea a doua matrici patratice

void multiply(int ***multi, int **mat1, int **mat2, int dim)
{
	for (int i = 0; i < dim ; ++i)
		for (int j = 0; j < dim ; ++j) {
			(*multi)[i][j] = 0;
			for (int k = 0; k < dim ; ++k) {
				(*multi)[i][j] = (*multi)[i][j] + (mat1[i][k] * mat2[k][j]);
				(*multi)[i][j] = mod((*multi)[i][j]);
			}
			(*multi)[i][j] = mod((*multi)[i][j]);
		}
}

void R_function(int **index_r, int **index_c, int ****array_mat, int count)
{
	int **rez, index, p;
	scanf("%d %d", &index, &p);
	if (index > count - 1 || index < 0) {
		printf("No matrix with the given index\n");
	} else {
//verificam daca se poate face ridicarea la putere
		if (p < 0) {
			printf("Power should be positive\n");
			return;
		}
//verificam daca matricea este patratica
		if ((*index_r)[index] != (*index_c)[index]) {
			printf("Cannot perform matrix multiplication\n");
			return;
		}
//alocam memorie pt matricea care va rezulta din ridicarea la putere
		rez = alloc_matrix((*index_r)[index], (*index_c)[index]);
		unitate(&rez, (*index_r)[index]);
		int **mat = (*array_mat)[index];
		int **multi = alloc_matrix((*index_r)[index], (*index_c)[index]);
//aplicam algoritmul de ridicare la putere in timp logaritmic
		while (p) {
			if (p % 2 == 1) {
				multiply(&multi, rez, mat, (*index_r)[index]);
				for (int i = 0; i < (*index_r)[index]; i++)
					for (int j = 0; j < (*index_c)[index]; j++)
						rez[i][j] = multi[i][j];
			}
//ridicam la patrat matricea dupa fiecare inmultire
			multiply(&multi, mat, mat, (*index_r)[index]);
			for (int i = 0; i < (*index_r)[index]; i++)
				for (int j = 0; j < (*index_c)[index]; j++)
					mat[i][j] = multi[i][j];
			p = p / 2;
		}
		free_matrix((*index_r)[index], multi);
		free_matrix((*index_r)[index], (*array_mat)[index]);
		(*array_mat)[index] = rez;
	}
}

void F_function(int ****array_mat, int **index_r,
				int **index_c, int index, int *count)
{
	scanf("%d", &index);
	if (index > (*count - 1) || index < 0) {
		printf("No matrix with the given index\n");
	} else {
//eliberam memoria matricei
		free_matrix((*index_r)[index], (*array_mat)[index]);
//restul matricilor se muta cu o pozitie spre stanga
		for (int i = index; i < (*count - 1); i++) {
			(*array_mat)[i] = (*array_mat)[i + 1];
			(*index_r)[i] = (*index_r)[i + 1];
			(*index_c)[i] = (*index_c)[i + 1];
		}
//scade nr de matrici
		(*count)--;
	}
}

int main(void)
{
	int count = 0;
	int index;
	int ***array_mat, *index_r, *index_c;
	char letter;
	alloc(&array_mat, &index_r, &index_c);
	scanf("%c", &letter);
//in functie de litera citita se face operatia corespunzatoare
//pana cand se citeste Q
//apoi se elibereaza intreaga memorie
	do {
		if (!strchr("LDPCMOTFR0123456789\n", letter)) {
			printf("Unrecognized command\n");
		} else {
			if (letter == 'L') {
				int r, c;
				scanf("%d%d", &r, &c);
//se aloca memorie matricei si se citeste
				index_r[count] = r;
				index_c[count] = c;
				realloc_mat(&array_mat, &index_r, &index_c, count + 1);
				array_mat[count] = alloc_matrix(r, c);
				read_matrix(r, c, array_mat[count]);
				count++;
			} else {
				if (letter == 'D') {
//se afiseaza dimensiunea matricei cu indexul citit
					int index;
					scanf("%d", &index);
					if (index > count - 1 || index < 0)
						printf("No matrix with the given index\n");
					else
						printf("%d %d\n", index_r[index], index_c[index]);
				} else if (letter == 'P') {
//se afiseaza matricea
					scanf("%d", &index);
					if (index > count - 1 || index < 0)
						printf("No matrix with the given index\n");
					else
						print_matrix(index_r[index], index_c[index]
									, array_mat[index]);
				} else if (letter == 'C') {
					C_function(&array_mat, &index_r, &index_c, count);
				} else if (letter == 'M') {
					M_function(&array_mat, &index_r, &index_c, &count);
				} else if (letter == 'T') {
					T_function(&index_r, &index_c, &array_mat, count);
				} else if (letter == 'O') {
					O_function(&index_r, &index_c, &array_mat, count);
				} else if (letter == 'R') {
					R_function(&index_r, &index_c, &array_mat, count);
				} else if (letter == 'F') {
					F_function(&array_mat, &index_r, &index_c,
							   index, &count);
				}
			}
		}
		scanf("%c", &letter);
	} while (letter != 'Q');
	for (int i = 0; i < count; i++)
		free_matrix(index_r[i], array_mat[i]);
	free(index_r);
	free(index_c);
	free(array_mat);
	return 0;
}

