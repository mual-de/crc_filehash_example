FROM ubuntu:20.04

RUN apt-get -y update && apt-get install -y
RUN echo 'tzdata tzdata/Areas select Europe' | debconf-set-selections
RUN echo 'tzdata tzdata/Zones/Europe select Berlin' | debconf-set-selections
RUN apt install -y tzdata
RUN apt-get -y install build-essential gdb
RUN apt-get -y install cmake
RUN apt-get -y install git
RUN apt-get -y install libboost-all-dev

CMD ["bash"]