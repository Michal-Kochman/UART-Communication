//g++ -pthread main.cpp UART.cpp 

#include <iostream>
#include <cstring>
#include <thread>
#include <termios.h>
#include <unistd.h> 
#include "uart.hpp"
#include "terminal_colours.h"

#define SETUP_MSG T_CYN "For port selection press 's'\n\rFor baudrate settings press 'b' (only after port is set and succesfully opened)\n \rTo start the terminal press 'g'\n \rTo terminate program, press 'x'. \n\rTo enter this setup during run, press '&' \n\r" T_RES

using namespace std;

void read_n_send();
void recieve_n_print();
void call_termios(int reset);
int setup();

term_t terminal;
bool run;


int main()
{
 
 char name[20];
 run = true;
 setup();
 if (!run) return 0;

 //start thread for recieving thread
 thread rec(recieve_n_print);

 read_n_send();
 //recieve_n_print();
 rec.join();
 call_termios(1);
 terminal.close_serial();
 return 0;
}


void read_n_send()
{
 cout<<T_GRN "Read and send\n" T_RES;
 char c='\0';
 while(run)
 {
  c=getchar();
  switch (c)
  {
   case '&':
    call_termios(1);
    setup();
    if (!run)terminal.send_char('&');
    break;
   case '\n':terminal.send_char('\r');   
   default: terminal.send_char(c);
  }
 }
 cout<<T_GRN "Read and send says goodbye" T_RES <<endl;
}

void recieve_n_print()
{
 cout<<T_GRN "Recieve and print\n" T_RES;

 while(run)
 {
  char c='\0';
  c=terminal.get_char();
  if (c == -1) continue;
  switch (c)
  {
  /*case '&':
   run=false;
   terminal.send_char('&');
   //cin.putback('&');
   break ;*/
  case '\r':
    cout<<"\n\r";
    break;
  case '\n':
    cout<<'\r';
  default:
    cout<<c<<'\0';
  }

 }
 cout<<T_GRN "Recieve and print says goodbye\n\r" T_RES;
}

void call_termios(int reset){
   static struct termios tio, tioOld;
   tcgetattr(STDIN_FILENO, &tio);
   if (reset) {
     // cout<<T_RED "-NORMAL-" T_RES;
      tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);
   } else {
    //  cout<<T_RED "-RAW-" T_RES;
      tioOld = tio; //backup 
      cfmakeraw(&tio);
      tio.c_oflag |= OPOST;
      tcsetattr(STDIN_FILENO, TCSANOW, &tio);
   }
}

int setup()
{
 bool cycle = true;
 char choice;
 cout<<SETUP_MSG;
 while (cycle)
 {

  call_termios(0);
  choice = getchar();
  call_termios(1);

  switch (choice)
  {  
    case 's':
      char port_name[50];
      cout<<"Path: ";
      cin>>port_name;
      if (terminal.is_opened())
      {
        cout<<"Found opened terminal, closing...\n\r";
        terminal.close_serial();
      }
      if (terminal.open_serial(port_name, 9600) != 0) cout<<"FAILED TO SET PORT\n\r" << SETUP_MSG;
      else cout<<" - Port opened at " << port_name <<endl;
      getchar();
      break;
    case 'b':
      if (!terminal.is_opened())
       {
        cout<<"Can not set baudrate, no port opened\n\r";
        break;
       }
      cout<<"Please specify the baudrate: ";
      int bd;
      cin>>bd;
      if (terminal.change_baudrate(bd) ==-1) cout<<"\n\rUnsupported baudrate, no changes\n\r";
      cout<< "\n\r";
      getchar();
      break;
    case 'g':
      if(terminal.is_opened()) 
      {
        call_termios(0);
        return 0;
      }
      cout<<"Unable to comply, no terminal opened\n\r";
      break;
    case 'x':
      cout<<endl;
      run=false;
      call_termios(1);
      return 0;
   }
  }
  
}

