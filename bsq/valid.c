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
		return  map_error(&buffer, fp);

	if (*rows <= 0)
		return map_error(&buffer, fp);

	if (!ft_isprint(*empty) || !ft_isprint(*obstacle)
		|| !ft_isprint(*full))
		return map_error(&buffer, fp);

	
	if (*empty == *obstacle || *empty == *full
		|| *obstacle == *full)
		return map_error(&buffer, fp);

	len = getline(&buffer, &capacity, fp);
	if (len != 1 || buffer[0] != '\n')
		return map_error(&buffer, fp);

	*columns = -1;
	line_count = 0;
	while (line_count < *rows)
	{
		len = getline(&buffer, &capacity, fp);
		if (len == -1)
			return map_error(&buffer, fp);

		/*
		** Cada línea debe acabar en '\n'.
		**
		** getline incluye el '\n' dentro del buffer.
		*/
		if (len == 0 || buffer[len - 1] != '\n')
			return map_error(&buffer, fp);

		/*
		** El salto de línea no pertenece a las columnas del mapa.
		**
		** Ejemplo:
		** buffer = "....\n"
		** len = 5
		** columnas = 4
		*/
		current_columns = (int)len - 1;

		/*
		** Debe existir al menos una casilla en cada fila.
		*/
		if (current_columns <= 0)
			return map_error(&buffer, fp);

		/*
		** La primera fila determina el ancho esperado.
		*/
		if (*columns == -1)
			*columns = current_columns;
		else if (current_columns != *columns)
			return map_error(&buffer, fp);

		/*
		** El mapa original solo puede contener empty y obstacle.
		** No incluimos el '\n' en el recorrido.
		*/
		i = 0;
		while (i < current_columns)
		{
			if (buffer[i] != *empty && buffer[i] != *obstacle)
				return map_error(&buffer, fp);
			i++;
		}
		line_count++;
	}

	/*
	** Ya hemos leído exactamente el número de filas declarado.
	** Si queda otra línea, el mapa tiene más filas de las indicadas.
	*/
	len = getline(&buffer, &capacity, fp);
	if (len != -1)
		return map_error(&buffer, fp);

	free(buffer);
	fclose(fp);
	return (0);

}

