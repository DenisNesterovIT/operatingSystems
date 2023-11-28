#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>

int terminate(int isTerm, struct input_event ev){
	if(ev.code == KEY_E && isTerm == 2){
		printf("\nEND\n");
		return 3;
	}else if(ev.code == KEY_X && isTerm == 1){
		printf("\nEND\n");
		return 3;
	}else if(ev.code == KEY_X){
		return 2;
	}else if(ev.code == KEY_E){
		return 1;
	}else{
		return 0;
	}
}

int first_shortcut(int firstSh, struct input_event ev){
	if(ev.code == KEY_P && firstSh == 2){
		printf("\nI passed the Exam!\n");
		return 0;
    }else if(ev.code == KEY_E && firstSh == 1){
        printf("\nI passed the Exam!\n");
        return 1;
    }else if(ev.code == KEY_E){
        return 2;
    }else if(ev.code == KEY_P){
        return 1;
	}else{
        return 0;
	}
}

int second_shortcut(int secondSh[], struct input_event ev){
	if (ev.code == KEY_C)
	{
		secondSh[0] = 1;
	}
	else if (ev.code == KEY_A)
	{
		secondSh[1] = 1;
	}else if (ev.code == KEY_P)
	{
		secondSh[2] = 1;
	}else{
		for (int i = 0; i < 3; i++)
		{
			secondSh[i] = 0;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (secondSh[i] == 0)
		{
			return 0;
		}
	}
	return 1;
}

int custom_shortcut(int customSh, struct input_event ev){
	if(ev.code == KEY_H && customSh == 2){
		printf("\nHello World!\n");
		return 0;
    }else if(ev.code == KEY_W && customSh == 1){
        printf("\nHello World!\n");
        return 1;
    }else if(ev.code == KEY_W){
        return 2;
    }else if(ev.code == KEY_H){
        return 1;
	}else{
        return 0;
	}
}

int main(){
	struct input_event ev;
	int fd = open("/dev/input/event2", O_RDONLY);
	int isTerm = 0;
	int firstSh = 0;
	int secondSh[3] = {0, 0, 0};
	int customSh = 0;
	while(1){
		read(fd, &ev, sizeof(struct input_event));
		char *type;
		if(ev.type == EV_KEY){
		if(ev.value == 0){
			type = "RELEASED";
		}else if (ev.value == 1){
			type = "PRESSED";
		}else {
			type = "REPEATED";
		}
		printf("\n%s 0x%04x %d\n",type, ev.code, ev.code);
		if((isTerm = terminate(isTerm, ev)) == 3){
			return EXIT_SUCCESS;
		}
		firstSh = first_shortcut(firstSh, ev);
		if (second_shortcut(secondSh, ev) == 1)
		{
			printf("\nGet some cappuccino!");
		}
		customSh = custom_shortcut(customSh, ev);
		}
	}
	close(fd);
	return EXIT_SUCCESS;
}
