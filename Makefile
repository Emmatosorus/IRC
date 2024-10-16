NAME = ircserv
GUARDIAN = guardian
AI = ai

CXX = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
DEBUGFLAGS =

OBJS_DIR = .obj/
OBJS_AI_DIR = $(OBJS_DIR)bots/ai/
OBJS_BOT_DIR = $(OBJS_DIR)bots/guardian/
SRC_DIR = src/
SRC_AI_DIR = $(SRC_DIR)bots/ai/
SRC_BOT_DIR = $(SRC_DIR)bots/guardian/

HEADERS = include/Channel.hpp include/Client.hpp include/Server.hpp include/client_msg_parse.hpp include/bot.hpp

SRCS_BOT = guardian_bot.cpp
SRCS_AI = ai.cpp
SRCS_COMMANDS = $(addprefix commands/, join.cpp names.cpp nick.cpp notice.cpp pass.cpp ping.cpp pong.cpp privmsg.cpp quit.cpp user.cpp invite.cpp topic.cpp)
SRCS = main.cpp client_msg_parse.cpp Channel.cpp Client.cpp Server.cpp $(SRCS_COMMANDS)
OBJS = $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
OBJS_AI = $(addprefix $(OBJS_AI_DIR), $(SRCS_AI:.cpp=.o))
OBJS_BOT = $(addprefix $(OBJS_BOT_DIR), $(SRCS_BOT:.cpp=.o))

.SILENT:

all : obj $(NAME)

guardian_launch : obj $(GUARDIAN)

ai_launch: obj $(AI)

debug : CXX=g++
debug : DEBUGFLAGS = -g3 -fsanitize=address
debug : obj $(NAME)

valgrind : CXX=g++
valgrind : DEBUGFLAGS = -g3
valgrind : obj $(NAME)
	valgrind  --track-fds=yes ./$(NAME) 123

obj :
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/commands
	@mkdir -p $(OBJS_AI_DIR)
	@mkdir -p $(OBJS_BOT_DIR)

$(NAME) : $(OBJS)
	@echo -n "$(Red)Compiling ft_irc ..${NC}" && sleep 0.2
	@echo -n "$(Red)\rCompiling ft_irc ...${NC}"
	$(CXX) $^ $(CPPFLAGS) $(DEBUGFLAGS) -o $(NAME) && sleep 0.1
	@echo "$(Green)\r------COMPILATION COMPLETE-------${NC}"

$(GUARDIAN) : $(OBJS_BOT)
	@echo -n "$(Red)Compiling guardian bot ..${NC}" && sleep 0.2
	@echo -n "$(Red)\rCompiling guardian bot ...${NC}"
	$(CXX) $^ $(CPPFLAGS) -o $(GUARDIAN) && sleep 0.1
	@echo "$(Green)\r------Compiling COMPLETE!-------${NC}"

$(AI) : $(OBJS_AI)
	@echo -n "$(Red)Compiling ai bot ..${NC}" && sleep 0.2
	@echo -n "$(Red)\rCompiling ai bot ...${NC}"
	$(CXX) $^ $(CPPFLAGS) -o $(AI) && sleep 0.1
	@echo "$(Green)\r------COMPILATION COMPLETE!-------${NC}"

$(OBJS_DIR)%.o : $(SRC_DIR)%.cpp Makefile $(HEADERS)
	$(CXX) $(CPPFLAGS) $(DEBUGFLAGS) -c $< -o $@

$(OBJS_AI_DIR)%.o : $(SRC_AI_DIR)%.cpp Makefile $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $< -o $@

$(OBJS_BOT_DIR)%.o : $(SRC_BOT_DIR)%.cpp Makefile $(HEADERS)
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean :
	@rm -rf $(OBJS_DIR)

fclean :
	@rm -rf $(OBJS_DIR) $(NAME) guardian ai

re : fclean
	$(MAKE) all

# allows printing of Makefile variables
# make print-SRCS ------> prints content of the SRCS
print-%: 
	@echo $*=$($*)

