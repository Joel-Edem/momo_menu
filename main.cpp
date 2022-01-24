#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else 
    #include <unistd.h>
#endif


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

struct UserAccount{

    std::string phone_number;
    int balance ;
    int pin;
    int data;
    int airtime;
    int mins;
    UserAccount(
        std::string _phone_number = "0553272851",
        int _balance= 1000, int _pin=1234, int _data=500,
         int _airtime=20, int _mins=30)
        {
            phone_number = _phone_number;
            balance = _balance;
            pin =_pin;
            data=_data;
            mins=_mins;
            airtime=_airtime;
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
    draw_box(45);std::cout<<"\n\n\n\t  Transaction Failed\n\n\n";draw_box(45);
    sleep(2);clear_screen();
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

void show_success_message(std::string recipient, int amount)
{
    draw_box(45);std::cout<<"\n\n\n"
                            "\t\tSucess\n\n"
                            "\tYou have transfered GHS¢ "<<amount<<"\n"
                            "\tto "<<recipient<<".\n"
                            "\tYour new balance is GHS¢" <<user_account_ptr->balance<<"\n"
                            "\n\n\n";draw_box(45);
    sleep(3);clear_screen();
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
    std::string network;
    TransactionAmtScreen(std::string _phone_number, std::string _network="MoMo") 
    { num_opts = -1;  num_retries=0; phone_number=_phone_number; network=_network;}
    
    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t   Send To "<<network<<" User\n";
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

class SendToOtherNetworkScreen : public Screen
{
public:
    int num_retries;
    std::string phone_number;
    std::string network;
    SendToOtherNetworkScreen() { num_opts = -1;  num_retries=0;}
    SendToOtherNetworkScreen( std::string _network) 
    { num_opts = -1;  num_retries=0; network=_network;}
    SendToOtherNetworkScreen(std::string _phone_number, std::string _network) 
    { num_opts = -1;  num_retries=0; phone_number=_phone_number; network=_network;}
    
    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t   Send To "<<network<<" User\n";
        if (this->phone_number.empty()){
           std::cout << "\n\t   Enter Phone Number\n";
        }
        else{ std::cout << "\n\t   Confirm Phone Number\n";}
        
        draw_box(45);
        if(showPrompt==1){std::cout << "Phone Number==> ";}
        
    }
    Screen *handle_selection(long int selection);
};

class SelectNetworkScreen : public Screen
{
public:
    SelectNetworkScreen() { num_opts = 3; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t\t Send to Other Network\n"
                     "\n\t\tSelect Network\n\n";
        std::cout << "\t1\tVodafone\n"
                     "\t2\tAirtelTigo\n"
                     "\t3\tGlo\n"
                     "\t0\tBack\n\n";
        draw_box(45);
        if(showPrompt == 1){std::cout << "Enter your selection==> ";}
    }
    Screen *handle_selection(long int selection);
};

class MyWalletScreen : public Screen
{
public:
    MyWalletScreen() { num_opts = 2; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t\t My Wallet\n"
                     "\n\t\tSelect Option\n\n";
        std::cout << "\t1\tView Balance\n"
                     "\t2 *\tTransaction History\n"
                     "\t0\tBack\n\n";
        draw_box(45);
        if(showPrompt == 1){std::cout << "Enter your selection==> ";}
    }
    Screen *handle_selection(long int selection);
};

class AllowCashOutScreen : public Screen
{
public:
    AllowCashOutScreen() { num_opts = 1; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\tAllow Cashout\n"
                     "\n\tAre you sure you want to allow Cash out?\n\n";
        std::cout << "\t1\tYes, Allow Cash Out\n"
                     "\t0\tCancel\n\n";
        draw_box(45);
        if(showPrompt == 1){std::cout << "Enter your selection==> ";}
    }
    Screen *handle_selection(long int selection);
};

class BalanceScreen : public Screen
{
public:
    BalanceScreen() { num_opts = 1; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t\t My Wallet\n"
                     "\n\t\tCurrent Balance\n\n";

        std::cout << "\tCurrent Momo Balance\tGHS¢ "<<user_account_ptr->balance<<"\n"
                     "\tAirtime             \tGHS¢ "<<user_account_ptr->airtime<<"\n"
                     "\tMinutes             \t"<<user_account_ptr->mins<<" Mins\n"
                     "\tData                \t"<<user_account_ptr->data<<" Mb\n"

                     "\n\n\t\tPress 0 Back\n\n";
        draw_box(45);
    }
    Screen *handle_selection(long int selection);
};

class ConfirmTransactionScreen : public Screen
{
public:
    std::string phone_number;
    int amount;
    ConfirmTransactionScreen(std::string _phone_number, int _amount)
     { phone_number=_phone_number;
        amount =_amount;
        num_opts = -1; } 

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\tConfirm transaction\n"
                     "\n\tSend GHS ¢"<<amount<<" to "<< phone_number<<"?\n\n";
        std::cout <<"\t1\tConfirm\n"
                    "\t0\tCancel\n\n";
        draw_box(45);
        if(showPrompt == 1){std::cout << "Enter your selection==> ";}
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
                     "\t3 *\tSend With Care\n"
                     "\t4 *\tFavourite\n"
                     "\t5\tOther Networks\n"
                     "\t6 *\tBank Account\n"
                     "\t0\tMain Menu\n\n";
        draw_box(45);
        if(showPrompt == 1){std::cout << "Enter your selection==> ";}
    }
    Screen *handle_selection(long int selection);
};

class VerifyPasswordScreen:public Screen{
public:
    std::string phone_number;
    int amount;
    int num_retries;
    Screen *next_screen;
    VerifyPasswordScreen(Screen *_next_screen){
        num_opts = -1;
        num_retries =0; 
        next_screen = _next_screen;
    }
    VerifyPasswordScreen(std::string _phone_number, int _amount)
     { phone_number=_phone_number;
        amount =_amount;
        num_opts = -1;
        num_retries =0; } 

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\t\tEnter your PIN\n\n\n";
        draw_box(45);
        if(showPrompt == 1){std::cout << "PIN ==> ";}
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
                     "\t2 *\tMoMoPay & Pay Bill\n"
                     "\t3 *\tAirtime & Bundles\n"
                     "\t4 \tAllow Cash Out\n"
                     "\t5 *\tFinancial Services\n"
                     "\t6 \tMy Wallet\n"
                     "\t0\texit\n\n";
        draw_box(45);
        if(showPrompt==1){std::cout << "Enter your selection==> ";}
    }

