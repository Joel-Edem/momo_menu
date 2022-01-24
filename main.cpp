#include <iostream>
#include <unistd.h>
#include <string>

int isRunning = 1;

void clear_screen()
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
long int validate_input(std::string& input, int max_opts=-1){
    // checks if use inpput is valid. 
    // returns input as intif valid else -1
    try
        {
            long int opt = stol(input); // try converting input to int
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
     sleep(1); clear_screen();
}

struct UserAccount{

    std::string phone_number;
    int balace ;
    int pin;
    UserAccount(
        std::string _phone_number = "0553272851",
        int _balance= 1000, int _pin=1234)
        {
            phone_number = _phone_number;
            balace = _balance;
            pin =_pin;
        }

};
UserAccount *user_account_ptr;

std::string phone_num_to_str(long int input)
{
    std::string output = "0";
    output += std::to_string(input);
    return output;
}
bool is_mtn_number(std::string phone_number)
{
    // (024), (054), (055) (059)
    std::string mtn_prefix[] = {"024", "054", "055", "059"};

    for (int i = 0; i<4; i++){
        int pos = phone_number.find(mtn_prefix[i]);
        if( pos != std::string::npos && pos == 0) {
           return true;
        }
    }
    return false;
    
}
void show_transaction_failed_message()
{
    draw_box(45);std::cout<<"\n\n\n\t\tTransaction Failed\n\n\n";draw_box(45);
    sleep(2);clear_screen();
}


// BASE class for all screens
class Screen
{
public:
    int num_opts;
    virtual void display(bool showPrompt=true){};
    virtual Screen *(handle_selection)(long int selection) { return new Screen; };
};

class TransactionAmtScreen : public Screen
{
public:
    int num_retries;
    std::string phone_number;
    TransactionAmtScreen(std::string _phone_number) { num_opts = -1;  num_retries=0; phone_number=_phone_number;}
    
    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t   Send To MoMo User\n";
           std::cout << "\n\t   Enter Amount\n";
        draw_box(45);
        if(showPrompt==1){std::cout << "GHS¢==> ";}
        
    }
    Screen *handle_selection(long int selection);
};

class SendToMomoUserScreen : public Screen
{
public:
    int num_retries;
    std::string phone_number;
    SendToMomoUserScreen() { num_opts = -1;  num_retries=0;}
    SendToMomoUserScreen(std::string _phone_number) { num_opts = -1;  num_retries=0; phone_number=_phone_number;}
    
    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t   Send To MoMo User\n";
        if (this->phone_number.empty()){
           std::cout << "\n\t   Enter Phone Number\n";
        }
        else{ std::cout << "\n\t   Confirm Phone Number\n";}
        
        draw_box(45);
        if(showPrompt==1){std::cout << "Phone Number==> ";}
        
    }
    Screen *handle_selection(long int selection);
};

class MomoScreen : public Screen
{
public:
    MomoScreen() { num_opts = 6; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t\t Transfer Money\n"
                     "\n\t\tSelect An Option\n\n";
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
    Screen *handle_selection(long int selection);
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

    Screen *handle_selection(long int selection);
};

Screen *MomoScreen::handle_selection(long int selection)
{
    switch (selection)
        {
        case 0: // Go back to home screen
            HomeScreen *hs_ptr;
            hs_ptr = new HomeScreen();
            hs_ptr->display();
            return hs_ptr;
        case 1: //  transfer to momo user screen
            SendToMomoUserScreen *stms_ptr;
            stms_ptr = new SendToMomoUserScreen();
            stms_ptr->display();
            return stms_ptr;
        default:
            this->display(false);
            std::cout << "sorry, this feature is not yet available\n"
                "Please Select another option\n\nEnter your selection==>";
            return this;
        }
}

Screen *HomeScreen::handle_selection(long int selection)
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
        this->display(false);
        std::cout << "sorry, this feature is not yet available\n"
                     "Please Select another option\n\nEnter your selection==>";
        return this;
    }
}

Screen *SendToMomoUserScreen::handle_selection(long int selection)
{   
    std::string phone_num= phone_num_to_str(selection);
    if (is_mtn_number(phone_num) == 1){ // validate input is mtn number 
        // store and return to confirm number 
        // todo 
        if (this->phone_number.empty()){ // if no phone num then we confirm
            SendToMomoUserScreen *stms_ptr;
            stms_ptr = new SendToMomoUserScreen(phone_num);
            stms_ptr->display();
            return stms_ptr;
        }else{ // if phone num then this  is confirmation 

            if (phone_num == this->phone_number){
                //if valid redirect to next screen TODO
                TransactionAmtScreen *tamt_ptr;
                tamt_ptr = new TransactionAmtScreen(phone_num);
                tamt_ptr->display();
                return tamt_ptr;
            }
        }
    } 
    // invalid num ask to retry. 
    if (this->num_retries < 3){
        this->num_retries++;
            this->display(false); //  show current screen again
        if (this->phone_number.empty()){
             std::cout<<"Please Enter a valid phone number\nEnter phone number ==>  ";
        }else{
            std::cout<<"The phone numbers do not match\nEnter phone number ==>  ";
        }
       
        return this;
    }else{
        // after 3 retries return to home screen
        // todo transaction failed screen;
        show_transaction_failed_message();
        HomeScreen *hs_ptr;
        hs_ptr = new HomeScreen();
        hs_ptr->display();
        return hs_ptr;
    }

}

 Screen *TransactionAmtScreen::handle_selection(long int selection){
     std::cout<<"Sending "<<selection<<"to "<<phone_number<<std::endl;
    

    //check if amt is available in account
    // if amt is unavailable retry.
    // if available send to confirmation page 
     return this;
 }



int main()
{
    user_account_ptr =  new UserAccount();

    show_welcome_message(); // display welcome message
    // wait for welcome message to display and clear screen
    HomeScreen home_screen; // create the home screen object and display menuL options
    Screen *currentScreen = &home_screen; // create a pointer to the current screen using 
                                          //the base class so all subclasses can access pointer
    currentScreen->display();             // display the currnent screen and
    std::string input;                    //get  user input
    while (isRunning) // listen for commands
    {
        getline(std::cin, input);
        long int opt = validate_input(input, currentScreen->num_opts); 
        clear_screen();
        // if input is valid call handler 
        if (opt >-1){currentScreen = currentScreen->handle_selection(opt);}
        else{ // retry for valid option
            currentScreen->display(false); // clear the screen and display  options
            std::cerr << "Provide a valid option\n==>  ";
        }
    }

    return 0;
}
