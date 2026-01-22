.PHONY : all clean fclean re bonus clean-lib clean-bin clean-obj debug debug-CXX debug-print
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
DEPENDANCIES = -MMD -MP
NO_DIR = --no-print-directory
MAKE := $(MAKE) -j $(NO_DIR)
NAME = PmergeMe

# Debugging flags
CXXFLAGS_DEBUG = -Wall -Wextra -g3 -pg -D DEBUG_LEVEL=1 -std=c++98
CXX_DEBUG = clang++
CXX_DEBUG_CXXFLAGS = -std=c++98 -g3 -D DEBUG_LEVEL=1 -Weverything -Wno-padded -pedantic -O2 -Wwrite-strings -Wconversion -fsanitize=address -fsanitize=leak
#############################################################################################
#                                                                                           #
#                                         DIRECTORIES                                       #
#                                                                                           #
#############################################################################################
# Source directories
P_SRC = ./
P_OBJ = .obj/

P_INC = ./

#############################################################################################
#                                                                                           #
#                                           FILES                                           #
#                                                                                           #
#############################################################################################
# Headers
INC = \
	PmergeMe.hpp \
	PmergeMe.tpp \

# Source files
SRC = \
	main.cpp \
	PmergeMe.cpp \

LIBS = \

#############################################################################################
#                                                                                           #
#                                        MANIPULATION                                       #
#                                                                                           #
#############################################################################################
SRCS =	\
	$(addprefix $(P_SRC), $(SRC)) 

# List of object files (redirect to P_OBJ)
OBJS = $(subst $(P_SRC), $(P_OBJ), $(SRCS:.cpp=.o))
P_OBJS = $(subst $(P_SRC), $(P_OBJ), $(SRCS))

# List of depedencies
DEPS = $(OBJS:%.o=%.d)

# List of header files
INCS = $(addprefix $(P_INC), $(INC))

#############################################################################################
#                                                                                           #
#                                          RULES                                            #
#                                                                                           #
#############################################################################################
all: 
	@$(MAKE) $(NAME)

# Create $(NAME) executable
$(NAME): $(OBJS) $(INCS)
	$(CXX) $(CXXFLAGS) $(DEPENDANCIES) $(DEBUG_STATE) -I $(P_INC) -o $(NAME) $(OBJS)

# Custom rule to compilate all .cpp with there path
$(P_OBJ)%.o: $(P_SRC)%.cpp $(INCS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPENDANCIES) $(DEBUG_STATE) -I $(P_INC) -c $< -o $@

#############################################################################################
#                                                                                           #
#                                      Other RULES                                          #
#                                                                                           #
#############################################################################################
# Rules for clean up
clean:
	rm -rfd $(P_OBJ)
	rm -rfd $(OBJS)
	rm -rfd $(DEPS)

clean-lib:
	rm -rfd $(P_LIB)

clean-bin:
	rm -f $(NAME)

clean-obj:
	@$(MAKE) clean

fclean:
	@$(MAKE) clean-obj
	@$(MAKE) clean-lib
	@$(MAKE) clean-bin

re:
	@$(MAKE) fclean
	@$(MAKE) all

# Aliases
clear: clean
fclear: fclean
flcean: fclean
flcear: fclean

#############################################################################################
#                                                                                           #
#                                           DEBUG                                           #
#                                                                                           #
#############################################################################################
# Debugging rules
debug:
	@$(MAKE) $(NAME) CXXFLAGS="$(CXXFLAGS_DEBUG)"

debug-cxx:
	@$(MAKE) $(NAME) CXXFLAGS="$(CXX_DEBUG_CXXFLAGS)" CXX="$(CXX_DEBUG)"

# Debugging print
debug-print:
	@$(MAKE) debug-print-project
	@$(MAKE) debug-print-separator

debug-print-separator:
	@echo ""
	@echo "$(On_Yellow)____________________$(Color_Off)"
	@echo ""

define check_var
	@if [ "$(strip $(1))" = "" ]; then \
		echo "$(Red)No $(2) found$(Color_Off)"; \
	else \
		echo "$(Red)$(2): \n\t$(Blue)$(1)$(Color_Off)"; \
	fi
endef

debug-print-project:
	$(call check_var,$(NAME),"Project")
	$(call check_var,$(SRCS),"SRCS")
	$(call check_var,$(OBJS),"OBJS")
	$(call check_var,$(INCS),"INCS")
	$(call check_var,$(LIBS),"LIBS")

#############################################################################################
#                                                                                           #
#                                         COSMETIC                                          #
#                                                                                           #
#############################################################################################
# Reset
Color_Off=\033[0m       # Text Reset

# Regular Colors
Black=\033[0;30m
Red=\033[0;31m
Green=\033[0;32m
Yellow=\033[0;33m
Blue=\033[0;34m
Purple=\033[0;35m
Cyan=\033[0;36m
White=\033[0;37m

# Background
On_Black=\033[40m
On_Red=\033[41m
On_Green=\033[42m
On_Yellow=\033[43m
On_Blue=\033[44m
On_Purple=\033[45m
On_Cyan=\033[46m
On_White=\033[47m

-include $(DEPS)% 