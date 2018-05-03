#имя базового образа
FROM ubuntu

RUN apt update 
RUN apt install -y make 
RUN apt install -y gcc

#создаем папку, где будет наша программа
RUN mkdir -p /home/lab5/bin

#идем в папку
WORKDIR /home/work

#копируем все файлы из текущего пути к файлу Docker на вашей системе в нашу новую папку образа
COPY ./src /home/work

COPY ./init /home/work

CMD ./init 
