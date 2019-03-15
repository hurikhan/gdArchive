FROM opensuse/leap

WORKDIR /build

COPY ./build /build

RUN rm /etc/zypp/repos.d/*
COPY ./opensuse/repo-cn-2 /etc/zypp/repos.d

RUN zypper --non-interactive install -t pattern devel_C_C++
RUN zypper --non-interactive install cmake git fish hostname figlet libbz2-devel libzstd-devel xz-devel
RUN chsh -s /usr/bin/fish
RUN git clone --depth 1 https://github.com/libarchive/libarchive.git

CMD ./build.sh
