/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: virginie <virginie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:22:36 by virginie          #+#    #+#             */
/*   Updated: 2023/11/27 22:28:13 by virginie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <fcntl.h>
# include <limits.h>

# define MAX_WIDTH 640
# define MAX_HEIGHT 480
# define BUFFER_SIZE 1024

// Structure pour stocker les données de la carte
typedef struct  s_map
{
    char    **map;
    int     width;
    int     height;
    int     player_x;
    int     player_y;
    char    player_dir; 
    char    *north_texture_path;
    char    *south_texture_path;
    char    *west_texture_path;
    char    *east_texture_path;
}               t_map;

bool check_missing_characters(const char *map);
bool check_duplicate_characters(const char *map);
bool check_resolution(int width, int height);
bool contains_non_digit(const char *str);
bool check_resolution_format(const char *resolutionStr);
bool check_wall(t_map *map_info, int x, int y);
bool check_borders(t_map *map_info, int x, int y, int dx, int dy);
bool check_player_recursive(t_map *map_info, int x, int y, int *player_count);
bool check_single_player(t_map *map_info);
bool check_valid_characters(t_map *map_info);
int validate_map(t_map *map_info);
void replace_spaces_with_walls(t_map *map_info, int max_length);
int add_line_to_map(t_map *map_info, const char *line);
bool parse_resolution(char *buffer, t_map *map_info);
bool validate_color_value(int value);
bool parse_and_validate_color_value(char **ptr, int *value);
bool parse_color(char *buffer, int *r, int *g, int *b);
bool process_map_line(char *buffer, t_map *map_info, int fd);
bool parse_north_texture(char *buffer, t_map *map_info);
bool parse_south_texture(char *buffer, t_map *map_info);
bool parse_west_texture(char *buffer, t_map *map_info);
bool parse_east_texture(char *buffer, t_map *map_info);
bool process_texture_line(char *buffer, t_map *map_info);
bool handle_line(char *buffer, t_map *map_info, int fd);
void update_line_counts(char c, int *length, int *max_length, int *line_count);
void process_file_buffer(const char *buffer, ssize_t bytes_read, int *length, int *max_length, int *line_count);
void count_lines_and_max_length(const char *filename, int *max_length, int *line_count);
int store_line_in_map(char *line, t_map *map_info, int *line_index);
int read_line(int fd, char *line, t_map *map_info, int max_length, int *line_index);
int initialize_map_array(t_map *map_info, int line_count);
int open_file_and_allocate_line(const char *filename, char **line, int max_length);
int parse_to_char_array(const char *filename, t_map *map_info);
void free_map(t_map *map_info);
int check_file_accessibility(const char *filename);
int check_arguments(int ac, char **av);

#endif