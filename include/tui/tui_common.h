#ifndef TRAINTICKETINGSYSTEM_TUI_COMMON_H
#define TRAINTICKETINGSYSTEM_TUI_COMMON_H

#include <panel.h>

typedef enum CurrentMenu {
    EXIT_MENU = 0,
    MEMBER_MENU = 1,
    STAFF_MENU = 2
} current_menu_t;

#define CTRL(c) ((c) & 0x1f)
#define KEY_ESC 27


#define MINORI "..........................................................................................\n..........................................................................................\n..........................................................................................\n...............................................:::........................................\n................................::=+++===============:....................................\n.............................:-=========================-:................................\n............................-======++++===*=+==============:..............................\n..........................:+========***+=++=====*+==========-.............................\n.........................-==============*#++=======+==========-...........................\n.......................:-=================#*====================..........................\n......................:=============+##*===#**######*==========+=:........................\n.....................:==========+++#######++++#########+=++=======-.......................\n....................:==========*#+###########+*############+==++==+-......................\n....................=======++++###+###########+###########%#===+*=++-.....................\n...................=======+==+#%##=#######################%%#==+#+=*+=:...................\n..................-=======+=+#%*+%=#############%##########%%#=+##++===...................\n..................=======++#*#*=++*+############%%#%#######%%%#+###=*==-..................\n.................-=======+**##==++#=####%##########%%#######%#%*###+++==-.................\n................:=+=====++=*%+=++++#+#%##%#######%#%%%%%####%#%####*=====-................\n................-+====+===+##-=+++++%+%%##%%#####%+*%##%%###%###%###=====-:...............\n...............:=++==++==+##-.-=+++++++%###%%#####++*#**#%#####%####====-=-...............\n...............=*+===+=+#%=:..:=++++++++%%##%###%#++++#++##%#%#%####===+:-=-..............\n..............:#++==+*++=:....:=++++++++*#%#####%%++#%@@#++##%#%+#*#====-.==:.............\n..............+=++==##:=-:..-+@%%%%%#+++*#########@%#%%%%#%@%%##++*#=====.-=-.............\n.............-*+++=+#-:==:.:##--#%%%#++++++*#++++**#%%%%%%#%##%#++*#=====..==-............\n............:#=++++*%#--=-:+%--*%%%@%%+++++++#+++++#@%@@@%%###@+=++#+====:.:=-:...........\n............+*==*++*%##+--::#.:*%%@@@%+++++++++++++#%%@@@%%+#%#===+#*====:..+--...........\n...........-+=+=+++*%##%+:=:::.+%%%%@#++++++++++++++%@%%%@#+%*+===+##+===-..-=-...........\n..........-=+=++++*###%*#++-...:+%@%#+++++++*%#++++++%@@%++**#+++=+####++=-..=:...........\n..........==+=++=*+##+----=#:....-+++++++++++##++++++++++*%%#***+==*%%##*++*==:...........\n.........-==+==+=+#+----+*--+:...-++++++++++++++++++*++%*==#%#####++%+=*##++%:............\n.........-==+==+##=---+-.....:-:.-+++*#+++++++++++*###=-=###########+=##++#*=#-...........\n.........-==+=+%=---==.........::-=++++++*++++****#+==+###############*###+*#+++:.........\n.........:===%=----+:............:=-==++++++++++%+-=*#####################%%*#=-*=........\n..........:++-----=:...........:+#++=-----=+##%+=+#########################%*##=..........\n.........-+=-----=-..........:-+-.........-=##+*##################%##########%+*=.........\n.......:++------==..........:==........:--#%###################################=:.........\n.....:=#--------=.........:-+:......::-=#%####################%####%###########*-.........\n....-*=--------=::::......--......:-==#%#####################%%####%#############+:.......\n...=+---------++..--.....-:......:-=#%######################%%#####%###########*#%*.......\n..-=-----------##+=::...--:.:-..:-=%#######################%%##################*#*%=......\n.-++-------------=+*=-:-+-..:--:-+########################%%%##%%%%############*#+#%=.....\n:=+=-------------------=++*+-:--=*#######################%%%#@%###%%%#########*##+*%%-....\n:=-=--------=----------=-------*%#######################%%%%##################+##++#%#-...\n" \


#endif //TRAINTICKETINGSYSTEM_TUI_COMMON_H
