# FROM resin/rpi-raspbian:wheezy
# FROM dockcross/linux-armv6
FROM multiarch/debian-debootstrap:armhf-wheezy 
 
RUN echo "deb http://archive.debian.org/debian wheezy main" > /etc/apt/sources.list
RUN echo "deb http://archive.debian.org/debian-security wheezy/updates main" >> /etc/apt/sources.list

RUN apt-get update
RUN apt-get install -y --force-yes \
    make \
    build-essential \
    libglib2.0-bin \
    clang++ \
    libmagick++-dev \
    libglibmm-2.4-dev \
    g++-4.7 \
    vim

CMD [ "bash" ]
