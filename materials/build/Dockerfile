FROM quay.io/school21/alpine:3.16

RUN apk --no-cache --upgrade add \
    git \
    git-lfs \
    curl \
    jq \
    bash \
    build-base \
    python3 \
    valgrind \
    cppcheck \
    alpine-sdk \
    pcre-dev \
    clang-extra-tools \
    util-linux \
    coreutils \
    gcompat

RUN mkdir /test 

COPY . /project
COPY materials/build/scripts /project/ci-scripts
COPY materials/build/scripts /project/tests
COPY materials/linters /project/tests/linters

WORKDIR "/project"

ENTRYPOINT ["/bin/bash","-c", "bash ci-scripts/test.sh && cd src/cat && bash tests.sh && cd ../grep && bash tests.sh"]
# ENTRYPOINT ["/bin/bash","-c", "ci-scripts/test.sh"]