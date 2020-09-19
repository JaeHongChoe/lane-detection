#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <termio.h>
#include <unistd.h>
#include <time.h>
#include <geometry_msgs/Twist.h>


int kbhit();
int getch();


int getSpeed(){
	return rand() % 200;
}

int getAngle(){
	return 30 + rand() % 120;
}



int main(int argc, char **argv)
{
	srand((unsigned)time(NULL));
    int temp = 0;

    printf("Nice to see you.\n");
	//scanf("%d", &temp);
	//return 0;

	ros::init(argc, argv, "msg_publisher");
	ros::NodeHandle nh3;

	//ros::Publisher pub2 = nh3.advertise<std_msgs::Int8>("data_msg", 100);
	ros::Publisher pub2 = nh3.advertise<geometry_msgs::Twist>("data_msg", 100);
	//ros::Rate loop_rate(1);
	geometry_msgs::Twist cmd;
    std_msgs::Int8 msg2;

	int key = 0;

	printf("\nESC to quit");
	int spd_send=30;
	int angle_send=90;

	int cmdSpeed = 0; // hallym
    int cmdAngle = 0; // hallym

	int cnt = 0;
	int COUNT_MAX = 50;
	while(true)
	{
		if( ++cnt >= COUNT_MAX )
			break;

		cmdSpeed = getSpeed();
        cmdAngle = getAngle();

		cmd.linear.x=cmdSpeed;
		cmd.angular.z=cmdAngle;

		printf("Current speed (%d), angle (%d)\n", cmdSpeed, cmdAngle);

/*
		if(kbhit){
			key = getch();
		}else{
			key = 0;
		}

		switch(key){
			case 27:
            	return 0;
            	break;
			case 'w':
				cmd.linear.x=spd_send++;
				break;
			case 'a':
				cmd.angular.z=angle_send--;
				break;
			case 'd':
				cmd.angular.z=angle_send++;
				break;
			case 'x':
				cmd.linear.x=spd_send--;
				break;
			case 's':
				spd_send=0;
				cmd.linear.x=spd_send;
				cmd.angular.z=angle_send;
				break;
		}
*/
		pub2.publish(cmd);

	}

	return 0;
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF)
    {
    ungetc(ch, stdin);
    return 1;
    }
    return 0;
}
int getch(){
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}
