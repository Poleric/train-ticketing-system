#ifndef TRAINTICKETINGSYSTEM_MENU_UTILS_H
#define TRAINTICKETINGSYSTEM_MENU_UTILS_H

#include <tui/form/login_form.h>

typedef enum CurrentMenu current_menu_t;
typedef enum StatusColor status_color_t;

enum CurrentMenu {
    EXIT_MENU = 0,
    MEMBER_MENU = 1,
    STAFF_MENU = 2
};

enum StatusColor {
    COLOR_1 = 1,
    COLOR_2 = 2,
    SELECTED = 3,
    ERROR = 4
};

#define MINORI "..........................................................................................\n..........................................................................................\n..........................................................................................\n...............................................:::........................................\n................................::=+++===============:....................................\n.............................:-=========================-:................................\n............................-======++++===*=+==============:..............................\n..........................:+========***+=++=====*+==========-.............................\n.........................-==============*#++=======+==========-...........................\n.......................:-=================#*====================..........................\n......................:=============+##*===#**######*==========+=:........................\n.....................:==========+++#######++++#########+=++=======-.......................\n....................:==========*#+###########+*############+==++==+-......................\n....................=======++++###+###########+###########%#===+*=++-.....................\n...................=======+==+#%##=#######################%%#==+#+=*+=:...................\n..................-=======+=+#%*+%=#############%##########%%#=+##++===...................\n..................=======++#*#*=++*+############%%#%#######%%%#+###=*==-..................\n.................-=======+**##==++#=####%##########%%#######%#%*###+++==-.................\n................:=+=====++=*%+=++++#+#%##%#######%#%%%%%####%#%####*=====-................\n................-+====+===+##-=+++++%+%%##%%#####%+*%##%%###%###%###=====-:...............\n...............:=++==++==+##-.-=+++++++%###%%#####++*#**#%#####%####====-=-...............\n...............=*+===+=+#%=:..:=++++++++%%##%###%#++++#++##%#%#%####===+:-=-..............\n..............:#++==+*++=:....:=++++++++*#%#####%%++#%@@#++##%#%+#*#====-.==:.............\n..............+=++==##:=-:..-+@%%%%%#+++*#########@%#%%%%#%@%%##++*#=====.-=-.............\n.............-*+++=+#-:==:.:##--#%%%#++++++*#++++**#%%%%%%#%##%#++*#=====..==-............\n............:#=++++*%#--=-:+%--*%%%@%%+++++++#+++++#@%@@@%%###@+=++#+====:.:=-:...........\n............+*==*++*%##+--::#.:*%%@@@%+++++++++++++#%%@@@%%+#%#===+#*====:..+--...........\n...........-+=+=+++*%##%+:=:::.+%%%%@#++++++++++++++%@%%%@#+%*+===+##+===-..-=-...........\n..........-=+=++++*###%*#++-...:+%@%#+++++++*%#++++++%@@%++**#+++=+####++=-..=:...........\n..........==+=++=*+##+----=#:....-+++++++++++##++++++++++*%%#***+==*%%##*++*==:...........\n.........-==+==+=+#+----+*--+:...-++++++++++++++++++*++%*==#%#####++%+=*##++%:............\n.........-==+==+##=---+-.....:-:.-+++*#+++++++++++*###=-=###########+=##++#*=#-...........\n.........-==+=+%=---==.........::-=++++++*++++****#+==+###############*###+*#+++:.........\n.........:===%=----+:............:=-==++++++++++%+-=*#####################%%*#=-*=........\n..........:++-----=:...........:+#++=-----=+##%+=+#########################%*##=..........\n.........-+=-----=-..........:-+-.........-=##+*##################%##########%+*=.........\n.......:++------==..........:==........:--#%###################################=:.........\n.....:=#--------=.........:-+:......::-=#%####################%####%###########*-.........\n....-*=--------=::::......--......:-==#%#####################%%####%#############+:.......\n...=+---------++..--.....-:......:-=#%######################%%#####%###########*#%*.......\n..-=-----------##+=::...--:.:-..:-=%#######################%%##################*#*%=......\n.-++-------------=+*=-:-+-..:--:-+########################%%%##%%%%############*#+#%=.....\n:=+=-------------------=++*+-:--=*#######################%%%#@%###%%%#########*##+*%%-....\n:=-=--------=----------=-------*%#######################%%%%##################+##++#%#-...\n"

void clear_login_menu(login_form_t* login_form);

#endif //TRAINTICKETINGSYSTEM_MENU_UTILS_H
