#
# MAINTAINER        Carson,C.J.Zeong <zcy@nicescale.com>
# DOCKER-VERSION    1.6.2
#
# Dockerizing CentOS7: Dockerfile for building CentOS images
#
FROM		ubuntu
MAINTAINER Li Jiaying<sutd@googlegroups.com>

#ENV TZ "Asia/Singapore"
#ENV TERM xterm

RUN apt-get update && apt-get install -y -qq g++ make cmake libgsl0-dev libz3-dev git cmake flex bison clang
#&& \
#		mkdir -p nklib

#RUN \
#		git clone https://github.com/Z3Prover/z3.git && \
#		cd z3 && \
#		python contrib/cmake/bootstrap.py create && \
#		mkdir build && \
#		cd build && \
#		cmake ../ && \
#		make -j && \
#		make install && \
#		rm -rf * 

ADD . nklib/

RUN \
		cd nklib && \
		mkdir -p build && \
		cd build && \
		cmake .. && \
		make && \
		make install && \
		rm -rf *

#EXPOSE 22
#ENTRYPOINT ["/bin/bash"]
