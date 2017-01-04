all:
	gcc -O3 -o switch_c.cgi switch.c -lwiringPi
debug:
	gcc -g -o switch_c.cgi switch.c -lwiringPi
deploy: all
	sudo mv switch_c.cgi /var/www/html/cgi-bin/
	sudo chown www-data:www-data /var/www/html/cgi-bin/switch_c.cgi
run: all
	export QUERY_STRING="switch_1=on"; export WIRINGPI_CODES=1; sudo -E ./switch_c.cgi