    Screen *handle_selection(long int selection);
};

class CashoutAllowedScreen : public Screen
{
public:
    CashoutAllowedScreen() { num_opts = -1; }

    void display(bool showPrompt=true)
    {
        //displays options
        draw_box(45);
        std::cout << "\n\tCash out \n"
                     "\n\tCash Out Allowed\n\n";
        draw_box(45);
        sleep(2);
        clear_screen();
        handle_selection(0);
    }
    Screen *handle_selection(long int selection);
};

HomeScreen *return_home(){
     HomeScreen *hs_ptr;
        hs_ptr = new HomeScreen();
        hs_ptr->display();
        return hs_ptr;
}

Screen *MomoScreen::handle_selection(long int selection)
{
    switch (selection)
        {
        case 0: // Go back to home screen
            return return_home();
        case 1: //  transfer to momo user screen
            SendToMomoUserScreen *stms_ptr;
            stms_ptr = new SendToMomoUserScreen();
            stms_ptr->display();
            return stms_ptr;
        case 2: case 5:
             SelectNetworkScreen *sns_ptr;
                sns_ptr = new SelectNetworkScreen();
                sns_ptr->display();
                return sns_ptr;
        default:
            this->display(false);
            std::cout << "sorry, this feature is not yet available\n"
                "Please Select another option\n\nEnter your selection==>";
            return this;
        }
}

Screen *SelectNetworkScreen::handle_selection(long int selection)
{   
    std::string network;
    switch (selection)
        {
        case 0: // Go back to home screen
            MomoScreen *ms_ptr;
            ms_ptr = new MomoScreen();
            ms_ptr->display();
            return ms_ptr;
        case 1: //  transfer to momo user screen
            network = "Vodafone";
            break;
        case 2:
            network = "Airtel";
            break;
        case 3:
            network = "Glo";
            break;
        default:
            this->display(false);
            std::cout << "Invalid Selection\n"
                "Please Select another option\n\nEnter your selection==>";
            return this;
        }
        SendToOtherNetworkScreen *stons_ptr;
        stons_ptr = new SendToOtherNetworkScreen(network);
        stons_ptr->display();
        return stons_ptr;
}

Screen *MyWalletScreen::handle_selection(long int selection)
{   
    switch (selection)
        {
        case 0: // Go back to home screen
            return return_home();
        case 1: //  Balance Screen

            BalanceScreen *bs_ptr;
            bs_ptr = new BalanceScreen();

            VerifyPasswordScreen *vp_ptr;
            vp_ptr = new VerifyPasswordScreen(bs_ptr);
            vp_ptr->display();
            return vp_ptr;
        default:
            this->display(false);
            std::cout << "Sorry this feature is currently not available\n"
                "Please Select another option\n\nEnter your selection==>";
            return this;
        }

}

