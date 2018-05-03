#имя базового образа
FROM debian

RUN apt update && apt install -y make gcc

#создаем папку, где будет наша программа
RUN mkdir -p /home/lab5/bin

#идем в папку
WORKDIR /home/work

#копируем все файлы из текущего пути к файлу Docker на вашей системе в нашу новую папку образа
COPY ./lab5.c /home/work

COPY ./Makefile /home/work

COPY ./init /home/work

CMD ./init 
