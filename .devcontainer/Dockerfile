FROM gcc:13.2.0
RUN \
    apt update && \
    apt upgrade -y && \
    apt install -y cmake gdb valgrind vim lcov python3-pip python3-venv && \
    python3 -m venv /tmp/gcovr && \
    /tmp/gcovr/bin/pip install gcovr lxml

ARG USERNAME=devcontainer
ARG USER_UID=1000
ARG USER_GID=$USER_UID


# Create the user
RUN groupadd --gid ${USER_GID} ${USERNAME} \
    && useradd --uid ${USER_UID} --gid ${USER_GID} -m ${USERNAME}

ENV PATH="/tmp/gcovr/bin:${PATH}"
ENTRYPOINT /bin/bash