Screen *AllowCashOutScreen::handle_selection(long int selection)
{   

    if (selection == 1){
        CashoutAllowedScreen *coas_ptr;
        coas_ptr = new CashoutAllowedScreen();

        VerifyPasswordScreen *vp_ptr;
        vp_ptr = new VerifyPasswordScreen(coas_ptr);
        vp_ptr->display();
        return vp_ptr;
    }
    return return_home();

}

Screen *CashoutAllowedScreen::handle_selection(long int selection)
{   
    
    return return_home();

}

Screen *BalanceScreen::handle_selection(long int selection)
{   
    MyWalletScreen *mw_ptr;
    mw_ptr = new MyWalletScreen();
    mw_ptr->display();
    return mw_ptr;
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
    case 4:
        AllowCashOutScreen *aco_ptr;
        aco_ptr = new AllowCashOutScreen();
        aco_ptr->display();
        return aco_ptr;
    case 6:
        MyWalletScreen *mw_ptr;
        mw_ptr = new MyWalletScreen();
        mw_ptr->display();
        return mw_ptr;
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
        return return_home();
    }

}

Screen *SendToOtherNetworkScreen::handle_selection(long int selection)
{   
    std::string phone_num= phone_num_to_str(selection);
    if (is_mtn_number(phone_num) != 1){ // validate input is mtn number 
        // store and return to confirm number 
        // todo 
        if (this->phone_number.empty()){ // if no phone num then we confirm
            SendToOtherNetworkScreen *stons_ptr;
            stons_ptr = new SendToOtherNetworkScreen(phone_num, this->network);
            stons_ptr->display();
            return stons_ptr;
        }else{ // if phone num then this  is confirmation 

            if (phone_num == this->phone_number){
                //if valid redirect to next screen TODO
                TransactionAmtScreen *tamt_ptr;
                tamt_ptr = new TransactionAmtScreen(phone_num, this->network);
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
        return return_home();
    }

}

Screen *TransactionAmtScreen::handle_selection(long int selection){
    if (selection > user_account_ptr->balance){
        // amt too large
        //display insufficient fund msg
        if (this->num_retries < 3){
            this->num_retries++;
            draw_box(45);
            std::cout<<"\n\nInsufficient funds.\nYour current balance is GHS¢ "<<user_account_ptr->balance<<"\n\n"<<std::endl;
            draw_box(45);
            sleep(2); clear_screen();
            this->display();
            return this;
        }else{
            show_transaction_failed_message();
            return return_home();
        }

    }else{
        // send to confirmation screen
        ConfirmTransactionScreen *ms_ptr;
        ms_ptr = new ConfirmTransactionScreen(this->phone_number, selection);
        ms_ptr->display();
        return ms_ptr;
    }

     
 }

Screen *ConfirmTransactionScreen::handle_selection(long int selection){
    // if 1 perform transaction
    if(selection == 1){
        VerifyPasswordScreen *vps_ptr;
        vps_ptr = new VerifyPasswordScreen(this->phone_number, this->amount);
        vps_ptr->display();
        return vps_ptr;
    }else if(selection == 0){ // if 0 cancel
        show_transaction_failed_message();
        return return_home();
    }else{//else show screen again
        this->display(false);
        std::cout<<"Invalid Option\nSelect an option ==>";
        return this;
    }
}

Screen *VerifyPasswordScreen::handle_selection(long int selection){
    if (selection != user_account_ptr->pin){
        // amt too large
        //display insufficient fund msg
        if (this->num_retries < 3){
            this->num_retries++;
            draw_box(45);
            std::cout<<"\n\nInvalid Pin. Try again\nYou have " <<3-this->num_retries<<" attempts left\n\n"<<std::endl;
            draw_box(45);
            sleep(2); clear_screen();
            this->display();
            return this;
        }else{
            show_transaction_failed_message();
            return return_home();
        }

    }else{ // SUCCESS PRERFOM TRANSFER

        if(this->next_screen != nullptr){
            this->next_screen->display();
            return this->next_screen;      
        }
        else{
            // subtract from balance
            user_account_ptr->balance-=this->amount;
            // show success screen 
            show_success_message(this->phone_number, this->amount);
            // return home
            return return_home();            
        }

    }

     
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
