#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

const char *device = "/dev/ttyUSB0";

int
set_interface_attribs (int fd, int speed)
{
  struct termios tty;

  /*
  tcgetattr(fd, &old_tty);
  printf("iflag=%d\n", old_tty.c_iflag);
  printf("lflag=%d\n", old_tty.c_lflag);
  printf("cflag=%d\n", old_tty.c_cflag);
  printf("oflag=%d\n", old_tty.c_oflag);
  printf("speed=%d\n", cfgetispeed(&old_tty));
  */

  memset (&tty, 0, sizeof(tty));

  tty.c_iflag = 0;
  tty.c_lflag = 0;
  tty.c_cflag = CS8|CREAD|CLOCAL;
  tty.c_oflag = 0;
  tty.c_cc[VMIN] = 0;
  tty.c_cc[VTIME] = 5;

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    fprintf(stderr, "E: %d from tcsetattr", errno);
    return -1;
  }

  return 0;
}

void
clean_buffer(int fd)
{
  unsigned char d;
  while (read(fd, &d, 1) > 0) {
  }
}

unsigned int hex2int(char *a, unsigned int len)
{
  int i;
  unsigned int val = 0;

  for (i = 0; i < len; i++)
    if (a[i] <= 57)
      val += (a[i] - 48) * (1 << (4*(len-1-i)));
    else
      val += (a[i] - 55) * (1 << (4*(len-1-i)));

  return val;
}

int
read_serial(int fd, char* buffer, int len)
{
  int i;
  int j;
  unsigned char c;

  for (i = 0; i < len; i++) {

    j = 0;
    while (j < 300) {
      j++;
      usleep(500);
      if (read(fd, &c, 1) > 0) {
        buffer[i] = c;
        break;
      }
    }
  }
  return 0;
}

int
main(void) {

  int fd;
  int i = 0;
  unsigned char buffer[5];
  unsigned int neg;
  int T;
  char sign = ' ';

  puts("readtemp v0.2");

  fd = open(device, O_RDWR | O_NONBLOCK);
  if(fd == -1) {
    fprintf(stderr, "Failed to open port\n");
    return 1;
  }

  set_interface_attribs(fd, B2400);

  clean_buffer(fd);

  int bytes_written = write(fd, "T", 1);
  printf("BW=%d\n", bytes_written);

  read_serial(fd, buffer, 3);
  read_serial(fd, buffer, 4);

  buffer[4] = '\0';
  printf("R.BUF=%s\n", buffer);

  T = hex2int(buffer + 2, 2) << 8;
  T += hex2int(buffer, 2);

  neg = T >> 15;

  if (neg) {
    T--;
    T =~ T;
    T &= 0xFFFF;
    sign = '-';
  }

  printf("T = %c%.2f\n", sign, (float)(T)/16);

  close(fd);

  return 0;
}

