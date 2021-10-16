FROM alexdegtyar/og-dependencies:x86-v1

ENV BRANCH="develop"
ENV ARTIFACTS=""
ENV ARTIFACTS_FOLDER="/var/og-artifacts"

ENV PROJECT_ROOT="OpenGreenery/src/device"
ENV BUILD_DIRECTORY="${PROJECT_ROOT}/build"

CMD git clone --recursive -b ${BRANCH} --single-branch \
    https://github.com/OpenGreenery/OpenGreenery.git && \
    mkdir -p ${BUILD_DIRECTORY} && \
    cmake -S ${PROJECT_ROOT} -B ${BUILD_DIRECTORY} && \
    cmake --build ${BUILD_DIRECTORY} && \
    for f in ${ARTIFACTS}; do \
        echo Copy artifact ${f}; \
        cp ${BUILD_DIRECTORY}/${f} ${ARTIFACTS_FOLDER}; \
    done
