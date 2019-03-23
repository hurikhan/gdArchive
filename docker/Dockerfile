FROM opensuse/leap

WORKDIR /docker

# RUN rm /etc/zypp/repos.d/*
# COPY ./opensuse/repo-cn-3 /etc/zypp/repos.d

RUN zypper --non-interactive install -t pattern devel_C_C++ \
	cmake git fish hostname figlet libbz2-devel libzstd-devel xz-devel \
	scons ninja gettext-tools && \
	chsh -s /usr/bin/fish

CMD ['/usr/bin/bash']
