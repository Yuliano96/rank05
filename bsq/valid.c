#include "util.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (1);
	return (0);
}

int	map_error(char **buffer, FILE *fp)
{
	if (buffer != NULL)
	{
		free(*buffer);
		*buffer = NULL;
	}
	if (fp != NULL)
		fclose(fp);
	return (-1);
}

int	is_valid_map(int *rows, int *columns, char *empty,
		char *obstacle, char *full, char *filename)
{
	FILE		*fp;
	char		*buffer;
	size_t		capacity;
	ssize_t		len;
	int			current_columns;
	int			line_count;
	int			i;

	fp = fopen(filename, "r");
	if (fp == NULL)
		return (-1);
	buffer = NULL;
	capacity = 0;
	if (fscanf(fp, "%d %c %c %c",
			rows, empty, obstacle, full) != 4)
		return (map_error(&buffer, fp));
	if (*rows <= 0)
		return (map_error(&buffer, fp));
	if (!ft_isprint(*empty) || !ft_isprint(*obstacle)
		|| !ft_isprint(*full))
		return (map_error(&buffer, fp));
	if (*empty == *obstacle || *empty == *full
		|| *obstacle == *full)
		return (map_error(&buffer, fp));

	/*
	** fscanf deja pendiente el final de la cabecera.
	** Exigimos que después del carácter full solo exista '\n'.
	*/
	len = getline(&buffer, &capacity, fp);
	if (len != 1 || buffer[0] != '\n')
		return (map_error(&buffer, fp));
	*columns = -1;
	line_count = 0;
	while (line_count < *rows)
	{
		len = getline(&buffer, &capacity, fp);
		if (len == -1)
			return (map_error(&buffer, fp));
		if (len == 0 || buffer[len - 1] != '\n')
			return (map_error(&buffer, fp));
		current_columns = (int)len - 1;
		if (current_columns <= 0)
			return (map_error(&buffer, fp));
		if (*columns == -1)
			*columns = current_columns;
		else if (current_columns != *columns)
			return (map_error(&buffer, fp));
		i = 0;
		while (i < current_columns)
		{
			if (buffer[i] != *empty
				&& buffer[i] != *obstacle)
				return (map_error(&buffer, fp));
			i++;
		}
		line_count++;
	}

	/*
	** Si getline aún puede leer algo, había más filas
	** de las declaradas en la cabecera.
	*/
	len = getline(&buffer, &capacity, fp);
	if (len != -1)
		return (map_error(&buffer, fp));
	free(buffer);
	fclose(fp);
	return (0);
}

int	read_stream(FILE *fp, t_matriz **matrix,
		char *empty, char *obstacle, char *full)
{
	char		*buffer;
	size_t		capacity;
	ssize_t		len;
	int			rows;
	int			columns;
	int			current_columns;
	int			line;
	int			i;

	buffer = NULL;
	capacity = 0;

	/*
	** Leemos la cabecera exactamente igual que antes.
	*/
	if (fscanf(fp, "%d %c %c %c",
			&rows, empty, obstacle, full) != 4)
		return (map_error(&buffer, NULL));

	if (rows <= 0)
		return (map_error(&buffer, NULL));

	if (!ft_isprint(*empty)
		|| !ft_isprint(*obstacle)
		|| !ft_isprint(*full))
		return (map_error(&buffer, NULL));

	if (*empty == *obstacle
		|| *empty == *full
		|| *obstacle == *full)
		return (map_error(&buffer, NULL));

	/*
	** Comprobamos que después de la cabecera solo haya '\n'.
	*/
	len = getline(&buffer, &capacity, fp);
	if (len != 1 || buffer[0] != '\n')
		return (map_error(&buffer, NULL));

	/*
	** Leemos primero la primera fila para conocer las columnas.
	*/
	len = getline(&buffer, &capacity, fp);
	if (len == -1 || buffer[len - 1] != '\n')
		return (map_error(&buffer, NULL));

	columns = (int)len - 1;
	if (columns <= 0)
		return (map_error(&buffer, NULL));

	/*
	** Reservamos la estructura y todas sus filas.
	*/
	*matrix = init_matriz(rows, columns);
	if (*matrix == NULL)
		return (map_error(&buffer, NULL));

	(*matrix)->matriz = malloc(sizeof(char *) * rows);
	if ((*matrix)->matriz == NULL)
	{
		free_t_matriz(matrix);
		return (map_error(&buffer, NULL));
	}

	line = 0;
	while (line < rows)
	{
		/*
		** La primera línea ya fue leída antes del bucle.
	** Para las demás llamamos a getline().
		*/
		if (line > 0)
		{
			len = getline(&buffer, &capacity, fp);
			if (len == -1 || buffer[len - 1] != '\n')
			{
				free_matrizChar(&(*matrix)->matriz, line);
				free_t_matriz(matrix);
				return (map_error(&buffer, NULL));
			}
		}

		current_columns = (int)len - 1;
		if (current_columns != columns)
		{
			free_matrizChar(&(*matrix)->matriz, line);
			free_t_matriz(matrix);
			return (map_error(&buffer, NULL));
		}

		(*matrix)->matriz[line] = malloc(sizeof(char) * columns);
		if ((*matrix)->matriz[line] == NULL)
		{
			free_matrizChar(&(*matrix)->matriz, line);
			free_t_matriz(matrix);
			return (map_error(&buffer, NULL));
		}

		i = 0;
		while (i < columns)
		{
			if (buffer[i] != *empty
				&& buffer[i] != *obstacle)
			{
				free_matrizChar(&(*matrix)->matriz, line + 1);
				free_t_matriz(matrix);
				return (map_error(&buffer, NULL));
			}
			(*matrix)->matriz[line][i] = buffer[i];
			i++;
		}
		line++;
	}

	/*
	** Comprobamos que no existan más filas.
	*/
	len = getline(&buffer, &capacity, fp);
	if (len != -1)
	{
		free_matrizChar(&(*matrix)->matriz, rows);
		free_t_matriz(matrix);
		return (map_error(&buffer, NULL));
	}

	free(buffer);
	return (0);
}