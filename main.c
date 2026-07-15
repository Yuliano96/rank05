#include "tools(1).h"

int	main(int argc, char **argv)
{
	int		width;
	int		height;
	int		iterations;
	int		bytes_read;
	int		generation;
	int		y;
	int		x;
	int		count;
	t_point	*pen;
	char	buffer[MAX_SIZE];
	char	**matrix;
	char	**next_matrix;

	if (argc != 4)
		return (1);
	width = atoi(argv[1]);
	height = atoi(argv[2]);
	iterations = atoi(argv[3]);
	if (width <= 0 || height <= 0 || iterations < 0)
		return (1);
	pen = init_pen(width, height);
	matrix = create_table(height, width);
	if (pen == NULL || matrix == NULL)
	{
		delete_pos(&pen);
		delete_table(&matrix, height);
		return (1);
	}
	init_table(matrix, height, width);
	/*
	** Antes llamabas a read() una sola vez. Ahora se procesa cada bloque
	** y se sigue leyendo hasta EOF, por lo que no hay límite de 100 comandos.
	*/
	bytes_read = read_commands(buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		draw_table(matrix, buffer, bytes_read, pen);
		bytes_read = read_commands(buffer, sizeof(buffer));
	}
	if (bytes_read < 0)
	{
		delete_table(&matrix, height);
		delete_pos(&pen);
		return (1);
	}
	generation = 0;
	while (generation < iterations)
	{
		next_matrix = copy_table(matrix, height, width);
		if (next_matrix == NULL)
		{
			delete_table(&matrix, height);
			delete_pos(&pen);
			return (1);
		}
		y = 0;
		while (y < height)
		{
			x = 0;
			while (x < width)
			{
				count = count_neighbors(matrix, height, width, y, x); //cuento la celulas vivas al mi alrededor
				modify_table(matrix, next_matrix, y, x, count); 
				x++;
			}
			y++;
		}
		delete_table(&matrix, height);
		matrix = next_matrix;
		generation++;
	}
	print_table(matrix, height, width);
	delete_table(&matrix, height);
	delete_pos(&pen);
	return (0);
}
