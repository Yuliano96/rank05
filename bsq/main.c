#include "util.h"

int	main(int argc, char **argv)
{
	FILE		*fp;
	t_matriz	*matrix;
	t_square	square;
	int			**dp;
	char		empty;
	char		obstacle;
	char		full;
	int			i;

	if (argc == 1)
	{
		/*
		** Sin argumentos: leemos directamente desde stdin.
		*/
		matrix = NULL;
		dp = NULL;
		if (read_stream(stdin, &matrix,
				&empty, &obstacle, &full) == -1)
		{
			fprintf(stderr, "map error\n");
			return (1);
		}
		if (init_square(matrix, empty, &dp) == -1)
		{
			free_matrizChar(&matrix->matriz, matrix->rows);
			free_t_matriz(&matrix);
			fprintf(stderr, "map error\n");
			return (1);
		}
		large_square(dp, matrix->rows, matrix->columns);
		win_square(dp, matrix->rows, matrix->columns, &square);
		draw_square(matrix, &square, full);
		free_matrizInt(&dp, matrix->rows);
		free_matrizChar(&matrix->matriz, matrix->rows);
		free_t_matriz(&matrix);
		return (0);
	}

	i = 1;
	while (i < argc)
	{
		matrix = NULL;
		dp = NULL;
		fp = fopen(argv[i], "r");
		if (fp == NULL
			|| read_stream(fp, &matrix,
				&empty, &obstacle, &full) == -1)
		{
			fprintf(stderr, "map error\n");
			if (fp != NULL)
				fclose(fp);
			i++;
			if (i < argc)
				fprintf(stdout, "\n");
			continue ;
		}
		fclose(fp);

		if (init_square(matrix, empty, &dp) == -1)
		{
			free_matrizChar(&matrix->matriz, matrix->rows);
			free_t_matriz(&matrix);
			fprintf(stderr, "map error\n");
			i++;
			if (i < argc)
				fprintf(stdout, "\n");
			continue ;
		}

		large_square(dp, matrix->rows, matrix->columns);
		win_square(dp, matrix->rows, matrix->columns, &square);
		draw_square(matrix, &square, full);

		free_matrizInt(&dp, matrix->rows);
		free_matrizChar(&matrix->matriz, matrix->rows);
		free_t_matriz(&matrix);

		i++;
		if (i < argc)
			fprintf(stdout, "\n");
	}
	return (0);
}