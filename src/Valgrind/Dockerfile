FROM alpine:3.20

RUN apk --no-cache --upgrade add \
    git \
    git-lfs\
    curl \
    jq \
    bash \
    dos2unix \
    subunit-dev \
    build-base \
    python3 \
    valgrind \
    cppcheck \
    alpine-sdk \
    pcre-dev \
    check \
    pcre2 \
    clang18-extra-tools && \
    ln -s /usr/lib/llvm18/bin/clang-format /usr/bin/clang-format

RUN curl -L https://github.com/libcheck/check/releases/latest/download/check-0.15.2.tar.gz -o check.tar.gz && \
    tar -xzf check.tar.gz && \
    cd check-0.15.2 && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -rf check-0.15.2 check.tar.gz

WORKDIR /usr/src/

COPY ./src /usr/src/
COPY ./src/Valgrind/* /usr/src/

USER root

ENTRYPOINT ["/bin/bash", "start.sh"]