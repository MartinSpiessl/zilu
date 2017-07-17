FROM klee/klee
MAINTAINER lijiaying <lijiaying1989@gmail.com>

# FIXME: Docker doesn't currently offer a way to
# squash the layers from within a Dockerfile so
# the resulting image is unnecessarily large!

ENV KLEE_SRC=/home/klee/klee_src \
    KLEE_BLD=/home/klee/klee_build \
    ZILU_SRC=/home/klee/zilu_src

USER root
RUN apt-get -y --no-install-recommends install \
		libgsl0-dev \
		libz3-dev \
		z3 && \
		mkdir ${ZILU_SRC}

# Copy across source files needed for build
#git clone https://github.com/lijiaying/ZILU.git ${ZILU_SRC}
ADD / ${ZILU_SRC}
	
RUN cd ${KLEE_SRC} && \
	git apply ${ZILU_SRC}/klee.patch && \
	cd ${KLEE_BLD}/klee && \
	make && \
	make install

RUN cd ${ZILU_SRC}/parser && \
		mkdir build && \
		cd build && \
		cmake .. && \
		make > /dev/null 2>&1 || mv parser.hpp .. && \
		cd .. && \
		rm -rf build

# Set klee user to be owner
RUN sudo chown --recursive klee: ${KLEE_SRC}
RUN sudo chown --recursive klee: ${KLEE_BLD}
RUN sudo chown --recursive klee: ${ZILU_SRC}

