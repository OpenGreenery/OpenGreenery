FROM navikey/raspbian-buster

RUN apt-get update && \
	apt-get install -y \
	git \
	cmake \
	build-essential \
	libgtest-dev \
	qt5-default \
	libqt5charts5-dev

COPY ./clone-and-build.sh /var/clone-and-build.sh

ENV BRANCH=develop
ENV COPYPATH=tests/database_tests
ENV VOL=/var/og-artifacts

WORKDIR /var
CMD ["/bin/bash", "-c", "./clone-and-build.sh ${BRANCH} /var/OpenGreenery/build/${COPYPATH} ${VOL}"]
