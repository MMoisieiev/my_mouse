my_mouse:
	gcc -Wall -Wextra -Werror -o mouse mouse.c

my_mouse.o: bc-main.c
	gcc -Wall -Wextra -Werror -c mouse.c

clean:
	rm mouse

fclean: clean
	rm mouse

re: fclean mouse