#include <iostream>
#include <unistd.h>
#include <string>

int isRunning = 1;

void Clear()
// Clears text from console
{
#if defined _WIN32
    system("cls");
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined(__APPLE__)
    system("clear");
#endif
}
int validate_input(std::string& input, int max_opts=-1){
    // checks if use inpput is valid. 
    // returns input as intif valid else -1
    try
        {
            int opt = stoi(input); // try converting input to int
            if(max_opts>0){
                if (-1 < opt && opt <= max_opts){return opt;}
                else{ return -1;}// if input in range of menu options 
            }
            else{ return opt;}// if input is outside range of menu options , input is invalid
        }
    catch (std::invalid_argument){ return -1; }
     // if we cant convert input to int , then input is not valid

};

void draw_box(int line_length)
{
    // draws dashes around menus. line length is the width of the line to be drawn
    for (int i = 0; i < line_length; i++)
    {
        std::cout << "-";
    }
    std::cout << std::endl;
}
void show_welcome_message()
{
    // displays welcome message
    std::string welcome_message = "  welcome\n\n\t  To Your\n\n\tMomo wallet";
    draw_box(welcome_message.length());
    std::cout << "\n\n\n\n\t" << welcome_message << "\n\n\n\n"
              << std::endl;
    draw_box(welcome_message.length());
}


// BASE class for all screens
class Screen
{
public:
    int num_opts;
    virtual void display(bool showPrompt=true){};
    virtual Screen *(handle_selection)(int selection) { return new Screen; };
};

class SendToMomoUserScreen : public Screen
{
public:
    SendToMomoUserScreen() { num_opts = -1; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t   Send To MoMo User\n"
                     "\n\t   Enter Phone Number\n";
        draw_box(45);
        if(showPrompt==1){std::cout << "Phone Number==> ";}
        
    }
    Screen *handle_selection(int selection);
};

class MomoScreen : public Screen
{
public:
    MomoScreen() { num_opts = 6; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t\t   Transfer Money\n"
                     "\n\tselect an option\n\n";
        std::cout << "\t1\tMomo User\n"
                     "\t2\tNon Momo User\n"
                     "\t3\tSend With Care\n"
                     "\t4\tFavourite\n"
                     "\t5\tOther Networks\n"
                     "\t6\tBank Account\n"
                     "\t0\tMain Menu\n\n";
        draw_box(45);
        if(showPrompt == 1){std::cout << "Enter your selection==> ";}
    }
    Screen *handle_selection(int selection);
};

class HomeScreen : public Screen
{
public:
    HomeScreen() { num_opts = 6; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t\t   Main Menu\n"
                     "\n\t\tSelect An Option\n\n";
        std::cout << "\t1\tTransfer Money\n"
                     "\t2\tMoMoPay & Pay Bill\n"
                     "\t3\tAirtime & Bundles\n"
                     "\t4\tAllow Cash Out\n"
                     "\t5\tFinancial Services\n"
                     "\t6\tMy Wallet\n"
                     "\t0\texit\n\n";
        draw_box(45);
        if(showPrompt==1){std::cout << "Enter your selection==> ";}
    }

    Screen *handle_selection(int selection);
};


Screen *MomoScreen::handle_selection(int selection)
{
    switch (selection)
        {
        case 0: // Go back to home screen
            HomeScreen *ms_ptr;
            ms_ptr = new HomeScreen();
            ms_ptr->display();
            return ms_ptr;
        case 1: //  transfer to momo user screen
            SendToMomoUserScreen *stms_ptr;
            stms_ptr = new SendToMomoUserScreen();
            stms_ptr->display();
            return stms_ptr;
        default:
            this->display();
            return this;
        }
}

Screen *HomeScreen::handle_selection(int selection)
{
    switch (selection)
    {
    
    case 0:
        /* todo graceful shutdown */
        isRunning = 0;
        return this;
    case 1:
        MomoScreen *ms_ptr;
        ms_ptr = new MomoScreen();
        ms_ptr->display();
        return ms_ptr;
    default:
        this->display();
        return this;
    }
}

Screen *SendToMomoUserScreen::handle_selection(int selection)
{
     HomeScreen *hs_ptr;
        hs_ptr = new HomeScreen();
        hs_ptr->display();
        return hs_ptr;
}

int main()
{
    show_welcome_message(); // display welcome message
    sleep(1);
    Clear();                // wait for welcome message to display and clear screen
    HomeScreen home_screen; // create the home screen object and display menuL options

    Screen *currentScreen = &home_screen; // create a pointer to the current screen using the base class so all subclasses can access pointer1
    currentScreen->display();             // display the currnent screen and
    std::string input;                    //get  user input

    while (isRunning) // listen for commands
    {
        getline(std::cin, input);
        int opt = validate_input(input, currentScreen->num_opts);
        if (opt >-1){
            Clear();
            currentScreen = currentScreen->handle_selection(opt);
        }else{
            Clear();
            currentScreen->display(false); // clear the screen and display  options
            std::cerr << "Select a valid option\n==>  ";
        }
    }

    return 0;
}
