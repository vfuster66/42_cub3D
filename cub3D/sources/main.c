/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: virginie <virginie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:24:03 by virginie          #+#    #+#             */
/*   Updated: 2023/11/27 22:23:20 by virginie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

// Verification des caracteres manquants
bool check_missing_characters(const char *map)
{
    if (ft_strchr(map, 'N') == NULL || ft_strchr(map, 'S') == NULL
        || ft_strchr(map, 'E') == NULL || ft_strchr(map, 'W') == NULL)
    {
        printf("Error: Missing characters in the map.\n");
        return (false);
    }
    return (true);
}

// Verification des caracteres en double
bool check_duplicate_characters(const char *map)
{
    if (ft_strchr(map, 'N') != ft_strrchr(map, 'N')
        || ft_strchr(map, 'E') != ft_strrchr(map, 'E')
        || ft_strchr(map, 'S') != ft_strrchr(map, 'S')
        || ft_strchr(map, 'W') != ft_strrchr(map, 'W'))
    {
        printf("Error: Duplicate characters in the map.\n");
        return (false);
    }
    return (true);
}

// Verification des valeurs incorrectes de la resolution
bool check_resolution(int width, int height)
{
    if (width <= 0 || height <= 0 || width > INT_MAX || height > INT_MAX)
    {
        printf("Error: Invalid resolution.\n");
        return false;
    }
    return true;
}

// Verification de valeurs non numeriques
bool contains_non_digit(const char *str)
{
    int i;

    i = 0;
    while (str[i] != '\0')
    {
        if (!ft_isdigit(str[i]) && str[i] != ',')
        {
            return (true);
        }
        i++;
    }
    return (false);
}

// Verification du format de resolution
bool check_resolution_format(const char *resolutionStr)
{
    if (contains_non_digit(resolutionStr))
    {
        printf("Error: Invalid characters in resolution.\n");
        return (false);
    }
    return (true);
}

// Verification des murs
bool check_wall(t_map *map_info, int x, int y)
{
    if (x < 0 || y < 0 || x >= map_info->width || y >= map_info->height)
        return (true);
    return (map_info->map[y][x] == '1');
}

// Verification des limites de la map
bool check_borders(t_map *map_info, int x, int y, int dx, int dy)
{
    if (x < 0 || y < 0 || x >= map_info->width || y >= map_info->height)
        return (true);
    if (!check_wall(map_info, x, y))
    {
        printf("Error: Map is not surrounded by walls at (%d, %d).\n", x, y);
        return (false);
    }
    return (check_borders(map_info, x + dx, y + dy, dx, dy));
}

// Fonction récursive pour verifier la presence d'un seul player
bool check_player_recursive(t_map *map_info, int x, int y, int *player_count)
{
    if (x < 0 || y < 0 || x >= map_info->width || y >= map_info->height)
        return (true);
    if (ft_strchr("NSEW", map_info->map[y][x]))
    {
        (*player_count)++;
        if (*player_count > 1)
        {
            printf("Error: Multiple player start positions found.\n");
            return (false);
        }
        map_info->player_x = x;
        map_info->player_y = y;
        map_info->player_dir = map_info->map[y][x];
        map_info->map[y][x] = '0'; 
    }
    if (x < map_info->width - 1)
        return (check_player_recursive(map_info, x + 1, y, player_count));
    else if (y < map_info->height - 1)
        return (check_player_recursive(map_info, 0, y + 1, player_count));
    return (true);
}

// Verification du player
bool check_single_player(t_map *map_info)
{
    int player_count;
    
    player_count = 0;
    if (!check_player_recursive(map_info, 0, 0, &player_count))
        return (false);
    if (player_count == 0)
    {
        printf("Error: No player start position found.\n");
        return (false);
    }
    return (true);
}

// Verification de la validité des caractères dans la map
bool check_valid_characters(t_map *map_info)
{
    int     x;
    int     y;
    char    c;
    
    y = 0;
    while (y < map_info->height)
    {
        x = 0;
        while (x < strlen(map_info->map[y]))
        {
            c = map_info->map[y][x];
            if (c != '0' && c != '1' && c != '2' && c != 'N' && c != 'S'
                && c != 'E' && c != 'W')
            {
                printf("Error: Invalid character '%c' found in map at position(%d, %d).\n", c, x, y);
                return (false);
            }
            x++;
        }
        y++;
    }
    return (true);
}

// Verification de la validite de la map
int validate_map(t_map *map_info)
{
    if (!check_borders(map_info, 0, 0, 1, 0)
            || !check_borders(map_info, 0, map_info->height - 1, 1, 0))
        return (1);
    if (!check_borders(map_info, 0, 0, 0, 1)
            || !check_borders(map_info, map_info->width - 1, 0, 0, 1))
        return (1);
    if (!check_valid_characters(map_info))
        return (1);
    if (!check_single_player(map_info))
        return (1);
    return (0);
}

// Remplacer les espaces par des murs et ajuster la longueur des lignes
void replace_spaces_with_walls(t_map *map_info, int max_length)
{
    int y;
    int x;
    int current_length;

    y = 0;
    while (y < map_info->height)
    {
        current_length = strlen(map_info->map[y]);
        map_info->map[y] = realloc(map_info->map[y], max_length + 1);
        if (!map_info->map[y])
        {
            perror("Failed to reallocate memory for map line");
            exit(1);
        }
        x = current_length;
        while (x < max_length)
        {
            map_info->map[y][x] = '1';
            x++;
        }
        map_info->map[y][max_length] = '\0';
        y++;
    }
}

int add_line_to_map(t_map *map_info, const char *line)
{
    static int row;
    
    row = 0;
    if (row >= map_info->height)
        return (1);
    map_info->map[row] = ft_strdup(line);
    if (!map_info->map[row])
        return (1);
    replace_spaces_with_walls(map_info, row);
    row++;
    return (0);
}

// Verification de la resolution
bool parse_resolution(char *buffer, t_map *map_info)
{
    char *ptr = buffer + 2;
    map_info->width = strtol(ptr, &ptr, 10);
    map_info->height = strtol(ptr, NULL, 10);

    if (map_info->width <= 0 || map_info->height <= 0
        || map_info->width > MAX_WIDTH || map_info->height > MAX_HEIGHT)
    {
        printf("Error: Invalid resolution.\n");
        return (false);
    }
    if (ft_strchr(buffer, ',') || contains_non_digit(ptr))
    {
        printf("Error: Resolution contains invalid characters.\n");
        return (false);
    }
    return (true);
}

// Vérification des valeurs de couleur
bool validate_color_value(int value)
{
    return (value >= 0 && value <= 255);
}

// Conversion et vérification des valeurs de couleur
bool parse_and_validate_color_value(char **ptr, int *value)
{
    *value = strtol(*ptr, ptr, 10);
    if (**ptr != ',' || !validate_color_value(*value))
    {
        printf("Error: Invalid color values.\n");
        return false;
    }
    (*ptr)++;
    return true;
}

// Verification de la couleur
bool parse_color(char *buffer, int *r, int *g, int *b)
{
    char    *ptr;
    
    ptr = buffer + 2;
    if (buffer[0] != 'F' && buffer[0] != 'C')
    {
        printf
            ("Error: Invalid color identifier '%c'. Use 'F' or 'C'.\n",
            buffer[0]);
        return (false);
    }
    if (!parse_and_validate_color_value(&ptr, r)
        || !parse_and_validate_color_value(&ptr, g)
        || !parse_and_validate_color_value(&ptr, b))
        return (false);
    if (!validate_color_value(*r) || !validate_color_value(*g) || !validate_color_value(*b))
    {
        printf("Error: Invalid color values.\n");
        return (false);
    }
    return (true);
}

bool process_map_line(char *buffer, t_map *map_info, int fd)
{
    if (ft_strncmp(buffer, "R ", 2) == 0)
    {
        if (!parse_resolution(buffer, map_info))
            return (false);
    }
    else if (ft_strncmp(buffer, "F ", 2) == 0 || ft_strncmp(buffer, "C ", 2) == 0)
    {
        int r, g, b;
        if (!parse_color(buffer, &r, &g, &b))
            return (false);
    }
    else if (ft_strchr("012NSEW", buffer[0]))
    {
        if (!add_line_to_map(map_info, buffer))
            return (false);
    }
    return (true);
}

// Verification des textures
bool parse_north_texture(char *buffer, t_map *map_info)
{
    map_info->north_texture_path = ft_strdup(buffer + 3);
    if (!map_info->north_texture_path 
        || ft_strlen(map_info->north_texture_path) == 0)
    {
        printf("Error allocating memory for north texture.\n");
        return (false);
    }
    return (true);
}


bool parse_south_texture(char *buffer, t_map *map_info)
{
    map_info->south_texture_path = ft_strdup(buffer + 3);
    if (!map_info->south_texture_path
        || ft_strlen(map_info->south_texture_path) == 0)
    {
        printf("Error allocating memory for south texture.\n");
        return (false);
    }
    return (true);
}

bool parse_west_texture(char *buffer, t_map *map_info)
{
    map_info->west_texture_path = ft_strdup(buffer + 3);
    if (!map_info->west_texture_path
        || ft_strlen(map_info->west_texture_path) == 0)
    {
        printf("Error allocating memory for west texture.\n");
        return (false);
    }
    return (true);
}

bool parse_east_texture(char *buffer, t_map *map_info)
{
    map_info->east_texture_path = ft_strdup(buffer + 3);
    if (!map_info->east_texture_path
        || ft_strlen(map_info->east_texture_path) == 0)
    {
        printf("Error allocating memory for east texture.\n");
        return (false);
    }
    return (true);
}

bool process_texture_line(char *buffer, t_map *map_info)
{
    if (strncmp(buffer, "NO ", 3) == 0)
    {
        if (!parse_north_texture(buffer, map_info))
            return (false);
    }
    else if (strncmp(buffer, "SO ", 3) == 0)
    {
        if (!parse_south_texture(buffer, map_info))
            return (false);
    }
    else if (strncmp(buffer, "WE ", 3) == 0)
    {
        if (!parse_west_texture(buffer, map_info))
            return (false);
    }
    else if (strncmp(buffer, "EA ", 3) == 0)
    {
        if (!parse_east_texture(buffer, map_info))
            return (false);
    }
    return (true);
}

bool handle_line(char *buffer, t_map *map_info, int fd)
{
    if (!process_map_line(buffer, map_info, fd))
    {
        return (false);
    }
    if (!process_texture_line(buffer, map_info))
    {
        return (false);
    }
    return (true);
}

// Fonction auxiliaire pour mettre à jour le nombre de lignes et la longueur maximale d'une ligne
void update_line_counts(char c, int *length, int *max_length, int *line_count)
{
    (*length)++;
    if (c == '\n')
    {
        (*line_count)++;
        if (*length > *max_length)
            *max_length = *length;
        *length = 0;
    }
}

// Traiter le buffer et mettre à jour les compteurs
void process_file_buffer(const char *buffer, ssize_t bytes_read, int *length, int *max_length, int *line_count)
{
    int i;
    
    i = 0;
    while (i < bytes_read)
    {
        update_line_counts(buffer[i], length, max_length, line_count);
        i++;
    }
}

// Compter les lignes et trouver la longueur maximale d'une ligne
void count_lines_and_max_length(const char *filename, int *max_length, int *line_count)
{
    char buffer[BUFFER_SIZE];
    int fd = open(filename, O_RDONLY);
    ssize_t bytes_read;
    int length;
    
    length = 0;
    *max_length = 0;
    *line_count = 0;
    if (fd == -1)
    {
        perror("Error opening file");
        return;
    }
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0)
        process_file_buffer(buffer, bytes_read, &length, max_length, line_count);
    if (bytes_read == -1)
        perror("Error reading file");
    close(fd);
}

// Ajouter une ligne
int store_line_in_map(char *line, t_map *map_info, int *line_index)
{
    map_info->map[*line_index] = ft_strdup(line);
    if (!map_info->map[*line_index])
    {
        perror("Failed to allocate memory for map line");
        return (1);
    }
    (*line_index)++;
    return (0);
}

// Lire une ligne du fichier et l'ajouter au tableau de map
int read_line(int fd, char *line, t_map *map_info, int max_length, int *line_index)
{
    int char_index;
    ssize_t bytes_read;

    char_index = 0;
    while ((bytes_read = read(fd, line + char_index, 1)) > 0)
    {
        if (line[char_index] == '\n' || char_index == max_length - 1)
        {
            line[char_index] = '\0';
            if (store_line_in_map(line, map_info, line_index) == 1)
                return (1);
            return (0);
        }
        else
            char_index++;
    }
    if (bytes_read == -1)
    {
        perror("Error reading file");
        return (1);
    }
    return (0);
}

// Initialiser le tableau de map
int initialize_map_array(t_map *map_info, int line_count)
{
    map_info->map = malloc(sizeof(char*) * line_count);
    if (!map_info->map)
    {
        perror("Failed to allocate memory for map");
        return (1);
    }
    map_info->height = line_count;
    return (0);
}

// Ouvrir le fichier et allouer de la memoire pour la ligne
int open_file_and_allocate_line(const char *filename, char **line, int max_length)
{
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error opening file");
        return (-1);
    }
    *line = malloc(sizeof(char) * max_length);
    if (!*line)
    {
        perror("Failed to allocate memory for line");
        close(fd);
        return (-1);
    }
    return (fd);
}

// Parser le fichier dans un tableau de char à double entrée
int parse_to_char_array(const char *filename, t_map *map_info)
{
    int     max_length;
    int     line_count;
    int     line_index;
    int     fd;
    char    *line;

    count_lines_and_max_length(filename, &max_length, &line_count);
    if (initialize_map_array(map_info, line_count) != 0)
        return (1);
    fd = open_file_and_allocate_line(filename, &line, max_length);
    if (fd == -1)
        return (1);
    line_index = 1;
    while (line_index < line_count)
    {
        if (read_line(fd, line, map_info, max_length, &line_index) != 0)
        {
            free(line);
            close(fd);
            return (1);
        }
        line_index++;
    }
    close(fd);
    free(line);
    return (0);
}

// Fonction pour libérer la mémoire du tableau de map
void free_map(t_map *map_info)
{
    for (int i = 0; i < map_info->height; i++)
    {
        free(map_info->map[i]);
    }
    free(map_info->map);
}

// Vérifier l'accessibilité du fichier
int check_file_accessibility(const char *filename)
{
    int fd;
    
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Error: Unable to open the file");
        return (1);
    }
    close(fd);
    return (0);
}

// Vérifier les arguments
int check_arguments(int ac, char **av)
{
    char    *extension;

    if (ac != 2 && ac != 3)
    {
        printf("Usage: %s <map_file.cub> [--save]\n", av[0]);
        return (1);
    }
    if (ac == 3 && strcmp(av[2], "--save") != 0)
    {
        printf("Error: Invalid argument. The third argument must be '--save'\n");
        return (1);
    }
    extension = ft_strrchr(av[1], '.');
    if (extension == NULL || strcmp(extension, ".cub") != 0
        || strlen(extension) != 4)
    {
        printf("Error: The map file must have a '.cub' extension.\n");
        return (1);
    }
    return check_file_accessibility(av[1]);
}

int main(int ac, char **av)
{
    t_map map_info;

    if (check_arguments(ac, av) != 0)
        return (1);
    ft_memset(&map_info, 0, sizeof(t_map));
    if (parse_to_char_array(av[1], &map_info) != 0)
    {
        printf("Error parsing the map file.\n");
        return (1);
    }
    if (validate_map(&map_info) != 0)
    {
        printf("Invalid map.\n");
        free_map(&map_info);
        return (1);
    }
    free_map(&map_info);
    return (0);
}

