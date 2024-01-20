FROM gcc:13.2.0
RUN \
    apt update && \
    apt upgrade -y && \
    apt install -y cmake gdb valgrind vim

ARG USERNAME=devcontainer
ARG USER_UID=1000
ARG USER_GID=$USER_UID


# Create the user
RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME}

ENTRYPOINT /bin/bash