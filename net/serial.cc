#include "serial.h"

using namespace std;

Serial::Serial(const char* name, int flags)
{
	open(name, flags);
//	set_opt(115200, 8, 'N', 1);
}

Serial::~Serial()
{
	close();
}

int Serial::set_opt(int nSpeed, int nBits, char nEvent, int nStop)  //串口设置
{
	if(fd < 0) return fd;
	struct termios newtio,oldtio;
	if  ( tcgetattr(fd,&oldtio)  !=  0) { 
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD; 
	newtio.c_cflag &= ~CSIZE; 
//	newtio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
//	newtio.c_oflag &= ~OPOST;
	switch( nBits )
	{
	case 7:
		newtio.c_cflag |= CS7;
		break;
	case 8:
		newtio.c_cflag |= CS8;
		break;
	}

	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  
		newtio.c_cflag &= ~PARENB;
		break;
	}

	switch( nSpeed )
	{
	case 2400:
		cfsetispeed(&newtio, B2400);
		cfsetospeed(&newtio, B2400);
		break;
	case 4800:
		cfsetispeed(&newtio, B4800);
		cfsetospeed(&newtio, B4800);
		break;
	case 9600:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	case 115200:
		cfsetispeed(&newtio, B115200);
		cfsetospeed(&newtio, B115200);
		break;
	default:
		cfsetispeed(&newtio, B9600);
		cfsetospeed(&newtio, B9600);
		break;
	}
	if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else if ( nStop == 2 )
    	newtio.c_cflag |=  CSTOPB;
	newtio.c_cc[VTIME]  = 0;
	newtio.c_cc[VMIN] = 0;
	tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	return 0;
}

void Serial::setBlock(bool isBlock)
{
    if(fd < 0) return;
    int flags = fcntl(fd, F_GETFL, 0)|O_NONBLOCK;
    if(isBlock)
        flags &= ~O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

int Serial::open(const char* name, int flags)                 //通过参数，打开相应的串口
{
    do{
        if(flags & O_CREAT)
	        fd = ::open(name, flags, S_IRUSR | S_IWUSR);
        else
            fd = ::open(name, flags);
        cout << "open:" << name << " fd:" << fd << endl;
    }while(fd < 0);
    setBlock(true);
	return fd;
}

int Serial::close()
{
	cout <<"close: "<< fd << endl;
	if(fd >= 0)
	{
		::close(fd);
		fd = -1;
	}
}

int Serial::write(const string& str)  //写串口信息
{
	if(fd < 0) return fd;
    int total_len = str.size();
	u8* data = reinterpret_cast<unsigned char*>(const_cast<char*>(str.data()));
	while(total_len)
	{
		int len = ::write(fd, data, total_len);
		if(len < 0)
		{
            cout << "write error: "<< len << endl;
			close();
			return -1;
		}
		total_len -= len;
		data += len;
	}
	return str.size();
}

int Serial::read(string& str)   //读取串口信息
{
	if(fd < 0) return fd;
	u8 buff[1024];
    int readlen = ::read(fd, buff, sizeof(buff));
	if(readlen < 0)
	{
        cout << "read error: "<< readlen << endl;
		close();
		return -1;
	}
	str.append(reinterpret_cast<char*>(buff), readlen);
    return readlen;
}

