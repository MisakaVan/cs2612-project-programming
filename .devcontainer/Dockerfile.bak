FROM --platform=linux/amd64 ubuntu:20.04

# 更换为清华大学的镜像源
RUN echo "\n\
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ focal main restricted universe multiverse          \n\
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ focal-updates main restricted universe multiverse  \n\
deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ focal-backports main restricted universe multiverse\n\
deb http://security.ubuntu.com/ubuntu/ focal-security main restricted universe multiverse          \n\
" > /etc/apt/sources.list
# deb http://mirrors.tuna.tsinghua.edu.cn/ubuntu/ focal-security main restricted universe multiverse \n\

# RUN sed -i 's/http:\/\/archive.ubuntu.com/https:\/\/mirror.sjtu.edu.cn/g' /etc/apt/sources.list

# 安装必要的软件包
RUN apt-get update
#  && apt-get install -y \
#     build-essential \
#     curl \
#     git \
#     vim \
#     flex \
#     bison \
#     clangd \
#     && rm -rf /var/lib/apt/lists/*
RUN apt-get install -y build-essential
RUN apt-get install -y curl
RUN apt-get install -y git
RUN apt-get install -y vim
RUN apt-get install -y flex
RUN apt-get install -y bison
RUN apt-get install -y clangd

# remove the apt cache
RUN rm -rf /var/lib/apt/lists/* 
