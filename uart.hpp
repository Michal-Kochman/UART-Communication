
#ifndef BUFF_SIZE
#define BUFF_SIZE 64
#endif

#ifndef BAUD_RATE
#define BAUD_RATE 115200
#endif

class term_t
{
/*holds the file descriptor to the serial port and functions to open/close/send/recieve*/
public:
  /*-------------------------------------------------------
  brief open_serial:   open serial port (file). 
  param: port - string with path to port
  param: baud_rate - integer designaing baudrate
  return: 0 on succes, otherwise -1
  --------------------------------------------------------*/
  int open_serial(char* port, int baud_rate); 
  /*-------------------------------------------------------
  brief change_baudrate:   set baudrate to baud_rate 
  param: baudrate - integer baudrate
  return: 0 on succes, otherwise -1
  --------------------------------------------------------*/
  int change_baudrate(int baud_rate);
  /*-------------------------------------------------------
  brief close_serial:   close serial port
  return: 0 on succes, otherwise -1
  --------------------------------------------------------*/
  int close_serial() ;
  /*-------------------------------------------------------
  brief char:   send char c via serial port 
  param: char - character to send
  return: 0 on succes, otherwise -1
  --------------------------------------------------------*/
  int send_char(char c);
  /*-------------------------------------------------------
  brief get_char:   read ASCII char from serial
  return: character
  --------------------------------------------------------*/
  bool is_opened(); 
  /*brief is_opened:   
  return: TRUE if serial port is opened, otherwise false
  ------------------------------------*/
  char get_char(); 
  /*default class constructor*/
  term_t();
   /*class constructor, opens 'port' with default baudrate*/
  term_t(char* port);
  /*class destructor, closes the port*/
  ~term_t();
private:
  /*file descriptor*/
  int fd;
  /*convert int to size_t for baudrate*/
  int int_to_bd(int wanted);
};
