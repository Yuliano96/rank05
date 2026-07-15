#include "util.h"


int main()
{
	int **m;
	int rows, columns;
	char empty, obstacle, full;
	t_matriz *matriz;

	if (is_valid_map(&rows, &columns, &empty, &obstacle, &full, "map.txt") == -1)
	{
		fprintf(stderr, "map error\n");
		return (1);
	}

	matriz = init_matriz(rows, columns);
	if (!matriz)
	{
		fprintf(stderr, "map error\n");
		return (1);
	}

	if (read_file("map.txt", matriz) == -1)
	{
		free_t_matriz(&matriz);
		fprintf(stderr, "map error\n");
		return (1);
	}

	print(matriz);
	fprintf(stdout, "\n");

	init_square(matriz, empty, &m);

	print_int(m, rows, columns);
	fprintf(stdout, "\n");

	large_square(m, matriz->rows, matriz->columns);

	print_int(m, rows, columns);

	t_square *c = init_squar();

	win_square(m, matriz->rows, matriz->columns, c);

	// free_matrizChar(&matriz->matriz, matriz->rows);
	// free_matrizInt(&m, matriz->rows);
	// free_t_matriz(&matriz);
	
	draw_square(matriz, c, full);
	return (0);
}