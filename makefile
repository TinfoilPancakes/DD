#
#	C / C++ Project Template.
#	Version 0.0.0 r2
#	Prateek R. Patil
#

NAME		:=	dd

CC			:=	clang++
C_FLAGS		:=	-Wall -Wextra -O3
CXX_FLAGS 	:=	-Wall -Wextra -O3 -std=c++14 -fsanitize=thread
LNK_FLAGS	:=	-Wall -Wextra -O3 -std=c++14 -L. -fsanitize=thread
LNK_LIBS	:=	SDL2 pthread

SRC_DIR 	:=	./src
INC_DIR		:=	./inc
OBJ_DIR		:=	./.obj

#	Including source files.
#	Variable should be named C_FILES or CXX_FILES

-include sources.mk

SOURCES		:=	$(addprefix $(SRC_DIR)/, $(C_FILES)) \
				$(addprefix $(SRC_DIR)/, $(CXX_FILES))

INCUDES		:=	$(addprefix -I , $(INC_DIR))

OBJECTS		:=	$(addprefix $(OBJ_DIR)/, $(patsubst %.c, %.c.o, $(C_FILES))) \
				$(addprefix $(OBJ_DIR)/, $(patsubst %.cpp, %.cpp.o, $(CXX_FILES)))

LIBRARIES	:=	$(addprefix -l , $(LNK_LIBS))

all : $(NAME)

#	Implicit rule for object files

$(OBJ_DIR)/%.c.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@printf "Compiling [ %s ]: " $<
	@$(CC) $(C_FLAGS) -c $< -o $@ $(INCUDES)
	@printf "Done.\n"

$(OBJ_DIR)/%.cpp.o : $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@printf "Compiling [ %s ]: " $<
	@$(CC) $(CXX_FLAGS) -c $< -o $@ $(INCUDES)
	@printf "Done.\n"

$(NAME) : $(OBJECTS)
	@printf "Linking [ %s ]: " $(NAME)
	@$(CC) $(LNK_FLAGS) -o $(NAME) $(OBJECTS) $(INCUDES) $(LIBRARIES)
	@printf "Done.\n"

fclean : clean
	@printf "Cleaning [ %s ]: " $(NAME)
	@rm -rf $(NAME)
	@printf "Done.\n"

clean :
	@printf "Cleaning [ %s ]: " $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)
	@printf "Done.\n"

re : fclean all

.PHONY : re fclean clean