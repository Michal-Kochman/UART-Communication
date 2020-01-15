#include <iostream>
#include <termios.h>
#include <cerrno>
#include <fcntl.h>
#include <cstring>
#include <unistd.h> 

#ifndef BUFF_SIZE
#define BUFF_SIZE 64
#endif

class term_t
{
/*holds the file descriptor to the serial port and functions to open/close/send/recieve*/
public:
  int open_serial(const char* port, const int baud_rate);
  int change_baudrate(const int baud_rate);
  int close_serial();
  int send_string(char* c);
  int read_string(char* ret);
  term_t();
  term_t(const char* port, const int baud_rate);
  ~term_t();
private:
  int fd;
};

main(){return 0;}

term_t::term_t(void) 
{
   fd = -1;
}

term_t::term_t(const char* port, const int baud_rate)
{
  if (open_serial(port, baud_rate) !=0)
  {
        std::cerr<<"ERROR: failed to inicialize port, exiting" << '\n';
        fd = -1;
  }
}

term_t::~term_t(void) 
{
   close_serial();
}

int term_t::open_serial(const char* port, const int baud_rate)
{/*open serial port (file) and return it's descriptor*/
  fd = open(port, O_RDWR | O_NOCTTY); //file descriptor of "port" file in read-write mode, non-ctrl terminal 
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
  
  return 0;
}

int term_t::change_baudrate(const int baud_rate)
{
  struct termios trm;
  if (tcgetattr(fd, &trm) !=0) return -1;
  if (cfsetispeed(&trm, baud_rate) !=0 ||  cfsetospeed(&trm, baud_rate)) return -1;
  tcflush(fd, TCIFLUSH); //discard any unread characters
  if (tcsetattr(fd, TCSANOW, &trm) !=0) return -1;
  return 0;
}

int term_t::close_serial()
{
  return close(fd);
}

int term_t::send_string(char* c)
{
   int len = strlen(c);
   return write(fd, c, len);
}

int term_t::read_string(char*rec)
{
  return read(fd, rec, BUFF_SIZE);
}

