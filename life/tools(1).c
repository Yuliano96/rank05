#include "tools(1).h"

int	read_commands(char *buffer, size_t size)
{
	int	bytes_read;

	/*
	** Antes añadías '\0' en buffer[bytes_read]. Si read() llenaba todo el
	** buffer, escribías una posición fuera. Aquí no necesitamos terminarlo
	** como string: draw_table() usa exactamente el número de bytes leídos.
	*/
	bytes_read = read(0, buffer, size);
	return (bytes_read);
}

t_point	*init_pen(int width, int height)
{
	t_point	*pen;

	pen = (t_point *)malloc(sizeof(t_point));
	if (pen == NULL)
		return (NULL);
	pen->flag = 0;
	pen->x = 0;
	pen->y = 0;
	pen->width = width;
	pen->height = height;
	return (pen);
}

void	delete_table(char ***matrix, int rows)
{
	int	i;

	if (matrix == NULL || *matrix == NULL)
		return ;
	i = 0;
	while (i < rows)
	{
		free((*matrix)[i]);
		i++;
	}
	free(*matrix);
	*matrix = NULL;
}

char	**create_table(int rows, int columns)
{
	char	**matrix;
	int		i;

	if (rows <= 0 || columns <= 0)
		return (NULL);
	matrix = (char **)malloc(sizeof(char *) * rows);
	if (matrix == NULL)
		return (NULL);
	i = 0;
	while (i < rows)
	{
		matrix[i] = (char *)malloc(sizeof(char) * columns);
		/* Antes comprobabas !matrix; hay que comprobar la fila recién creada. */
		if (matrix[i] == NULL)
		{
			delete_table(&matrix, i);
			return (NULL);
		}
		i++;
	}
	return (matrix);
}

void	init_table(char **matrix, int rows, int columns)
{
	int	i;
	int	j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < columns)
		{
			matrix[i][j] = ' ';
			j++;
		}
		i++;
	}
}

char	**copy_table(char **matrix, int rows, int columns)
{
	char	**copy;
	int		i;
	int		j;

	copy = create_table(rows, columns);
	if (copy == NULL)
		return (NULL);
	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < columns)
		{
			/* Antes incrementabas i dentro del bucle de columnas. */
			copy[i][j] = matrix[i][j];
			j++;
		}
		i++;
	}
	return (copy);
}

void	move_position(t_point *pen, int x, int y)
{
	pen->x += x;
	pen->y += y;
}

void	draw_table(char **matrix, char *buffer, int size, t_point *pen)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (buffer[i] == 'x')
		{
			pen->flag = !pen->flag;
			/* Al bajar el lápiz, se dibuja inmediatamente la casilla actual. */
			if (pen->flag == 1)
				matrix[pen->y][pen->x] = 'O';
		}
		/* Antes las condiciones permitían y = -1 o y = height. */
		else if (buffer[i] == 'w' && pen->y > 0)
			move_position(pen, 0, -1);
		else if (buffer[i] == 's' && pen->y + 1 < pen->height)
			move_position(pen, 0, 1);
		else if (buffer[i] == 'a' && pen->x > 0)
			move_position(pen, -1, 0);
		else if (buffer[i] == 'd' && pen->x + 1 < pen->width)
			move_position(pen, 1, 0);
		/*
		** Si el comando era un movimiento válido y el lápiz está bajado,
		** la nueva posición se convierte en célula viva. Si el movimiento
		** intentaba salir del tablero, la posición no cambia y no pasa nada.
		*/
		if (buffer[i] != 'x' && pen->flag == 1
			&& (buffer[i] == 'w' || buffer[i] == 'a'
				|| buffer[i] == 's' || buffer[i] == 'd'))
			matrix[pen->y][pen->x] = 'O';
		i++;
	}
}

int	is_alive(int y, int x, char **matrix)
{
	return (matrix[y][x] == 'O');
}

int	show(int x, int y, char **matrix, int rows, int columns)
{
	if (x < 0 || x >= columns || y < 0 || y >= rows)
		return (0);
	return (matrix[y][x] == 'O');
}

int	count_neighbors(char **matrix, int rows, int columns, int y, int x)
{
	int	count;
	int	dy;
	int	dx;

	count = 0;
	dy = -1;
	while (dy <= 1)
	{
		dx = -1;
		while (dx <= 1)
		{
			/* No contamos la propia célula, solo sus ocho vecinas. */
			if ((dx != 0 || dy != 0)
				&& show(x + dx, y + dy, matrix, rows, columns))
				count++;
			dx++;
		}
		dy++;
	}
	return (count);
}

void	modify_table(char **old_matrix, char **new_matrix,
		int y, int x, int count)
{
	/*
	** Antes consultabas el estado dentro de la copia que estabas cambiando.
	** Todas las células de una iteración deben mirar el tablero anterior,
	** por eso leemos old_matrix y escribimos únicamente en new_matrix.
	*/
	if (is_alive(y, x, old_matrix))
	{
		if (count == 2 || count == 3)
			new_matrix[y][x] = 'O';
		else
			new_matrix[y][x] = ' ';
	}
	else if (count == 3)
		new_matrix[y][x] = 'O';
	else
		new_matrix[y][x] = ' ';
}

void	print_table(char **matrix, int rows, int columns)
{
	int	i;
	int	j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < columns)
		{
			/* Antes imprimías un espacio extra después de cada célula. */
			putchar(matrix[i][j]);
			j++;
		}
		putchar('\n');
		i++;
	}
}

void	delete_pos(t_point **pen)
{
	if (pen != NULL && *pen != NULL)
		free(*pen);
	if (pen != NULL)
		*pen = NULL;
}