sus:
	@echo "$(IRed)           ⣠⣤⣤⣤⣤⣤⣶⣦⣤⣄⡀        $(NC)"
	@echo "$(IRed)        ⢀⣴⣿⡿⠛⠉⠙⠛⠛⠛⠛⠻⢿⣿⣷⣤⡀     $(NC)"
	@echo "$(IRed)        ⣼⣿⠋       ⢀⣀⣀⠈⢻⣿⣿⡄    $(NC)"
	@echo "$(IRed)       ⣸⣿⡏   ⣠⣶⣾⣿⣿⣿⠿⠿⠿⢿⣿⣿⣿⣄   $(NC)"
	@echo "$(IRed)       ⣿⣿⠁  ⢰⣿⣿⣯⠁       ⠈⠙⢿⣷⡄ $(NC)"
	@echo "$(IRed)  ⣀⣤⣴⣶⣶⣿⡟   ⢸⣿⣿⣿⣆          ⣿⣷ $(NC)"
	@echo "$(IRed) ⢰⣿⡟⠋⠉⣹⣿⡇   ⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿⣿ $(NC)"
	@echo "$(IRed) ⢸⣿⡇  ⣿⣿⡇    ⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃ $(NC)"
	@echo "$(IRed) ⣸⣿⡇  ⣿⣿⡇     ⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇  $(NC)"
	@echo "$(IRed) ⠸⣿⣧⡀ ⣿⣿⡇                ⣿⣿⠃  $(NC)"
	@echo "$(IRed)  ⠛⢿⣿⣿⣿⣿⣇     ⣰⣿⣿⣷⣶⣶⣶⣶⠶ ⢠⣿⣿   $(NC)"
	@echo "$(IRed)       ⣿⣿     ⣿⣿⡇ ⣽⣿⡏⠁  ⢸⣿⡇   $(NC)"
	@echo "$(IRed)       ⣿⣿     ⣿⣿⡇ ⢹⣿⡆   ⣸⣿⠇   $(NC)"
	@echo "$(IRed)       ⢿⣿⣦⣄⣀⣠⣴⣿⣿⠁ ⠈⠻⣿⣿⣿⣿⡿⠏    $(NC)"
	@echo "$(IRed)       ⠈⠛⠻⠿⠿⠿⠿⠋⠁              $(NC)"
	@echo "$(White)         ░█▀▀░█░█░█▀▀$(NC)"
	@echo "$(White)         ░▀▀█░█░█░▀▀█$(NC)"
	@echo "$(White)         ░▀▀▀░▀▀▀░▀▀▀$(NC)"

.PHONY : all clean fclean re obj debug valgrind print-% sus

# COLORS =======================================================================

# Reset
NC=\033[0m

# Regular Colors
Black=\033[0;30m
Red=\033[0;31m
Green=\033[0;32m
Yellow=\033[0;33m
Blue=\033[0;34m
Purple=\033[0;35m
Cyan=\033[0;36m
White=\033[0;37m

# Bold
BBlack=\033[1;30m
BRed=\033[1;31m
BGreen=\033[1;32m
BYellow=\033[1;33m
BBlue=\033[1;34m
BPurple=\033[1;35m
BCyan=\033[1;36m
BWhite=\033[1;37m

# Underline
UBlack=\033[4;30m
URed=\033[4;31m
UGreen=\033[4;32m
UYellow=\033[4;33m
UBlue=\033[4;34m
UPurple=\033[4;35m
UCyan=\033[4;36m
UWhite=\033[4;37m

# Background
On_Black=\033[40m
On_Red=\033[41m
On_Green=\033[42m
On_Yellow=\033[43m
On_Blue=\033[44m
On_Purple=\033[45m
On_Cyan=\033[46m
On_White=\033[47m

# High Intensity
IBlack=\033[0;90m
IRed=\033[0;91m
IGreen=\033[0;92m
IYellow=\033[0;93m
IBlue=\033[0;94m
IPurple=\033[0;95m
ICyan=\033[0;96m
IWhite=\033[0;97m

# Bold High Intensity
BIBlack=\033[1;90m
BIRed=\033[1;91m
BIGreen=\033[1;92m
BIYellow=\033[1;93m
BIBlue=\033[1;94m
BIPurple=\033[1;95m
BICyan=\033[1;96m
BIWhite=\033[1;97m

# High Intensity backgrounds
On_IBlack=\033[0;100m
On_IRed=\033[0;101m
On_IGreen=\033[0;102m
On_IYellow=\033[0;103m
On_IBlue=\033[0;104m
On_IPurple=\033[0;105m
On_ICyan=\033[0;106m
On_IWhite=\033[0;107m
