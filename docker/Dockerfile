FROM opensuse/leap

WORKDIR /worker
COPY ./worker/worker.sh /worker/worker.sh

#RUN rm /etc/zypp/repos.d/*
#COPY ./opensuse/repo-cn-1 /etc/zypp/repos.d
COPY ./opensuse/cache /var/cache/zypp
RUN zypper mr -a -k

RUN zypper --non-interactive install -t pattern devel_C_C++

RUN zypper --non-interactive --no-refresh install cmake git fish hostname figlet libbz2-devel libzstd-devel xz-devel \
	ninja gettext-tools vim python3-pip libarchive-devel clang

RUN pip3 install scons
RUN sed -i '1 s/python/python3/g' /usr/bin/scons

ENTRYPOINT ["/worker/worker.sh"]
