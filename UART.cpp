#include <iostream>
#include <termios.h>
#include <cerrno>
#include <fcntl.h>
#include <cstring>
#include <unistd.h> 

#include "uart.hpp"


term_t::term_t(void) 
{ 
   fd = -1;
   
}

term_t::term_t(char* port)
{  std::cout<<"Inicitializing port\n";
  if (open_serial(port, BAUD_RATE) != 0)
  {
        std::cerr<<"ERROR: failed to inicialize port, exiting" << '\n';
        fd = -1;
  }
}

term_t::~term_t(void) 
{
   if (fd != -1) close_serial();
}

int term_t::open_serial(char* port,int baud_rate)
{/*open serial port (file)*/
  fd = open(port, O_RDWR | O_NOCTTY| O_SYNC); //file descriptor of "port" file in read-write mode, non-ctrl terminal 
  /* check if the serial port was opened correctly*/ 
  if (fd ==-1 || !isatty(fd))
  {
    std::cerr<<"ERROR: failed to open serial port" << '\n';
    return -1;
  }
  /*set the baudrate*/
  if (change_baudrate(baud_rate) !=0)
  {
    std::cerr<<"ERROR: failed to set initial baudrate" << '\n';
    return -1;
  }
  struct termios trm;
  if (tcgetattr(fd, &trm) !=0) return -1;
  cfmakeraw(&trm);
  trm.c_cc[VMIN] = 0;
  trm.c_cc[VTIME] = 5;
  tcsetattr(fd, TCSANOW, &trm);
   // Set the serial port to non block mode
   //int flags = fcntl(fd, F_GETFL);
   //flags &= O_NONBLOCK;
   //fcntl(fd, F_SETFL, flags);
   

   
  return 0;
}

int term_t::change_baudrate(int baud_rate)
{
  struct termios trm;
  if (tcgetattr(fd, &trm) !=0) return -1;
  int bdr = int_to_bd(baud_rate);
  if (bdr == -1) return -1;
  if (cfsetispeed(&trm, bdr) !=0 ||  cfsetospeed(&trm, bdr) != 0) return -1;
  tcflush(fd, TCIFLUSH); //discard any unread characters
  if (tcsetattr(fd, TCSANOW, &trm) !=0) return -1;
  return 0;
}

int term_t::close_serial()
{
  if (close(fd)==0)
  {
    fd=-1;
    return 0;
  };
  return -1;
}

int term_t::send_char(char c)
{
   return write(fd, &c, 1);
}

char term_t::get_char()
{ 
   char c;
   int r = read(fd, &c, sizeof(char));
   return r == 1 ? c : -1;
}

int term_t::int_to_bd(int wanted)
{
    switch (wanted) {
    case 9600:
        return B9600;
    case 19200:
        return B19200;
    case 38400:
        return B38400;
    case 57600:
        return B57600;
    case 115200:
        return B115200;
    case 230400:
        return B230400;
    case 460800:
        return B460800;
    case 500000:
        return B500000;
    case 576000:
        return B576000;
    case 921600:
        return B921600;
    case 1000000:
        return B1000000;
    case 1152000:
        return B1152000;
    case 1500000:
        return B1500000;
    case 2000000:
        return B2000000;
    case 2500000:
        return B2500000;
    case 3000000:
        return B3000000;
    case 3500000:
        return B3500000;
    case 4000000:
        return B4000000;
    default: 
        return -1;
    }
}

bool term_t::is_opened()
{
  return fd == -1 ? false : true;
}
