# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acossari <acossari@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/14 13:25:00 by acossari          #+#    #+#              #
#    Updated: 2025/09/30 20:09:24 by acossari         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                  TARGETS                                     #
# **************************************************************************** #

NAME = pipex

# **************************************************************************** #
#                              COMPILER CONFIG                                 #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror

# **************************************************************************** #
#                               DIRECTORIES                                    #
# **************************************************************************** #

SRCDIR = src
INCDIR = include
LIBFTDIR = libft
OBJDIR = obj

# **************************************************************************** #
#                              SOURCE FILES                                    #
# **************************************************************************** #

# Mandatory source files
SRCS = $(SRCDIR)/main.c \
       $(SRCDIR)/parsing.c \
       $(SRCDIR)/utils.c \
       $(SRCDIR)/path_resolution.c \
       $(SRCDIR)/pipeline.c \
       $(SRCDIR)/exec_utils.c

# Bonus source files
SRCS_BONUS = $(SRCDIR)/main_bonus.c \
             $(SRCDIR)/parsing_bonus.c \
             $(SRCDIR)/utils.c \
             $(SRCDIR)/path_resolution.c \
             $(SRCDIR)/exec_utils.c \
             $(SRCDIR)/pipeline_bonus.c \
             $(SRCDIR)/here_doc_bonus.c \
             $(SRCDIR)/pipe_management_bonus.c \
             $(SRCDIR)/pipe_utils_bonus.c \
             $(SRCDIR)/child_process_bonus.c

# **************************************************************************** #
#                              OBJECT FILES                                    #
# **************************************************************************** #

OBJS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS_BONUS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS_BONUS))

# **************************************************************************** #
#                                LIBRARIES                                     #
# **************************************************************************** #

LIBFT = $(LIBFTDIR)/libft.a

# **************************************************************************** #
#                            COMPILATION FLAGS                                 #
# **************************************************************************** #

INCLUDES = -I$(INCDIR) -I$(LIBFTDIR)/include

# **************************************************************************** #
#                            HEADER DEPENDENCIES                               #
# **************************************************************************** #

HEADERS = $(INCDIR)/pipex.h
HEADERS_BONUS = $(INCDIR)/pipex_bonus.h

# **************************************************************************** #
#                                 BUILD RULES                                  #
# **************************************************************************** #

# Default target - builds mandatory version
all: $(LIBFT) .mandatory

# Bonus target - builds bonus version with extra features
bonus: $(LIBFT) .bonus

# Marker file for mandatory version
.mandatory: $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@touch .mandatory
	@rm -f .bonus

# Marker file for bonus version
.bonus: $(OBJDIR) $(OBJS_BONUS)
	$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFT) -o $(NAME)
	@touch .bonus
	@rm -f .mandatory

# **************************************************************************** #
#                            COMPILATION RULES                                 #
# **************************************************************************** #

# Create object directory
$(OBJDIR):
	@mkdir -p $(OBJDIR)

# Pattern rule: compile any .c file in src/ to obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) $(HEADERS_BONUS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# **************************************************************************** #
#                              LIBRARY RULES                                   #
# **************************************************************************** #

# Build libft library
$(LIBFT):
	$(MAKE) -C $(LIBFTDIR)

# **************************************************************************** #
#                               CLEAN RULES                                    #
# **************************************************************************** #

# Remove object files
clean:
	rm -rf $(OBJDIR)
	$(MAKE) clean -C $(LIBFTDIR)

# Remove object files and executables
fclean: clean
	rm -f $(NAME) .mandatory .bonus
	$(MAKE) fclean -C $(LIBFTDIR)

# Rebuild everything from scratch
re: fclean all

# **************************************************************************** #
#                                 PHONY TARGETS                                #
# **************************************************************************** #

.PHONY: all bonus clean fclean re
